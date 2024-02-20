/**
  @file value.c
  @author John Butterfield (jpbutte2)
  Implementation for the value component, with support for integer
  and (eventually) string values.
*/

#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

//////////////////////////////////////////////////////////
// Integer implementation.

// print method for Integer.
static void printInteger( Value const *v )
{
  // Print the integer inside this value.
  printf( "%d", v->ival );
}

// move method for Integer.
static void moveInteger( Value const *src, Value *dest )
{
  // We just need to copy the value.
  dest->ival = src->ival;
  
  dest->print = src->print;
  dest->move = src->move;
  dest->equals = src->equals;
  dest->hash = src->hash;
  dest->empty = src->empty;
}

// equals method for Integer.
static bool equalsInteger( Value const *v, Value const *other )
{

  // Make sure the other object is also an Integer.
  // (i.e., it uses the same print funtion)
  if ( other->print != printInteger )
    return false;

  return v->ival == other->ival;
}

// hash method for Integer.
static unsigned int hashInteger( Value const *v )
{
  // This will overflow negative values to large positive ones.
  return v->ival;
}

// Free memory used inside this integer Value.
static void emptyInteger( Value *v )
{
  // An int vaue doesn't need any additional memory.
}

int parseInteger( Value *v, char const *str )
{
  // Try to parse an integer from str.
  int val, len;
  if ( sscanf( str, "%d%n", &val, &len ) != 1 )
    return 0;

  // Fill in all the fields of v for an integer type of value.
  v->print = printInteger;
  v->move = moveInteger;
  v->equals = equalsInteger;
  v->hash = hashInteger;
  v->empty = emptyInteger;
  v->ival = val;

  // Return how much of str we parsed.
  return len;
}

//////////////////////////////////////////////////////////
// String implementation.

// print method for String.
static void printString( Value const *v )
{
  // Print the string inside this value.
  printf( "\"%s\"", (char *)v->vptr );
}

// move method for Integer.
static void moveString( Value const *src, Value *dest )
{
  // Allocate new memory for the destination string and copy the contents.
  char *srcStr = (char *)src->vptr;
    
  dest->vptr = malloc( strlen( srcStr)  + 1 );
  strcpy( (char *)dest->vptr, srcStr );

  // Copy other function pointers.
  dest->print = src->print;
  dest->move = src->move;
  dest->equals = src->equals;
  dest->hash = src->hash;
  dest->empty = src->empty;
}


// equals method for String.
static bool equalsString( Value const *v, Value const *other )
{
  // Make sure the other object is also a String.
  // (i.e., it uses the same print funtion)
  if ( other->print != printString )
    return false;

  if ( strcmp( (char *)v->vptr, (char *)other->vptr ) == 0 ) {
    return true;
  } else {
    return false;
  }
}

// Hash method for String using the Jenkins one-at-a-time hash algorithm.
static unsigned int hashString(Value const *v)
{

  const uint8_t* key = ( const uint8_t* ) v->vptr;
  size_t length = strlen( (char *) key );

  // Jenkins one-at-a-time hash calculation
  size_t i = 0;
  uint32_t hash = 0;
  while ( i != length ) {
    hash += key[ i++ ];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;

  return hash;
}

// Free memory used inside this string Value.
static void emptyString( Value *v )
{
  free( v->vptr );
  v->vptr = NULL;
}

int parseString( Value *v, char const *str )
{

  bool inQuotes = false;
  int len = 0;
  int quoteIndex = 0;
  int quotes = 0;
  int c = 0;
  while ( 1 )
  {
    if ( str[ len ] == '\"' ) {
      quotes++;
      inQuotes = !inQuotes;
      if ( quotes == 1 ) {
        quoteIndex = len + 1;
      }
    }
    if ( inQuotes ) {
      c++;
    }
    len++;
    if ( quotes == 2 ) {
      break;
    }
    
  }

  // Fill in all the fields of v for a string type of value.
  v->print = printString;
  v->move = moveString;
  v->equals = equalsString;
  v->hash = hashString;
  v->empty = emptyString;

  // Allocate memory and copy the string.
  v->vptr = ( char * )malloc( c );
  strncpy( ( char * )v->vptr, str + quoteIndex, c - 1 );
  ( ( char * )v->vptr )[ c - 1 ] = '\0';

  // Return the length of the string parsed.
  return len;
}
