/**
 * @file nttrain.c
 * @brief Implementation of backpropagation training for NeuroTIC networks.
 *
 * Provides functions to allocate training datasets and train feedforward
 * networks using standard backpropagation.
 * 
 * @author Oscar Sotomayor
 * @date 2026
 */

#include "nttrain.h"

#include "ntactivation.h"
#include "ntcalculate.h"
#include "ntmemory.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * @details
 * Allocates memory for training data arrays.  
 * Initializes `in` and `results` arrays based on the network structure and number of samples.
 */
void newtraindata( traindata_t *train_data , net_s *net ){
    train_data->in= memtrack( calloc( train_data->samples , sizeof( data_t * ) ) );
    train_data->results= memtrack( calloc( train_data->samples , sizeof( data_t * ) ) );
    layer_t last_layer= net->layers - 1;
    for( sample_t i= 0 ; i < train_data->samples ; i++ ){
        train_data->in[i]= memtrack( calloc( net->inputs , sizeof( data_t ) ) );
        train_data->results[i]= memtrack( calloc( net->neurons[last_layer] , sizeof( data_t ) ) );
    }
}


/**
 * @details
 * Implements the backpropagation algorithm to train the network.
 * For each training sample:
 * - Computes outputs via feedforward.
 * - Calculates errors for each neuron.
 * - Propagates deltas backward and updates weights and biases.
 * - Repeats until error is below `tolerance` or `max_attempts` is reached.
 */
attempts_t backpropagation( net_s *net , traindata_t *train_data ){
    attempts_t attempt= train_data->max_attempts;
    const layer_t prev_layer= net->layers - 1;
    layer_t next_layer;
    size_t max_mem= 0;
    for( layer_t i= 0 ; i < net->layers ; i++ ) max_mem= (size_t)net->neurons[i] > max_mem ? (size_t)net->neurons[i] : max_mem;
    max_mem*= sizeof( data_t );
    const size_t inputs_size= (size_t)net->inputs * sizeof( data_t );
    data_t *restrict in= malloc( inputs_size );
    precision_t err_total, *restrict delta= malloc( max_mem ), *restrict delta_h= malloc( max_mem );
    for( input_t i= 0 ; i < net->inputs ; i++ ) net->in[i]= &in[i];
    do{
        err_total= 0;
        for( sample_t i= 0 ; i < train_data->samples ; i++ ){
            memcpy( in , train_data->in[i] , inputs_size );
            feedforward( net );
            for( uint16_t j= 0 ; j < net->neurons[prev_layer] ; j++ ){
                err_total+= fabsf( delta[j]= train_data->results[i][j] - *net->out[j] );
                delta[j]*= ntact_activation[net->nn[prev_layer][j].fn][1]( weighing( &net->nn[prev_layer][j]));
            }
            if( err_total < train_data->tolerance ) continue;
            for( layer_t j= prev_layer ; j-- > 0 ; ){
                next_layer= j + 1;
                memset( delta_h , 0 , max_mem );
                for( uint16_t k= 0 ; k < net->neurons[next_layer] ; k++ ) for( input_t l= 0 ; l < net->nn[next_layer][k].inputs ; l++ ) delta_h[l]+= delta[k] * net->nn[next_layer][k].w[l];
                for( uint16_t k= 0 ; k < net->neurons[j] ; k++ ) delta_h[k]*= ntact_activation[net->nn[j][k].fn][1]( weighing( &net->nn[j][k] ) );
                for( uint16_t k= 0 ; k < net->neurons[next_layer] ; k++ ){
                    for( input_t l= 0 ; l < net->nn[next_layer][k].inputs ; l++ ) net->nn[next_layer][k].w[l]+= delta[k] * train_data->learning_rate * *net->nn[next_layer][k].in[l];
                    net->nn[next_layer][k].b+= delta[k] * train_data->learning_rate;
                }
                memcpy( delta , delta_h , max_mem );
            }
            for( uint16_t j= 0 ; j < net->neurons[0] ; j++ ){
                for( input_t k= 0 ; k < net->nn[0][j].inputs ; k++ ) net->nn[0][j].w[k]+= delta[j] * train_data->learning_rate * *net->nn[0][j].in[k];
                net->nn[0][j].b+= delta[j] * train_data->learning_rate;
            }
        }
    } while( --attempt && err_total > train_data->tolerance );
    free( delta );
    free( delta_h );
    free( in );
    return train_data->max_attempts - attempt;
}