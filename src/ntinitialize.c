/**
 * ntinitialize.c - Initialization routines for NeuroTIC neural networks
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * Implements functions to initialize network parameters, such as randomly
 * assigning weights within recommended ranges based on the neuron's activation function.
 * Utilizes standard C library functions for randomness and system time seeding.
 *
 * This module depends on the activation module (`ntactivation`) to
 * retrieve suitable initialization ranges for weights.
 */

#include "ntinitialize.h"

#include "ntactivation.h"
#include <stdlib.h>
#include <time.h>

/**
 * @brief Randomly initialize all neuron weights in the network.
 *
 * Seeds the random number generator with the current system time.
 * Then iterates over all neurons and their inputs, setting weights
 * to random float values within the recommended initialization
 * range specific to each neuron's activation function.
 *
 * Bias values are initialized to 0.0f.
 *
 * @param net Pointer to a fully built `net_t` network structure.
 */
void randnet( net_t *net ){
    srand( time( NULL ) );
    for( uint16_t i= 0, j, k ; i < net->layers ; i++ ) for( j= 0 ; j < net->neurons[i] ; j++ ){
        for( k= 0 ; k < net->nn[i][j].inputs ; k++ ) net->nn[i][j].w[k]= ( ( float )rand( ) / RAND_MAX ) * ( ntact_rand_range[net->nn[i][j].fn][1] - ntact_rand_range[net->nn[i][j].fn][0] ) + ntact_rand_range[net->nn[i][j].fn][0];
        net->nn[i][j].b = 0.0f;
    }
}
