/**
 * @note This file is part of Neuroticore, a neural network library.
 * @author Oscar Sotomayor
 * @date 2025
 * @version 1.0
 * @license Mozilla Public License v. 2.0 (https://mozilla.org/MPL/2.0/)
 * @file creation.c
 * @brief Implementation of network creation functions for the NeuroTIC neural network framework.
 * @details
 * This file provides functions to define and build neural networks, including memory management
 * for neurons, inputs, and weights. It allows for dynamic configuration of network topology
 * and is designed to work with the `net` and `neuron` structures defined in
 * `neuroticore.h`. The functions handle memory allocation, initialization, and linking of
 * neurons, inputs, and outputs, enabling the creation of flexible and efficient neural networks.
 * The implementation is modular, allowing for easy extension with additional features or
 * configurations in the future.
 */

#include "creation.h"

#include "neuroticore.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @var mem_register
 * @brief Global array to track allocated memory for neurons and other structures.
 * This array is used to ensure that all allocated memory can be freed
 * at program termination, preventing memory leaks.
 * @note The memory tracking is done using a static array that grows dynamically
 * as new memory is allocated. The `memtrack` function is called to register
 * each allocation, and `memfree` is registered with `atexit` to ensure it
 * is called when the program exits.
 * @warning Ensure that all allocated memory is registered with `memtrack`
 * to avoid memory leaks. The `memfree` function will free all tracked memory
 * when the program terminates.
 * @details
 * The `mem_register` array is initialized to NULL and grows as needed.
 * The `mem_count` variable keeps track of the number of allocations.
 * The `memfree` function iterates through the `mem_register` array and frees
 * each allocated memory block. It is registered to be called at program exit
 * using `atexit`, ensuring that all memory is properly released when the program
 * terminates.
 * @note This memory tracking mechanism is useful for debugging and ensuring
 * that all dynamically allocated memory is properly managed throughout the
 * lifecycle of the program. It is particularly important in complex applications
 * where multiple layers of memory allocation may occur, such as in neural network
 * implementations where neurons, inputs, and weights are dynamically allocated.
 * @warning
 * If the program exits without freeing all allocated memory, it may lead to memory leaks.
 * Ensure that all memory allocations are tracked using `memtrack` and that
 * `memfree` is called at the end of the program to release all resources.
 * @note
 * This implementation assumes that the program will not run indefinitely and will
 * eventually terminate, at which point all tracked memory will be freed.
 * This is suitable for most applications, but if the program is expected
 * to run for an extended period or indefinitely, consider implementing
 * additional memory management strategies to periodically free unused memory
 * or to handle dynamic resizing of the `mem_register` array.
 * @warning
 * If the program is designed to run indefinitely, consider implementing
 * additional memory management strategies to periodically free unused memory
 * or to handle dynamic resizing of the `mem_register` array.
 * This implementation is primarily designed for applications where the program
 * will eventually terminate, allowing all tracked memory to be freed at that time.
 * If the program is expected to run for an extended period or indefinitely,
 * it may be necessary to implement additional memory management strategies
 * to periodically free unused memory or to handle dynamic resizing of the `mem_register` array.
 * This implementation is primarily designed for applications where the program
 * will eventually terminate, allowing all tracked memory to be freed at that time.
 * If the program is expected to run for an extended period or indefinitely,
 */
static void **mem_register= NULL;

/**
 * @var mem_count
 * @brief Counter for the number of allocated memory blocks.
 * This variable keeps track of how many memory blocks have been allocated
 * and registered in the `mem_register` array. It is used to iterate through
 * the `mem_register` array when freeing memory at program termination.
 * @note The `mem_count` variable is incremented each time a new memory block
 * is registered with `memtrack`, and it is decremented when memory is freed.
 * It ensures that all allocated memory can be properly released when the program exits.
 */
static unsigned int mem_count= 0;

/**
 * @brief Frees all memory tracked by `mem_register`.
 * This function is registered with `atexit` to ensure it is called
 * when the program terminates, releasing all dynamically allocated memory.
 * It iterates through the `mem_register` array and frees each allocated block.
 * @note Ensure that all memory allocations are registered with `memtrack`
 * to avoid memory leaks. This function will free all tracked memory
 * when the program exits.
 */
static void memfree( void ){
    while( mem_count ) if( mem_register[--mem_count] ){
        free( mem_register[mem_count] );
        mem_register[mem_count]= NULL; // Clear the pointer after freeing
    }
    free( mem_register );
    mem_register= NULL; // Clear the register after freeing
}

/**
 * @fn memtrack
 * @brief Registers a memory allocation for tracking.
 * This function is called to register a memory allocation in the `mem_register`
 * array, allowing it to be freed later when the program terminates.
 * @param mem Pointer to the allocated memory block.
 * @note This function should be called immediately after any memory allocation
 * to ensure that it is tracked properly. It checks if the memory allocation was successful
 * and registers the pointer in the `mem_register` array.
 * If the allocation fails, it prints an error message and exits the program.
 * @warning Ensure that the `mem` pointer is not NULL before calling this function.
 * If the pointer is NULL, it indicates that the memory allocation failed,
 * and the program will print an error message and exit.
 * @details
 * The `memtrack` function checks if the `mem` pointer is NULL, indicating a
 * failed memory allocation. If it is NULL, it prints an error message and exits the program
 * with `EXIT_FAILURE`. If the allocation is successful, it reallocates the `mem_register`
 * array to accommodate the new memory block, increments the `mem_count`, and stores
 * the pointer in the `mem_register` array. It also registers the `memfree`
 * function to be called at program termination using `atexit`, ensuring that all
 * allocated memory is properly released when the program exits.
 * @note This function is essential for managing dynamic memory in the NeuroTIC framework,
 * as it ensures that all allocated memory is tracked and can be freed at program termination.
 * It is particularly important in neural network implementations where neurons,
 * inputs, and weights are dynamically allocated. By tracking memory allocations,
 * the framework can prevent memory leaks and ensure that all resources are properly managed.
 */
