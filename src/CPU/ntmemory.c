/**
 * @file ntmemory.c
 * @brief Memory tracking and cleanup system for NeuroTIC.
 *
 * Provides a lightweight mechanism to track dynamically allocated memory
 * and ensure proper release upon program termination. This prevents leaks
 * during neural network creation and teardown.
 *
 * The module maintains an internal registry of allocated pointers and
 * automatically frees them either manually through ::memfree() or
 * automatically via `atexit()`.
 *
 * @note This system does not manage individual deallocations — all
 * registered pointers are released together.
 * @author Oscar Sotomayor
 * @date 2024
 */

#include "ntmemory.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * @var mem_register
 * @brief Global dynamic register of tracked memory blocks.
 *
 * This array stores pointers to all dynamically allocated memory regions
 * that have been registered through ::memtrack().
 */
static void **mem_register= NULL;

/**
 * @var mem_count
 * @brief Current number of tracked memory blocks.
 *
 * Indicates how many pointers have been registered. This counter decrements
 * as ::memfree() iterates and releases each stored block.
 */
static unsigned int mem_count= 0;

/**
 * @fn void memfree( void )
 * @brief Frees all tracked memory blocks and clears the registry.
 *
 * This function iterates over the internal pointer register, releasing
 * each allocated block. Once all tracked memory has been freed, the
 * registry itself is also released and reset to NULL.
 *
 * Typically, this is called automatically at program termination via
 * `atexit()`. It can also be called manually to force cleanup earlier
 * in execution.
 *
 * @warning This function does not validate whether a pointer has already
 * been manually freed. Double freeing externally managed pointers
 * will result in undefined behavior.
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
 * @fn void memtrack( void *mem )
 * @brief Tracks a newly allocated memory block for future cleanup.
 *
 * Registers a pointer into the internal tracking registry. The registry
 * expands dynamically via `realloc()` as new allocations are tracked.
 *
 * On the first successful registration, the function automatically sets up
 * ::memfree() to be executed at program termination through `atexit()`.
 *
 * If allocation fails or a NULL pointer is passed, the program terminates
 * immediately with an error message printed to `stderr`.
 *
 * @param mem Pointer to a valid dynamically allocated memory block.
 * @warning Passing a NULL pointer will trigger program termination.
 */
void memtrack( void *mem ){
    if( !mem ){
        fprintf( stderr , "Failed to allocate memory for tracked object.\n" );
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
