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
#include <string.h>
#include <search.h>

/**
 * @details
 * This structure is used to associate memory blocks with their respective owners (e.g., modules or components within the NeuroTIC system).
 * Each `mem_format` instance contains a pointer to the owner, a dynamic array of pointers to registered memory blocks, and a counter for the number of registered blocks.  
 * The `mem_tracker` is a dynamic array of `mem_format` structures that allows for efficient tracking and management of memory ownership and associated memory blocks throughout the lifecycle of the program.
 */
struct mem_format{
    void    *mem_owner;     /**< Pointer to the owner of the memory block. */
    void    **mem_register; /**< Internal registry for tracking allocated memory blocks. */
    size_t  mem_track;      /**< Counter for the number of tracked memory blocks. */
} *mem_tracker= NULL;


/**
 * @details
 * Increases as new memory owners are created and memory blocks are registered under those owners.
 * Decreases as memory owners are deleted and their associated memory blocks are freed.
 * This counter is essential for managing the dynamic array of `mem_format` structures, ensuring that memory ownership and tracking are accurately maintained throughout the lifecycle of the program.
 * 
 * @warning
 * This counter must be carefully managed to prevent memory leaks or dangling pointers.
 */
size_t mem_track= 0;

/**
 * @param key Pointer to the memory owner to match.
 * @param element Pointer to the memory format element being compared.
 * @retval 0 if the owner matches the element's memory owner, non-zero otherwise.
 * @retval 1 if the provided key is NULL, or if the element's memory owner is NULL.
 * 
 * @details
 * This function is used as a comparison function for searching the `mem_tracker` array to find the index of a specific memory owner.  
 * It compares the provided key (memory owner) with the `mem_owner` field of the memory format element.  
 * A return value of 0 indicates a match (i.e., the owner is found), while a non-zero return value indicates that the owner does not match the element being compared.  
 * This function is essential for managing memory ownership and ensuring that memory blocks are correctly associated with their respective owners in the tracking system.
 */
int match_owner( const void *key , const void *element){
    return *(void **)key != ((struct mem_format *)element)->mem_owner;
}

/**
 * @details
 * This function is used as a comparison function for searching the `mem_tracker` array to find the index of a specific memory register entry.  
 * It compares the provided key (memory register entry) with the `mem_register` field of the memory format element.  
 * A return value of 0 indicates a match (i.e., the register entry is found), while a non-zero return value indicates that the register entry does not match the element being compared.  
 * This function is essential for managing memory ownership and ensuring that memory blocks are correctly associated with their respective owners in the tracking system.
 */
int match_register( const void *key , const void *element){
    return *(void **)key != *(void **)element;
}

/**
 * @details
 * Iterates through the `mem_tracker` array and frees all memory blocks registered under each memory owner.  
 * After freeing the memory blocks, it also frees the memory register and resets the tracking information for each owner.  
 * Finally, it frees the entire `mem_tracker` array and resets the tracking counter.  
 * This function is registered with `atexit()` to ensure that all tracked memory is automatically freed when the program exits, preventing memory leaks and ensuring proper cleanup of resources.  
 * It is important to note that this function should not be called manually, as it is designed to be invoked automatically.
 * @warning
 * This function should not be called manually, as it is designed to be invoked automatically at program termination. Calling it manually may lead to unintended consequences, such as double freeing memory or leaving dangling pointers if the tracking system is still in use.
 * @attention
 * This function assumes that all memory blocks registered in the tracking system were allocated using `malloc()` or similar functions and can be safely freed.
 * @see
 * deleteowner() for deleting a specific memory owner and its associated memory blocks.
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
 * @param owner Pointer to the memory owner to delete.
 * @retval 0 Success, owner not found.
 * @retval 1 owner parameter is NULL.
 * @retval 2 register allocation failed
 * 
 * @details
 * This function deletes a specific memory owner and all associated memory blocks from the tracking system.  
 * It first checks if the provided owner pointer is valid, then searches for the owner in the `mem_tracker` array. If the owner is found, it frees all memory blocks registered under that owner, frees the memory register, and removes the owner from the tracking system.  
 * The function returns 0 if the deletion is successful, 1 if the owner is not found, and 2 if memory allocation fails during the process.
 * @see
 * If is the last register in mem_tracker, cleanmemory() is called.
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
    } else cleanmemory( );
    return 0;
}

/**
 * @retval NULL if the provided owner pointer is NULL, or if memory allocation fails during the creation of a new memory owner.
 * @retval Pointer to the created memory owner if successful, or a pointer to the existing owner if it already exists in the tracking system.
 * 
 * @details
 * This function creates a new memory owner and registers it in the tracking system.  
 * If the owner already exists, it returns a pointer to the existing owner.  
 * The function ensures that each memory owner is uniquely tracked within the system.
 * If the provided owner pointer is NULL, the function returns NULL without creating a new owner.
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
 * @retval void* Pointer to the registered memory block.
 * @retval NULL if the provided owner block pointer is NULL.
 * @retval owner pointer if mem is NULL
 * 
 * @details
 * Search  for te existence of the provided memory owner in the tracking system.
 * If the owner does not exists:
 * - Increases the size of regisgter table.
 * - Creates a new memory owner and registers it in the tracking system.
 * If the owner exists:
 * - Search for the existence of the provided memory block in the owner's register.
 * - If the memory block does not exist, it is added to the owner's register.
 * - If the memory block already exists, a pointer to the existing block is returned.
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