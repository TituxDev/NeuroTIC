/**
 * ntmemory.c - Memory management and tracking for NeuroTIC
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * This module provides basic memory allocation tracking to ensure that all
 * dynamically allocated memory in the NeuroTIC system is properly freed at
 * program exit. It maintains a registry of allocated pointers and frees them
 * automatically on program termination.
 *
 * The system uses a global dynamic array to store pointers to all allocated
 * blocks, providing simple but effective memory cleanup.
 */

#include "ntmemory.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Dynamic array storing pointers to allocated memory blocks.
 *
 * Used for tracking all allocations so they can be freed on exit.
 */
static void **mem_register= NULL;

/**
 * @brief Number of currently tracked memory blocks.
 */
static unsigned int mem_count= 0;

/**
 * @brief Frees all tracked memory blocks and clears the registry.
 *
 * Called automatically on program exit to prevent memory leaks.
 * Iterates through the `mem_register` array in reverse order, freeing
 * each allocated block and setting its pointer to NULL, then frees
 * the registry itself.
 */
void memfree( void ){
    while( mem_count )
        if( mem_register[--mem_count] ){
            free( mem_register[mem_count] );
            mem_register[mem_count]= NULL;
        }
    free( mem_register );
    mem_register= NULL;
}

/**
 * @brief Registers a newly allocated memory block for tracking.
 *
 * Adds the pointer `mem` to the `mem_register` array, reallocating
 * the array to hold the new pointer. If allocation fails at any point,
 * the program terminates with an error message.
 *
 * On the first call, registers `memfree` to be called automatically
 * at program exit using `atexit()`.
 *
 * @param mem Pointer to newly allocated memory to be tracked.
 */
void memtrack( void *mem ){
    if( !mem ){
        fprintf( stderr , "Failed to allocate memory for neurons in define_net.\n" );
        exit( EXIT_FAILURE );
    }
    void **temp= realloc( mem_register, ++mem_count * sizeof( void * ) );
    if( !temp ){
        fprintf( stderr, "Failed to allocate memory for free tracking.\n" );
        exit( EXIT_FAILURE );
    }
    mem_register= temp;
    mem_register[mem_count - 1]= mem;
    static char called= 1;
    if( called ){
        atexit( memfree );
        called= 0;
    }
}
