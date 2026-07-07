/**
 * @file ntmemory.c
 * @brief Implementation of memory management functions.
 * 
 * Provides functions for tracking dynamically allocated memory blocks and ensuring they are freed at program termination.  
 * Uses a dynamic registry to store pointers to allocated memory, and registers a cleanup function with `atexit()` to automatically free all tracked memory when the program exits.
 * 
 * @author Oscar Sotomayor
 * @date 2026
 */

#include "ntmemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>

/**
 * @details
 * Associates the set of memory blocks tracked under `mem_register` with
 * the owner that requested them (typically a net_s instance), so every
 * block belonging to that owner can be located and freed together.
 *
 * `mem_tracker` is the array of every currently tracked owner -- `NULL`
 * exactly when the global @ref mem_track "mem_track" count is zero.
 */
struct mem_format{
    void    *mem_owner;     /**< Pointer to the owner of the memory block. */
    void    **mem_register; /**< Internal registry for tracking allocated memory blocks. */
    size_t  mem_track;      /**< Counter for the number of tracked memory blocks. */
} *mem_tracker= NULL;


/**
 * @details
 * The number of currently tracked owners -- not to be confused with the
 * per-owner `mem_format::mem_track` field, which counts memory blocks
 * registered under a single owner. Increases by one each time
 * createowner() tracks a new owner; decreases by one each time
 * deleteowner() removes one.
 */
size_t mem_track= 0;


/**
 * @param key Pointer to the memory owner to match.
 * @param element Pointer to the mem_format element being compared.
 * @retval 0 `*key` equals `element`'s `mem_owner` (match found).
 * @retval 1 `*key` differs from `element`'s `mem_owner` -- this includes
 *           the case where either (or both) is NULL, since NULL is
 *           treated as an ordinary pointer value here.
 *
 * @details
 * Comparison callback for `lfind()`, used to locate a specific owner
 * within the `mem_tracker` array. Compares `*key` against the `mem_owner`
 * field of each element via plain pointer equality.
 */
int match_owner( const void *key , const void *element){
    return *(void **)key != ((struct mem_format *)element)->mem_owner;
}

/**
 * @param key Pointer to the memory block pointer to match.
 * @param element Pointer to the registered block being compared.
 * @retval 0 `*key` equals `*element` (match found).
 * @retval 1 `*key` differs from `*element`.
 *
 * @details
 * Comparison callback for `lfind()`, used to locate a specific memory
 * block within one owner's `mem_register` array. Unlike match_owner(),
 * both `key` and `element` here are treated as a single registered
 * `void *` block -- no `mem_format` element is involved.
 */
int match_register( const void *key , const void *element){
    return *(void **)key != *(void **)element;
}

/**
 * @details
 * Frees every memory block registered under every tracked owner, then
 * frees each owner's register and the `mem_tracker` array itself,
 * resetting `mem_track` to zero.
 *
 * @warning
 * Not meant to be called manually -- it is registered with `atexit()` (by
 * createowner(), the first time it runs) so that all tracked memory is
 * freed automatically when the program exits. Calling it while the
 * tracking system is still in use may cause double frees or dangling
 * pointers.
 * @attention
 * Assumes every registered block was allocated with `malloc()` or a
 * compatible allocator (e.g. `calloc()`), and can be safely freed.
 * @see
 * deleteowner() to remove a single owner instead of everything.
 */
void cleanmemory( void ){
    if( mem_tracker ){
        while( mem_track ){
            --mem_track;
            if( mem_tracker[mem_track].mem_register ){
                for( size_t i= 0 ; i < mem_tracker[mem_track].mem_track ; i++ ) if( mem_tracker[mem_track].mem_register[i] ){
                    free( mem_tracker[mem_track].mem_register[i] );
                    mem_tracker[mem_track].mem_register[i]= NULL;
                }
                memset( mem_tracker[mem_track].mem_register , 0 , mem_tracker[mem_track].mem_track * sizeof( void * ) );
                free( mem_tracker[mem_track].mem_register );
                mem_tracker[mem_track].mem_register= NULL;
            }
            if( mem_tracker[mem_track].mem_owner ) mem_tracker[mem_track].mem_owner= NULL;
        }
        free( mem_tracker );
        mem_tracker= NULL;
    } else mem_track= 0;
}

