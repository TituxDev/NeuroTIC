/**
 * nttrain.c - Implementation of training functions for NeuroTIC
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * Contains core training routines for the NeuroTIC framework, including
 * allocation and initialization of training data structures and backpropagation
 * algorithm for supervised learning.
 */

#include "nttrain.h"

#include "ntactivation.h"
#include "ntcalculate.h"
#include "ntmemory.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * @brief Allocate and initialize training data buffers.
 *
 * Allocates memory for inputs and expected results arrays in the training data
 * structure based on the number of samples and network topology.
 * Uses `memtrack` for automatic memory management.
 *
 * @param train_data Pointer to training data structure to initialize.
 * @param net Pointer to the neural network structure.
 */
void newtraindata( traindata_t *train_data , net_t *net ){
    memtrack( train_data->in=calloc( train_data->samples , sizeof( float * ) ) );
    memtrack( train_data->results=calloc( train_data->samples , sizeof( float * ) ) );
    for( unsigned int i= 0, last_ly= net->layers - 1 ; i < train_data->samples ; i++ ){
        memtrack( train_data->in[i]= calloc( net->inputs , sizeof( float ) ) );
        memtrack( train_data->results[i]= calloc( net->neurons[last_ly] , sizeof( float ) ) );
    }
}

/**
 * @brief Train the neural network using the backpropagation algorithm.
 *
 * Performs iterative supervised learning on the provided network using
 * the training data. Updates weights and biases based on errors until
 * either the maximum number of attempts is reached or the error tolerance
 * is satisfied.
 *
 * The function:
 *  - Runs the network on each training sample,
 *  - Computes output errors and propagates them backward,
 *  - Adjusts weights and biases using the learning rate,
 *  - Continues iterating over epochs to minimize error.
 *
 * @param net Pointer to the neural network to train.
 * @param train_data Pointer to training data containing inputs, expected outputs,
 *                   learning rate, max attempts, and tolerance.
 * @return The number of attempts performed during training.
 */
int backpropagation( net_t *net , traindata_t *train_data ){
    unsigned int attempt= 0, max_mem= 0, inputs_size= net->inputs * sizeof( float ), prev_layer= net->layers - 1, next_layer, penu_layer= prev_layer - 1;
    for( unsigned int i= 0 ; i < net->layers ; i++ ) max_mem= fmax( max_mem , net->neurons[i] );
    max_mem*= sizeof( float );
    float err_total, *delta= malloc( max_mem ), *delta_h= malloc( max_mem ), *in= malloc( inputs_size );
    for( unsigned int i= 0 ; i < net->inputs ; i++ ) net->in[i]= &in[i];
    do{
        err_total= 0;
        for( unsigned int i= 0, j, k, l ; i < train_data->samples ; i++ ){
            memcpy( in , train_data->in[i] , inputs_size );
            feedforward( net );
            for( j= 0 ; j < net->neurons[prev_layer] ; j++ ){
                err_total+= fabs( delta[j]= train_data->results[i][j] - *net->out[j] );
                delta[j]*= ntact_activation[net->nn[prev_layer][j].fn][1]( weighing( &net->nn[prev_layer][j]));
            }
            if( err_total < train_data->tolerance ) continue;
            for( int j= ( int )penu_layer ; j >= 0 ; j-- ){
                next_layer= j + 1;
                memset( delta_h , 0 , max_mem );
                for( k= 0 ; k < net->neurons[next_layer] ; k++ ) for( l= 0 ; l < net->nn[next_layer][k].inputs ; l++ ) delta_h[l]+= delta[k] * net->nn[next_layer][k].w[l];
                for( k= 0 ; k < net->neurons[j] ; k++ ) delta_h[k]*= ntact_activation[net->nn[j][k].fn][1]( weighing( &net->nn[j][k] ) );
                for( k= 0 ; k < net->neurons[next_layer] ; k++ ){
                    for( l= 0 ; l < net->nn[next_layer][k].inputs ; l++ ) net->nn[next_layer][k].w[l]+= delta[k] * train_data->learning_rate * *net->nn[next_layer][k].in[l];
                    net->nn[next_layer][k].b+= delta[k] * train_data->learning_rate;
                }
                memcpy( delta , delta_h , max_mem );
            }
            for( j= 0 ; j < net->neurons[0] ; j++ ){
                for( k= 0 ; k < net->nn[0][j].inputs ; k++ ) net->nn[0][j].w[k]+= delta[j] * train_data->learning_rate * *net->nn[0][j].in[k];
                net->nn[0][j].b+= delta[j] * train_data->learning_rate;
            }
        }
    } while( ++attempt < train_data->max_attempts && err_total > train_data->tolerance );
    free( delta );
    free( delta_h );
    free( in );
    return attempt;
}