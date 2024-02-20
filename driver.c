/**
  @file driver.c
  @author John Butterfield (jpbutte2)
  Top level component of the Hash Map
  program, responsible for parsing user input and
  driving the hash map.
*/

#include "input.h"
#include "map.h"
#include <string.h>
#include <ctype.h>

#define MAP_SIZE 100
#define CMD_LEN 50

/**
  This function parses user input into different strings for use
  by the hash map
  @param cmd the line of user input
  @param c1 first token from input
  @param c2 second token from input
  @param c3 third token from input
  @param c4 fourth token from input
*/
static void parseCommand(const char* cmd, char* c1, char* c2, char* c3, char* c4);

int main()
{

  struct MapStruct * map = makeMap( MAP_SIZE );
  printf( "cmd> " );
  char *cmd;
  while ( ( cmd = readLine ( stdin ) ) != NULL )
  {

    char c1[ CMD_LEN ] = "";
    char c2[ CMD_LEN ] = "";
    char c3[ CMD_LEN ] = "";
    char c4[ CMD_LEN ] = "";

    parseCommand( cmd, c1, c2, c3, c4 );
    printf( "%s", cmd );

    free( cmd );

    if ( strcmp( c1, "set" ) == 0 ) {
      
      if ( strlen ( c2 ) == 0 || strlen( c4 ) > 0 ) {
        printf( "\nInvalid commmand" );
      } else if ( !isdigit(c2[0]) && c2[0] != '\"' && c2[0] != '-' ) {
        printf( "\nInvalid commmand" );
      } else if ( !isdigit(c3[0]) && c3[0] != '\"' && c3[0] != '-' ) {
        printf( "\nInvalid commmand" );
      } else {

        Value *key = ( Value * )malloc( sizeof( Value ) );
        if ( parseInteger( key, c2 ) == 0 ) {
          parseString( key, c2 );
        }
        Value *val = ( Value * )malloc( sizeof( Value ) );
        if ( parseInteger( val, c3 ) == 0 ) {
          parseString( val, c3 );
        }
        mapSet( map, key, val );

        key->empty( key );
        free( key );

        val->empty( val );
        free( val );

      }


    } else if ( strcmp( c1, "get" ) == 0 ) {

      if ( strlen ( c2 ) == 0 || strlen( c3 ) > 0 ) {
        printf( "\nInvalid commmand" );
      } else if ( !isdigit(c2[0]) && c2[0] != '\"' && c2[0] != '-' ) {
        printf( "\nInvalid commmand" );
      } else {

        Value *key = ( Value * )malloc( sizeof( Value ) );
        if ( parseInteger( key, c2 ) == 0 ) {
          parseString( key, c2 );
        }
        
        Value *val = mapGet( map, key );
        if ( val != NULL ) {
          printf( "\n" );
          val->print( val );
        } else {
          printf( "\nUndefined" );
        }
        key->empty( key );
        free( key );

      }

    } else if ( strcmp( c1, "remove" ) == 0 ) {
      
      if ( strlen ( c2 ) == 0 || strlen( c3 ) > 0 ) {
        printf( "\nInvalid commmand" );
      } else if ( !isdigit( c2[ 0 ] ) && c2[ 0 ] != '\"' && c2[0] != '-' ) {
        printf( "\nInvalid commmand" );
      } else {
          
        Value *key = ( Value * )malloc( sizeof( Value ) );
        if ( parseInteger( key, c2 ) == 0 ) {
          parseString( key, c2 );
        }
        if ( !mapRemove( map, key ) ) {
          printf( "\nNot in map" );
        }
        key->empty( key );
        free( key );

      }
      

    } else if ( strcmp( c1, "size" ) == 0 ) {
      
      if ( strlen ( c2 ) > 0 ) {
        printf( "\nInvalid commmand" );
      } else {
        printf( "\n%d", mapSize( map ) );
      }

    } else if ( strcmp( c1, "quit") == 0 ) {
      if ( strlen ( c2 ) > 0 ) {
        printf( "\nInvalid commmand" );
      } else {
        printf("\n");
        freeMap( map );
        exit( 0 );
      }
    }
    printf( "\n\ncmd> " );
  }
  freeMap( map );
  return 0;
}

static void parseCommand( const char* cmd, char* c1, char* c2, char* c3, char* c4 )
{
  const char *start, *end;
  start = cmd;
  int tokenIndex = 0;

  while ( *start ) {
        
    while ( *start == ' ' ) {
      start++;
    }

    bool inQuotes = (*start == '\"');
    end = inQuotes ? start + 1 : start;

    if (inQuotes) {
      while ( *end && !( *end == '\"' && *( end - 1 ) != '\\' ) ) { 
        end++;
      }
      if ( *end == '\"' )  {
        end++;
      }
    } else {
      while ( *end && *end != ' ' ) {
        end++;
      }
    }

    char * target;
    switch ( tokenIndex ) {
      case 0: target = c1; break;
      case 1: target = c2; break;
      case 2: target = c3; break;
      default: target = c4; break;
    }

    if ( end - start >= 1 ) {
      strncpy( target, start, end - start );
      target[ end - start ] = '\0';
    }

    tokenIndex++;
    start = end;
  }

  if (tokenIndex <= 3) *c4 = '\0';
  if (tokenIndex <= 2) *c3 = '\0';
  if (tokenIndex <= 1) *c2 = '\0';

}
