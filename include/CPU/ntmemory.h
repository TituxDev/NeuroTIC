/**
 * @file ntmemory.h
 * @brief Memory Management Functions  
 * @ref http://tituxdev.github.io/NeuroTIC/src/CPU/ntmemory.c
 * 
 * @copydetails ntmemory.c
 */

#ifndef NTMEMORY_H
#define NTMEMORY_H



/**
 * @brief Frees all memory blocks registered in the internal tracking registry.
 */
void memfree( void );

/**
 * @brief Registers a memory block for tracking and automatic cleanup.
 * 
 * @param mem Pointer to the memory block to track.
 */
void *memtrack( void *mem );

#endif // NTMEMORY_H