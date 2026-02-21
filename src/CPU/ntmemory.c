/**
 * @file ntmemory.c
 * @brief Implementation of memory management functions.
 * 
 * Provides functions for tracking dynamically allocated memory blocks and ensuring they are freed at program termination.  
 * Uses a dynamic registry to store pointers to allocated memory, and registers a cleanup function with `atexit()` to automatically free all tracked memory when the program exits.
 * 
 * @author Oscar Sotomayor.
 * @date 2026
 */

#include "ntmemory.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Internal registry for tracking allocated memory blocks.
 * 
 * This dynamic array stores pointers to all memory blocks that have been registered via ::memtrack().  
 * The registry allows for collective management of memory, enabling the ::memfree() function to iterate through all tracked pointers and free them at once.  
 * The registry is dynamically resized as new pointers are added, and is freed itself when ::memfree() is called.  
 * This approach simplifies memory management by centralizing the tracking and cleanup of dynamically allocated memory within the NeuroTIC system.  
 * 
 * @warning
 * This registry assumes exclusive ownership of all tracked pointers.  
 * Manually freeing any pointer that has been registered will lead to undefined behavior when ::memfree() is called, as it will attempt to free already freed memory.
 */
static void **mem_register= NULL;

/**
 * @brief Counter for the number of tracked memory blocks.
 * 
 * This counter keeps track of how many pointers have been registered in the `mem_register` array.  
 * It is incremented each time ::memtrack() is called and decremented as memory blocks are freed in ::memfree().  
 * The counter is used to manage the size of the registry and to ensure that all tracked memory is properly released when ::memfree() is invoked.
 * 
 * @warning
 * This counter must accurately reflect the number of valid pointers in the `mem_register`.  
 * Any discrepancies (e.g., due to manual freeing of tracked memory) can lead to undefined behavior when ::memfree() is called, as it may attempt to free invalid pointers or miss freeing valid ones.
 */
static unsigned int mem_count= 0;

/**
 * @details
 * Frees all memory blocks registered in the internal tracking registry.  
 * Iterates through the `mem_register` array, freeing each pointer and setting it to NULL to prevent dangling references.  
 * After all tracked memory has been freed, the registry itself is freed and set to NULL.  
 * This function is registered with `atexit()` to ensure that it is called automatically when the program terminates, providing a safety net for memory management and preventing leaks.
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
 * @details
 * Registers a dynamically allocated memory block for deferred release.  
 * Adds a pointer to the internal memory registry. The registry grows dynamically as new allocations are tracked.  
 * This function is intended to be called immediately after successful dynamic allocation (e.g. `malloc`, `calloc`, `realloc`).  
 * On the first invocation, ::memfree() is automatically registered to execute at program termination via `atexit()`.
 */
void *memtrack( void *mem ){
    if( !mem ){
        fprintf( stderr , "Failed to allocate memory for tracked object.\n" );
        exit( EXIT_FAILURE );
    }
    void **temp= realloc( mem_register, ++mem_count * sizeof( void * ) );
    if( !temp ){
        fprintf( stderr, "Failed to allocate memory for free tracking.\n" );
        exit( EXIT_FAILURE );
    }
    static char called= 1;
    if( called ){
        atexit( memfree );
        called= 0;
    }
    mem_register= temp;
    mem_register[mem_count - 1]= mem;
    return mem;
}
