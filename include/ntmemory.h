/**
 * ntmemory.h - Memory tracking interface for NeuroTIC
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * Declares functions for registering and freeing dynamically allocated memory
 * in the NeuroTIC system. Memory tracked through these functions will be
 * automatically freed when the program terminates.
 *
 * The `memfree()` function is also exposed for discretionary use by the user.
 * This allows manual cleanup of all tracked memory, which can be useful for:
 *  - Deallocating a complete neural network created via NeuroTIC functions.
 *  - Integration with system calls or signal handlers to clean resources
 *    on abnormal program termination (e.g., SIGINT, SIGTERM).
 *
 * This module simplifies memory management by maintaining a registry of all
 * allocated blocks and ensuring their cleanup.
 */

#ifndef NTMEMORY_H
#define NTMEMORY_H

/**
 * @brief Frees all memory blocks registered via memtrack().
 *
 * This function is automatically registered with `atexit()` and
 * called upon program exit to release all tracked allocations,
 * preventing memory leaks.
 *
 * It can also be called manually if immediate cleanup is desired,
 * such as when destroying a complete network or handling signals.
 */
void memfree(void);

/**
 * @brief Registers a pointer to dynamically allocated memory for tracking.
 *
 * Adds the pointer to an internal registry so it can be freed
 * automatically at program termination.
 *
 * @param mem Pointer to allocated memory to track.
 */
void memtrack(void *mem);

#endif // NTMEMORY_H

/*
================================================================================
1. USAGE EXAMPLES
================================================================================

Example: Basic memory allocation and tracking
---------------------------------------------

#include <stdlib.h>
#include "ntmemory.h"

void example() {
    int *array = malloc(10 * sizeof(int));
    if (!array) {
        // Handle allocation failure
        return;
    }

    // Register the allocated memory to ensure it is freed on program exit
    memtrack(array);

    // Use the array normally...

    // No need to call free(array) manually; memfree() will handle cleanup.
}

================================================================================
2. NOTES
================================================================================

- The memfree() function is automatically called once on program exit,
  freeing all tracked memory.

- memfree() is also available for manual invocation, enabling:
    * Complete deallocation of neural networks created with NeuroTIC.
    * Resource cleanup in response to signals or abnormal termination.

- Avoid manually freeing pointers registered with memtrack() to prevent
  double free errors.

- If memtrack() fails to allocate internal tracking memory, the program
  will terminate immediately to avoid inconsistent state.

*/
