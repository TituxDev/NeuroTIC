/**
 * @file ntinitialize.c
 * @brief Implementation Random Initialization.
 *
 * Initializes all neuron weights within their activation function's specified range and sets biases to zero.  
 * Uses `rand()` seeded with the current time.
 * 
 * @author Oscar Sotomayor
 * @date 2026
 */

#include "ntinitialize.h"

#include "ntactivation.h"
#include <stdlib.h>
#include <time.h>


/**
 * @details
 * Iterates through every neuron in the network, assigning each of its
 * weights a value drawn from `rand()` and linearly scaled into the range
 * defined for that neuron's activation function in `ntact_rand_range`.
 * Biases are set to zero for all neurons.
 */
void randnet( net_s *net ){
    srand( time( NULL ) );
    for( uint16_t i= 0, j ; i < net->layers ; i++ ) for( j= 0 ; j < net->neurons[i] ; j++ ){
        for( uint32_t k= 0 ; k < net->nn[i][j].inputs ; k++ ) net->nn[i][j].w[k]= ( ( float )rand( ) / RAND_MAX ) * ( ntact_rand_range[net->nn[i][j].fn][1] - ntact_rand_range[net->nn[i][j].fn][0] ) + ntact_rand_range[net->nn[i][j].fn][0];
        net->nn[i][j].b = 0.0f;
    }
}
