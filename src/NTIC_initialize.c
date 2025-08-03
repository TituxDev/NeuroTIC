/**
 * @note This file is part of the NeuroTIC project.
 * @author Oscar Sotomayor
 * @date 2025
 * @version 1.0
 * @license Mozilla Public License v. 2.0 (https://mozilla.org/MPL/2.0/)
 * @file initialize.c
 * @brief Source file for initializing neural networks in the NeuroTIC framework.
 * @details
 * This source file implements the `rand_net` function, which initializes the weights and biases
 * of the neurons in a neural network with random values. The function uses the `rand_range` array
 * defined in `activation.h` to determine the range of random values for each activation function.
 * It iterates through all layers and neurons, setting the weights and biases accordingly.
 * The random number generation is seeded with the current time to ensure different results on each run.
 * The file also includes necessary headers for the NeuroTIC framework, including definitions for
 * neural network structures and activation functions.
 * This implementation is designed to work with the `net` and `neuron` structures defined in `neuroticore.h`.
 * It is part of the larger NeuroTIC project, which provides a framework for building and training neural networks.
 */

#include "NTIC_initialize.h"

#include "NTIC_neuroticore.h"
#include "NTIC_activation.h"
#include <stdlib.h>
#include <time.h>

/**
 * @fn void rand_net(struct net *Net)
 * @brief Initializes the weights and biases of the neural network with random values.
 * @param Net Pointer to the neural network structure to be initialized.
 */
void rand_net( struct net *Net ){
    srand( time( NULL ) );
    for( int i= 0, j, k ; i < Net->layers ; i++ ) for( j= 0 ; j < Net->neurons[i] ; j++ ){
        for( k= 0 ; k < Net->N[i][j].inputs ; k++ ) Net->N[i][j].W[k]= ( ( float )rand() / RAND_MAX ) * ( rand_range[Net->N[i][j].FUNC][1] - rand_range[Net->N[i][j].FUNC][0] ) + rand_range[Net->N[i][j].FUNC][0];
        Net->N[i][j].B= ( ( float )rand() / RAND_MAX ) * ( rand_range[Net->N[i][j].FUNC][1] - rand_range[Net->N[i][j].FUNC][0] ) + rand_range[Net->N[i][j].FUNC][0];
    }
}