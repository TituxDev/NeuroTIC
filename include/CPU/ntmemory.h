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
 * @brief Deletes a memory block from the tracking registry and frees it.
 */
void cleanmemory( void );

/**
 * @brief Deletes a memory owner and all associated memory blocks.
 * 
 * @param owner Pointer to the memory owner to delete.
 * @return unsigned char 0 if successful, 1 if the owner is not found, 2 if memory allocation fails.
 */
unsigned char deleteowner( void *owner );

/**
 * @brief Creates a memory owner and registers it in the tracking system.
 * 
 * @param owner Pointer to the memory owner to create.
 * @return void* Pointer to the created memory owner, or NULL if creation fails.
 */
void *createowner( void *owner );

/**
 * @brief Creates a memory register entry for a specific memory owner.
 * 
 * @param owner Pointer to the memory owner.
 * @param mem Pointer to the memory block to register.
 * @return void* Pointer to the created register entry, or NULL if creation fails.
 */
void *createregister( void *owner, void *mem );

#endif // NTMEMORY_H