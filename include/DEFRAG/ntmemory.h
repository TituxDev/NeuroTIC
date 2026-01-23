/**
 * @file ntmemory.h
 * @brief Memory tracking utilities for the NeuroTIC framework.
 *
 * This module provides basic memory tracking and cleanup functions that help
 * manage dynamically allocated memory within the framework and user code.
 * 
 * The functions allow for automatic memory deallocation at program exit,
 * preventing leaks when using dynamically created structures.
 *
 * @see ntmemory_doc
 * @author Oscar Sotomayor
 * @date 2024
 */

#ifndef NTMEMORY_H
#define NTMEMORY_H


/**
 * @brief Frees all memory previously registered with memtrack().
 *
 * This function iterates through all tracked pointers, freeing each one.
 * It is automatically called at program termination via `atexit()` when
 * memory tracking is first invoked, but can also be called manually.
 */
void memfree(void);

/**
 * @brief Registers an allocated memory block for automatic cleanup.
 *
 * Tracks a pointer returned by `malloc`, `calloc`, or `realloc`. If the
 * pointer is NULL, the function prints an error message and terminates
 * the program to prevent undefined behavior. The first call to this
 * function automatically registers `memfree()` with `atexit()`.
 *
 * @param mem Pointer to dynamically allocated memory to track.
 * @warning Do not manually free or modify tracked memory.
 */
void memtrack(void *mem);

#endif // NTMEMORY_H