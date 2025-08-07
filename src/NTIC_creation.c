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

#include "NTIC_creation.h"

#include "NTIC_neuroticore.h"
#include "NTIC_memory.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    for( unsigned int i= 0 ; i < Net->layers ; i++ ) memtrack( Net->N[i]=malloc( Net->neurons[i] * sizeof( struct neuron ) ) );
    memtrack( Net->IN= malloc( Net->inputs * sizeof( float * ) ) );
    for( unsigned int i= 0 ; i < Net->neurons[0] ; i++ ){
        Net->N[0][i].inputs= Net->inputs;
        Net->N[0][i].IN= Net->IN;
        memtrack( Net->N[0][i].W= malloc( Net->inputs * sizeof( float ) ) );
    }
    memtrack( Net->B= malloc( ( Net->layers - 1 ) * sizeof( float ** ) ) );
    for( unsigned int i= 0 ; i < Net->layers - 1 ; i++ ){
        memtrack( Net->B[i]= malloc( Net->neurons[i] * sizeof( float * ) ) );
        for( unsigned int j= 0 ; j < Net->neurons[i] ; j++ ) Net->B[i][j]= &Net->N[i][j].OUT;
    }
    for( unsigned int i= 1 ; i < Net->layers ; i++ ) for( unsigned int j= 0 ; j < Net->neurons[i] ; j++ ){
        Net->N[i][j].inputs= Net->neurons[i - 1];
        Net->N[i][j].IN= Net->B[i - 1];
        memtrack( Net->N[i][j].W= malloc( Net->neurons[i - 1] * sizeof( float ) ) );
    }
    memtrack( Net->OUT= malloc( Net->neurons[Net->layers -1] * sizeof( float * ) ) );
    for( unsigned int i= 0 ; i < Net->neurons[Net->layers - 1] ; i++ ) Net->OUT[i]= &Net->N[Net->layers - 1][i].OUT;
    return Net;
}
