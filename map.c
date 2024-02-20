/**
  @file map.c
  @author John Butterfield (jpbutte2)
  Implementation for the map component, a hash map.
  The values will be kept in an array of pointers to
  linked lists.
*/

#include "map.h"
#include <stdlib.h>

#include "value.h"

typedef struct MapPairStruct MapPair;

/** Key/Value pair to put in a hash map. */
struct MapPairStruct {
  /** Key part of this node, stored right in the node to improve locality. */
  Value key;
  
  /** Value part of this node, stored right in the node to improve locality. */
  Value val;
  
  /** Pointer to the next node at the same element of this table. */
  MapPair *next;
};

/** Representation of a hash table implementation of a map. */
struct MapStruct {
  /** Table of key / value pairs. */
  MapPair **table;

  /** Length of the table. */
  int tlen;
  
  /** Number of key / value pairs in the map. */
  int size;
};

Map * makeMap( int len )
{

  struct MapStruct *map = malloc( sizeof( struct MapStruct ) );

  map->table = (MapPair **)malloc( len * sizeof(MapPair *) );

  map->tlen = len;
  map->size = 0;

  for ( int i = 0; i < len; i++ ) {
    map->table[i] = NULL;
  }

  return map;

}

int mapSize( Map *m )
{
  return m->size;
}

void mapSet(Map *m, Value *key, Value *val)
{

  unsigned int hash = key->hash( key );
  int index = hash % m->tlen;
  MapPair **bucket = &m->table[ index ];

  for ( MapPair *current = *bucket; current != NULL; current = current->next ) {
    if ( key->equals( &current->key, key ) ) {
      current->val.empty( &current->val );
      val->move (val, &current->val );
      return;
    }
  }

  MapPair *newPair = malloc( sizeof( MapPair ) );

  key->move( key, &newPair->key );
  val->move( val, &newPair->val );
  newPair->next = *bucket;
  *bucket = newPair;

  m->size++; // Increase the size of the map.
}

Value *mapGet(Map *m, Value *key)
{

  unsigned int hash = key->hash(key);
  int index = hash % m->tlen;
  MapPair *bucket = m->table[index];

  while (bucket != NULL) {

    if (key->equals(&bucket->key, key)) {

      return &bucket->val;
    }
    bucket = bucket->next;
  }

  return NULL;
}

void freeMap(Map *m)
{
  for (int i = 0; i < m->tlen; i++) {
    MapPair *current = m->table[i];
    while (current != NULL) {

      MapPair *next = current->next;

      current->key.empty(&current->key);
      current->val.empty(&current->val);

      free(current);
      current = next;
    }
  }

  free(m->table);

  free(m);
}

bool mapRemove(Map *m, Value *key)
{

  unsigned int hash = key->hash(key);
  int index = hash % m->tlen;
  MapPair **bucket = &m->table[index];

  while (*bucket != NULL) {
    if (key->equals(&(*bucket)->key, key)) {

      MapPair *toRemove = *bucket;
      *bucket = toRemove->next;

      toRemove->key.empty(&toRemove->key);
      toRemove->val.empty(&toRemove->val);


      free(toRemove);

      m->size--; // Decrement the size of the map.
      return true;
    }
    bucket = &(*bucket)->next;
  }
  return false;
}


