/**
    @file input.h
    @author John Butterfield (jpbutte2)
    Header file for input.c, which helps with reading input from the 
    park files and it will also be used to read commands from the user.
*/

#include <stdio.h>
#include <stdlib.h>

/** Initial string capacity */
#define INIT_CAP_LINE 10

/**
    Reads a single line of input from the given input stream 
    (stdin or a file) and returns it as a string inside a block 
    of dynamically allocated memory. Some other code will be 
    responsible for eventually freeing that memory (to avoid a memory leak).
    @param fp The given input stream to read from
    @return A pointer to dynamically allocated memory, containing a 
            line of text. If there’s no more input to read, this 
            function should return NULL.
*/
char *readLine( FILE *fp );
