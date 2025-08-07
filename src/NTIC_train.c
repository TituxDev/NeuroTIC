/**
 * @note This file is part of NeuroTIC, a neural network library.
 * @author Oscar Sotomayor
 * @date 2025
 * @version 1.0
 * @license Mozilla Public License v. 2.0 (https://mozilla.org/MPL/2.0/)
 * @file training.c
 * @brief Implementation of training functions for the neural network.
 * @details
 * This source file implements the training functionality for the neural network defined in
 * `neuroticore.h`. It includes the `train` function that performs backpropagation
 * to adjust the weights and biases of the network based on input samples and expected results.
 * The training process iterates through the provided samples, calculates the error,
 * and updates the weights using the specified learning rate until the error is below a defined tolerance
 * or the maximum number of attempts is reached.
 * Future revisions may include optimizations for batch processing, adaptive learning rates,
 * or advanced training techniques such as momentum or regularization.
 */

 #include "NTIC_train.h"

#include "NTIC_neuroticore.h"
#include "NTIC_activation.h"
#include "NTIC_calculate.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * @fn train
 * @brief Trains the neural network using backpropagation.
 * @param Net The network structure to be trained.
 * @param inputs Array of input samples for training.
 * @param results Array of expected output results corresponding to the inputs.
 * @param samples Number of input-output pairs in the training data.
 * @param learning_rate The rate at which the network learns during training.
 * @param tolerance The acceptable error threshold for training completion.
 * @param max_attempts Maximum number of training iterations allowed.
 * @return The number of attempts made to train the network before reaching the tolerance or max attempts.
 * @details
 * This function implements the backpropagation algorithm to adjust the weights and biases of the neural network.
 * It processes each input sample, calculates the output, computes the error, and updates the weights accordingly.
 * The function continues until either the total error is below the specified tolerance or the maximum number of attempts is reached.
 * The training process involves iterating through the layers of the network, applying the activation function,
 * and adjusting the weights based on the calculated deltas.
 * The function assumes that the network structure, including the number of layers, neurons, and their
 * respective inputs and weights, is properly initialized before training.
 * The training process is designed to be flexible, allowing for dynamic adjustment of weights based on the
 * learning rate and the error calculated from the expected results.
 * The function returns the number of attempts made to train the network.
 * If the training is successful and the error is within the specified tolerance, it will return the
 * number of attempts taken to achieve this. If the maximum number of attempts is reached without
 * achieving the desired tolerance, it will return that value.
 */
int train( struct net *Net , float **inputs , float **results , unsigned int samples , float learning_rate , float tolerance , unsigned int max_attempts ){
    unsigned int attempt= 0, max_mem= 0, inputs_size= Net->inputs * sizeof( float ), prev_layer= Net->layers - 1, next_layer, penu_layer= prev_layer - 1;
    for( unsigned int i= 0 ; i < Net->layers ; i++ ) max_mem= fmax( max_mem , Net->neurons[i] );
    max_mem*= sizeof( float );
    float err_total, *delta= malloc( max_mem ), *delta_h= malloc( max_mem ), *IN= malloc( inputs_size );
    for( unsigned int i= 0 ; i < Net->inputs ; i++ ) Net->IN[i]= &IN[i];
    do{
        err_total= 0;
        for( unsigned int i= 0, j, k, l ; i < samples ; i++ ){
            memcpy( IN , inputs[i] , inputs_size );
            run_net( Net );
            for( j= 0 ; j < Net->neurons[prev_layer] ; j++ ){
                err_total+= fabs( delta[j]= results[i][j] - *Net->OUT[j] );
                delta[j]*= activation[Net->N[prev_layer][j].FUNC][1]( weighing( Net->N[prev_layer][j]));
            }
            if( err_total < tolerance ) continue;
            for( int j= ( int )penu_layer ; j >= 0 ; j-- ){
                next_layer= j + 1;
                memset( delta_h , 0 , max_mem );
                for( k= 0 ; k < Net->neurons[next_layer] ; k++ ) for( l= 0 ; l < Net->N[next_layer][k].inputs ; l++ ) delta_h[l]+= delta[k] * Net->N[next_layer][k].W[l];
                for( k= 0 ; k < Net->neurons[j] ; k++ ) delta_h[k]*= activation[Net->N[j][k].FUNC][1]( weighing( Net->N[j][k] ) );
                for( k= 0 ; k < Net->neurons[next_layer] ; k++ ){
                    for( l= 0 ; l < Net->N[next_layer][k].inputs ; l++ ) Net->N[next_layer][k].W[l]+= delta[k] * learning_rate * *Net->N[next_layer][k].IN[l];
                    Net->N[next_layer][k].B+= delta[k] * learning_rate;
                }
                memcpy( delta , delta_h , max_mem );
            }
            for( j= 0 ; j < Net->neurons[0] ; j++ ){
                for( k= 0 ; k < Net->N[0][j].inputs ; k++ ) Net->N[0][j].W[k]+= delta[j] * learning_rate * *Net->N[0][j].IN[k];
                Net->N[0][j].B+= delta[j] * learning_rate;
            }
        }
    } while( ++attempt < max_attempts && err_total > tolerance );
    free( delta );
    free( delta_h );
    free( IN );
    return attempt;
}