/**
 * @retval 1 `owner` is NULL.
 * @retval 2 the compacted tracker array could not be allocated (only
 *           reachable when 2 or more owners are tracked).
 *
 * @details
 * Deletes `owner` and every memory block registered under it.
 *
 * When two or more owners are tracked, searches for `owner` and, if
 * found, frees its registered blocks and compacts the remaining owners
 * into a freshly allocated array.
 *
 * When at most one owner is tracked, deleting the sole remaining owner
 * (if `owner` matches it) is equivalent to clearing the entire tracking
 * system, so cleanmemory() is called directly instead of shrinking an
 * array down to zero elements.
 *
 * If `owner` is not currently tracked, nothing is deleted.
 */
unsigned char deleteowner( void *owner ){
    if( !( owner ) ) return 1;
    if( (mem_track - !!mem_track) && mem_tracker ){
        struct mem_format *owner_index= (struct mem_format *)lfind( &owner , mem_tracker , &mem_track , sizeof( struct  mem_format ) , match_owner );
        if( owner_index ){
            if( owner_index->mem_register ){
                for( size_t i= 0 ; i < owner_index->mem_track ; i++ ) if( owner_index->mem_register[i] ){
                    free( owner_index->mem_register[i] );
                    owner_index->mem_register[i]= NULL;
                }
                memset( owner_index->mem_register , 0 , owner_index->mem_track * sizeof( void * ) );
                free( owner_index->mem_register );
            }
            struct mem_format *tmp= calloc( mem_track - 1 , sizeof( struct mem_format ) );
            if( !tmp ) return 2;
            else --mem_track;
            memmove( owner_index , owner_index + (owner_index != &mem_tracker[mem_track]) , ((&mem_tracker[mem_track] - owner_index) + (owner_index == &mem_tracker[mem_track])) * sizeof( struct mem_format ) );
            memcpy( tmp , mem_tracker , mem_track * sizeof( struct mem_format ) );
            memset( mem_tracker , 0 , mem_track * sizeof( struct mem_format ) );
            free( mem_tracker );
            mem_tracker= tmp;
            tmp= NULL;
        }
    } else if( mem_tracker && lfind( &owner , mem_tracker , &mem_track , sizeof( struct mem_format ) , match_owner ) ) cleanmemory( );
    return 0;
}

/**
 * @retval NULL `owner` is NULL, or the tracker array could not be grown.
 *
 * @details
 * Finds `owner` in the tracking system, or creates it if not already
 * present. The first time this function runs, it also registers
 * cleanmemory() with `atexit()`.
 */
void *createowner( void *owner ){
    if( !owner ) return owner;
    static char called= 1;
    if( called ){
        atexit( cleanmemory );
        called= 0;
    }
    struct mem_format *element_index= (struct mem_format *)lfind( &owner , mem_tracker , &mem_track , sizeof( struct mem_format ) , match_owner );
    if( !element_index ){
        struct mem_format *tmp= (struct mem_format *)calloc( mem_track + 1 , sizeof( struct mem_format ) );
        if( !tmp ) return NULL;
        memcpy( tmp , mem_tracker , mem_track * sizeof( struct mem_format ) );
        memset( mem_tracker , 0 , mem_track * sizeof( struct mem_format ) );
        free( mem_tracker );
        mem_tracker= tmp;
        tmp= NULL;
        element_index= &mem_tracker[mem_track++];
        element_index->mem_owner= owner;
    }
    return element_index;
}

/**
 * @retval NULL
 *  - `mem` is NULL.
 *  - `owner` is non-NULL but the internal owner or register table could
 *    not be grown (allocation failure).
 *
 * @note
 * If `owner` is NULL and `mem` is non-NULL, `mem` itself is returned
 * unchanged -- but it is neither registered nor freed in that case; the
 * caller remains responsible for it.
 *
 * @details
 * Ensures `owner` is tracked (creating it via createowner() if needed),
 * then registers `mem` under that owner if it is not already present,
 * growing the owner's block registry by one slot. If `mem` was already
 * registered under this owner, it is returned unchanged without growing
 * the registry.
 */
void *createregister( void *owner, void *mem ){
    if( !( owner && mem ) ) return owner ? mem : owner;
    struct mem_format *element_index= createowner( owner );
    if( !element_index ) return element_index;
    void **register_index= (void **)lfind( &mem , element_index->mem_register , &element_index->mem_track , sizeof( void * ) , match_register );
    if( !register_index ){
        void *tmp= calloc( element_index->mem_track + 1 , sizeof( void * ) );
        if( !tmp ) return tmp;
        memcpy( tmp , element_index->mem_register , element_index->mem_track * sizeof( void * ) );
        memset( element_index->mem_register , 0 , element_index->mem_track * sizeof( void * ) );
        free( element_index->mem_register );
        element_index->mem_register= tmp;
        tmp= NULL;
        register_index= &element_index->mem_register[element_index->mem_track++];
        *register_index= mem;
    }
    return *register_index;
}