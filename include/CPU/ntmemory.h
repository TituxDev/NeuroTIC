/**
 * @file ntmemory.h
 * @ingroup NTMemory
 * @brief Auxiliary memory tracking utilities.
 * @author Oscar Sotomayor (TituxDev).
 *
 * Declares a minimal pointer tracking interface used to register
 * dynamically allocated memory blocks for deferred collective release.
 *
 * This interface is primarily intended for internal use during
 * development and experimentation.
 *
 * The module does not define ownership semantics and may be replaced
 * by a more complete solution as the framework evolves.
 *
 */

#ifndef NTMEMORY_H
#define NTMEMORY_H



/**
 * @ingroup NTMemory
 * @brief Releases all memory blocks previously registered.
 *
 * Frees every pointer stored in the internal tracking registry and clears
 * the registry itself.
 *
 * This function is automatically registered with `atexit()` upon the first
 * successful call to ::memtrack(), but may also be invoked explicitly.
 *
 * @warning
 * This function assumes exclusive ownership of all tracked pointers.
 * Manually freeing tracked memory before calling ::memfree() results in
 * undefined behavior.
 */
void memfree( void );

/**
 * @ingroup NTMemory
 * @brief Registers a dynamically allocated memory block for deferred release.
 *
 * Adds a pointer to the internal memory registry. The registry grows dynamically
 * as new allocations are tracked.
 *
 * This function is intended to be called immediately after successful dynamic
 * allocation (e.g. `malloc`, `calloc`, `realloc`).
 *
 * On the first invocation, ::memfree() is automatically registered to execute
 * at program termination via `atexit()`.
 *
 * @param mem Pointer to a valid dynamically allocated memory block.
 *
 * @warning
 * Passing a NULL pointer is treated as a fatal error and terminates execution.
 * Tracked memory must not be manually freed.
 */
void memtrack( void *mem );

#endif // NTMEMORY_H