/**
 * @defgroup NTMemory
 * @ingroup NTPeripherals
 * @file ntmemory.c
 * @ingroup NTMemory
 * @brief Pointer registry and deferred cleanup implementation.
 * @author Oscar Sotomayor (TituxDev).
 *
 * Implements a simple global pointer registry used during development
 * to track dynamically allocated memory blocks and release them
 * collectively.
 *
 * The registry provides a coarse-grained cleanup mechanism intended
 * for small experiments and early framework development.
 *
 * This module is auxiliary in nature and may be replaced by a more
 * capable memory subsystem in the future.
 *
 * @see ntmemory.h
 */

#include "ntmemory.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * @var mem_register
 * @brief Dynamic registry of tracked memory blocks.
 *
 * Stores pointers to all memory blocks registered through ::memtrack().
 * The registry itself is dynamically resized as new entries are added.
 */
static void **mem_register= NULL;

/**
 * @var mem_count
 * @brief Number of currently tracked memory blocks.
 *
 * Indicates how many pointers are stored in ::mem_register.
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
 * @brief Registers a memory block for deferred collective release.
 *
 * Appends a pointer to the internal tracking registry. The registry grows
 * dynamically via `realloc()` as new blocks are registered.
 *
 * On the first successful registration, ::memfree() is automatically
 * registered with `atexit()` to ensure cleanup at program termination.
 *
 * @param mem Pointer to a valid dynamically allocated memory block.
 *
 * @warning
 * Passing a NULL pointer is considered a fatal error and terminates execution.
 * Tracked memory must not be manually freed.
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