static inline void memtrack( void *mem ){
    if( !mem ){
		fprintf( stderr , "Failed to allocate memory for neurons in define_net.\n" );
		exit( EXIT_FAILURE );
	}
	void **temp= realloc( mem_register , ++mem_count * sizeof( void * ) );
	if( !temp ){
        fprintf( stderr , "Failed to allocate memory for free tracking.\n" );
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

/**
 * @fn define_net
 * @brief Defines a neural network structure with specified inputs, layers, and neurons per layer.
 * This function initializes a `net` structure with the given parameters,
 * setting the number of inputs, layers, and neurons in each layer.
 * It allocates memory for the `neurons` array and initializes other fields to NULL.
 * @param inputs Total number of input signals expected by the network.
 * @param layers Total number of neuron layers in the network.
 * @param neurons_by_layer Array specifying the number of neurons in each layer.
 * @return A `net` structure initialized with the specified parameters.
 * @note The `neurons_by_layer` array should have a length equal to `layers`.
 * Each element in this array represents the number of neurons in the corresponding layer.
 * The function allocates memory for the `neurons` array and initializes other fields to NULL.
 * It is essential to ensure that the `neurons_by_layer` array is properly defined
 * and matches the expected number of layers in the network.
 * @warning Ensure that the `neurons_by_layer` array is not NULL and has the correct length
 * before calling this function. If the array is NULL or has an incorrect length,
 * it may lead to undefined behavior or memory corruption.
 * The function does not perform any validation on the `neurons_by_layer` array,
 * so it is the caller's responsibility to ensure that the array is correctly defined.
 */
struct net define_net( int inputs, int layers, int *neurons_by_layer ){
	struct net N={
		.inputs= inputs,
		.layers= layers,
		.IN= NULL,
		.N= NULL,
		.B= NULL,
		.OUT= NULL
	};
	memtrack( N.neurons= malloc( N.layers * sizeof( int ) ) );
	memcpy( N.neurons , neurons_by_layer , layers * sizeof( int ) );
	return N;
}

/**
 * @fn build_net
 * @brief Builds a neural network structure based on the defined parameters.
 * This function allocates memory for the neurons, inputs, and outputs of the network,
 * linking them together according to the specified topology.
 * It initializes the `net` structure with the allocated memory and sets up
 * the input and output buffers for each neuron.
 * @param Net Pointer to a `net` structure that has been defined using `define_net`.
 * @return A pointer to the initialized `net` structure with all fields set up.
 * @note The function allocates memory for the neurons, inputs, and outputs,
 * linking them together according to the specified topology. It ensures that
 * all pointers are properly initialized and that the network is ready for use.
 * The function assumes that the `Net` structure has been defined with valid parameters
 * before calling this function.
 * @warning Ensure that the `Net` structure has been defined using `define_net`
 * before calling this function. If the structure is not properly defined,
 * it may lead to undefined behavior or memory corruption.
 */
struct net *build_net( struct net *Net ){
    memtrack( Net->N= malloc( Net->layers * sizeof( struct neuron * ) ) );
    for( int i= 0 ; i < Net->layers ; i++ ) memtrack( Net->N[i]=malloc( Net->neurons[i] * sizeof( struct neuron ) ) );
    memtrack( Net->IN= malloc( Net->inputs * sizeof( float * ) ) );
    for( int i= 0 ; i < Net->neurons[0] ; i++ ){
        Net->N[0][i].inputs= Net->inputs;
        Net->N[0][i].IN= Net->IN;
        memtrack( Net->N[0][i].W= malloc( Net->inputs * sizeof( float ) ) );
    }
    memtrack( Net->B= malloc( ( Net->layers - 1 ) * sizeof( float ** ) ) );
    for( int i= 0 ; i < Net->layers - 1 ; i++ ){
        memtrack( Net->B[i]= malloc( Net->neurons[i] * sizeof( float * ) ) );
        for( int j= 0 ; j < Net->neurons[i] ; j++ ) Net->B[i][j]= &Net->N[i][j].OUT;
    }
    for( int i= 1 ; i < Net->layers ; i++ ) for( int j= 0 ; j < Net->neurons[i] ; j++ ){
        Net->N[i][j].inputs= Net->neurons[i - 1];
        Net->N[i][j].IN= Net->B[i - 1];
        memtrack( Net->N[i][j].W= malloc( Net->neurons[i - 1] * sizeof( float ) ) );
    }
    memtrack( Net->OUT= malloc( Net->neurons[Net->layers -1] * sizeof( float * ) ) );
    for( int i= 0 ; i < Net->neurons[Net->layers - 1] ; i++ ) Net->OUT[i]= &Net->N[Net->layers - 1][i].OUT;
    return Net;
}
