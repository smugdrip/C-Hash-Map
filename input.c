/**
  @file input.c
  @author John Butterfield (jpbutte2)
  Helps with reading commands from the user.
*/

#include "input.h"
#define INIT_CAP_LINE 10

char *readLine( FILE *fp )
{
  int capacity = INIT_CAP_LINE;
  char *buffer = ( char * ) malloc( capacity );
  int len = 0;
  char ch;

  while ( ( ch = fgetc( fp ) ) != '\n' && ( ch != EOF ) ) {
    if ( len == capacity - 1 ) {
      capacity = capacity + INIT_CAP_LINE;
      buffer = ( char * ) realloc( buffer, capacity );
    }
    buffer[len] = ( char )ch;
    len++; 
  }
  buffer[len] = '\0';

  if ( len == 0 ) {
    free( buffer );
    return NULL;
  }
    
  return buffer;

}
