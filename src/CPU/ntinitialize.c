/**
 * @file ntinitialize.c
 * @brief Implementation of random weight initialization.
 *
 * Initializes all neuron weights within their activation function's specified
 * range and sets biases to zero. Uses `rand()` seeded with the current time.
 * 
 * @author Oscar Sotomayor
 * @date 2024
 */

#include "ntinitialize.h"

#include "ntactivation.h"
#include <stdlib.h>
#include <time.h>


/**
 * @brief Randomly sets weights and biases of a network.
 *
 * For each neuron:
 * - Weight `w[k]` is randomized in the range defined by `ntact_rand_range[fn]`.
 * - Bias `b` is set to 0.0f.
 *
 * @param net Pointer to the network to initialize.
 */
void randnet( net_s *net ){
    srand( time( NULL ) );
    for( uint16_t i= 0, j, k ; i < net->layers ; i++ ) for( j= 0 ; j < net->neurons[i] ; j++ ){
        for( k= 0 ; k < net->nn[i][j].inputs ; k++ ) net->nn[i][j].w[k]= ( ( float )rand( ) / RAND_MAX ) * ( ntact_rand_range[net->nn[i][j].fn][1] - ntact_rand_range[net->nn[i][j].fn][0] ) + ntact_rand_range[net->nn[i][j].fn][0];
        net->nn[i][j].b = 0.0f;
    }
}
