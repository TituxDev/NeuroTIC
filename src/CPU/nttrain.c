/**
 * @file nttrain.c
 * @brief Implementation of backpropagation training for NeuroTIC networks.
 *
 * Provides functions to allocate training datasets and train feedforward
 * networks using standard backpropagation.
 * 
 * @author Oscar Sotomayor
 * @date 2024
 */

#include "nttrain.h"

#include "ntactivation.h"
#include "ntcalculate.h"
#include "ntmemory.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * @brief Allocates memory for input and output arrays in training data.
 *
 * Each sample gets an array for inputs and an array for expected outputs.
 *
 * @param train_data Pointer to training data structure to initialize.
 * @param net Pointer to the network being trained.
 */
void newtraindata( traindata_t *train_data , net_s *net ){
    memtrack( train_data->in=calloc( train_data->samples , sizeof( float * ) ) );
    memtrack( train_data->results=calloc( train_data->samples , sizeof( float * ) ) );
    for( uint16_t i= 0, last_ly= net->layers - 1 ; i < train_data->samples ; i++ ){
        memtrack( train_data->in[i]= calloc( net->inputs , sizeof( float ) ) );
        memtrack( train_data->results[i]= calloc( net->neurons[last_ly] , sizeof( float ) ) );
    }
}


/**
 * @brief Performs backpropagation training on a feedforward network.
 *
 * - Computes outputs via feedforward.
 * - Calculates errors for each neuron.
 * - Propagates deltas backward and updates weights and biases.
 * - Repeats until error is below `tolerance` or `max_attempts` is reached.
 *
 * @param net Pointer to the network to train.
 * @param train_data Pointer to the training dataset and parameters.
 * @return Number of training iterations performed.
 */
int backpropagation( net_s *net , traindata_t *train_data ){
    uint64_t attempt= train_data->max_attempts;
    const uint16_t prev_layer= net->layers - 1;
    const int32_t penu_layer= ( int32_t )prev_layer - 1;
    uint16_t next_layer;
    size_t max_mem= 0;
    for( uint16_t i= 0 ; i < net->layers ; i++ ) if( ( size_t )net->neurons[i] > max_mem ) max_mem= ( size_t )net->neurons[i];
    max_mem*= sizeof( float );
    const size_t inputs_size= ( size_t )net->inputs * sizeof( float );
    float err_total, *restrict delta= malloc( max_mem ), *restrict delta_h= malloc( max_mem ), *restrict in= malloc( inputs_size );
    for( uint32_t i= 0 ; i < net->inputs ; i++ ) net->in[i]= &in[i];
    do{
        err_total= 0;
        for( uint64_t i= 0 ; i < train_data->samples ; i++ ){
            memcpy( in , train_data->in[i] , inputs_size );
            feedforward( net );
            for( uint16_t j= 0 ; j < net->neurons[prev_layer] ; j++ ){
                err_total+= fabsf( delta[j]= train_data->results[i][j] - *net->out[j] );
                delta[j]*= ntact_activation[net->nn[prev_layer][j].fn][1]( weighing( &net->nn[prev_layer][j]));
            }
            if( err_total < train_data->tolerance ) continue;
            for( int32_t j= penu_layer ; j >= 0 ; j-- ){
                next_layer= j + 1;
                memset( delta_h , 0 , max_mem );
                for( uint16_t k= 0 ; k < net->neurons[next_layer] ; k++ ) for( uint32_t l= 0 ; l < net->nn[next_layer][k].inputs ; l++ ) delta_h[l]+= delta[k] * net->nn[next_layer][k].w[l];
                for( uint16_t k= 0 ; k < net->neurons[j] ; k++ ) delta_h[k]*= ntact_activation[net->nn[j][k].fn][1]( weighing( &net->nn[j][k] ) );
                for( uint16_t k= 0 ; k < net->neurons[next_layer] ; k++ ){
                    for( uint32_t l= 0 ; l < net->nn[next_layer][k].inputs ; l++ ) net->nn[next_layer][k].w[l]+= delta[k] * train_data->learning_rate * *net->nn[next_layer][k].in[l];
                    net->nn[next_layer][k].b+= delta[k] * train_data->learning_rate;
                }
                memcpy( delta , delta_h , max_mem );
            }
            for( uint16_t j= 0 ; j < net->neurons[0] ; j++ ){
                for( uint32_t k= 0 ; k < net->nn[0][j].inputs ; k++ ) net->nn[0][j].w[k]+= delta[j] * train_data->learning_rate * *net->nn[0][j].in[k];
                net->nn[0][j].b+= delta[j] * train_data->learning_rate;
            }
        }
    } while( --attempt && err_total > train_data->tolerance );
    free( delta );
    free( delta_h );
    free( in );
    return train_data->max_attempts - attempt;
}