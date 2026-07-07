/**
 * @file ntmemory.h
 * @copybrief ntmemory.c
 * 
 * @ref http://tituxdev.github.io/NeuroTIC/src/CPU/ntmemory.c
 * 
 * @copydetails ntmemory.c
 */

#ifndef NTMEMORY_H
#define NTMEMORY_H

/**
 * @brief Frees every tracked memory block, for every owner, and empties
 *        the tracking registry.
 */
void cleanmemory( void );

/**
 * @brief Deletes a memory owner and all associated memory blocks.
 * 
 * @param owner Pointer to the memory owner to delete.
 * @return 0 on success -- the owner was deleted, or was not tracked to
 *         begin with.
 */
unsigned char deleteowner( void *owner );

/**
 * @brief Creates a memory owner and registers it in the tracking system.
 * 
 * @param owner Pointer to the memory owner to create.
 * @return Pointer to the owner's tracking entry -- newly created, or the
 *         existing one if `owner` was already tracked.
 */
void *createowner( void *owner );

/**
 * @brief Creates a memory register entry for a specific memory owner.
 * @param owner Pointer to the memory owner under which the memory block is registered.
 * @param mem Pointer to the memory block to register.
 * @return Pointer to the now-registered memory block.
 */
void *createregister( void *owner , void *mem );

#endif // NTMEMORY_H