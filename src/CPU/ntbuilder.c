/**
 * @file ntbuilder.c
 * @brief Implementation of network construction functions.
 *
 * Provides memory allocation and pointer setup for neurons, input/output
 * references, and inter-layer buffers. Uses memtrack() to manage memory safely.
 * 
 * @author Oscar Sotomayor
 * @date 2024
 */

#include "ntbuilder.h"
#include "ntmemory.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Initializes a new neural network structure.
 *
 * Validates input pointers and layer sizes. Allocates memory for:
 * - The `neurons` array defining layer sizes.
 * - The neuron matrix `nn`.
 *
 * @param net Pointer to the network structure to initialize.
 * @param neurons_per_layer Array of neuron counts per layer.
 * @param layers_size Number of layers.
 * @return Pointer to the initialized network on success, NULL on invalid input.
 */
struct net_s *newnet( net_s *net , uint16_t *neurons_per_layer , size_t layers_size ){
    if( !net || !neurons_per_layer || net->layers < 1 || layers_size != net->layers ) return NULL;
    for( layer_t i = 0 ; i < net->layers ; i++ ) if( neurons_per_layer[i] < 1 ) return NULL;
    net->in= NULL;
    net->nn= NULL;
    net->bff= NULL;
    net->out= NULL;
    memtrack( net->neurons= calloc( net->layers , sizeof( uint16_t ) ) );
    memcpy( net->neurons , neurons_per_layer, net->layers * sizeof( uint16_t ) );
    memtrack( net->nn= calloc( net->layers , sizeof( neuron_s * ) ) );
    for( uint16_t i = 0 ; i < net->layers ; i++ ) memtrack( net->nn[i]= calloc( net->neurons[i] , sizeof( neuron_s ) ) );
    return net;
}


/**
 * @brief Builds the internal buffers and connections of a neural network.
 *
 * Allocates memory for inputs, outputs, and inter-layer buffers according to
 * the `bff_wiring` configuration. Sets neuron input pointers and weight arrays.
 *
 * Buffer type explanations:
 * - 'M': Mixed buffer; each pointer is set based on `src_type`.
 *   - 'N': Points to another buffer in the network.
 *   - 'I': Points to the network input array.
 *   - 'O': Points to the network output array.
 * - 'N': Shared network buffer.
 * - 'I': Network input array.
 * - 'O': Network output array.
 *
 * @param net Pointer to the network to build.
 * @return Pointer to the fully constructed network.
 */
struct net_s *buildnet(net_s *net){
    memtrack(net->in  = malloc(net->inputs * sizeof(float *)));
    for(uint16_t i = 0; i < net->neurons[0]; i++){
        net->nn[0][i].inputs = net->inputs;
        net->nn[0][i].in     = net->in;
        memtrack(net->nn[0][i].w = malloc(net->inputs * sizeof(float)));
    }
    memtrack(net->out = malloc(net->neurons[net->layers - 1] * sizeof(float *)));
    for(uint16_t i = 0; i < net->neurons[net->layers - 1]; i++)
        net->out[i] = &net->nn[net->layers - 1][i].out;
    if(net->layers > 1){
        memtrack(net->bff = malloc((net->layers - 1) * sizeof(float ***)));
        for(uint16_t i = 0; i < net->layers - 1; i++){
            memtrack(net->bff[i] = malloc(net->wiring[i].arrays * sizeof(float **)));
            for(uint16_t j = 0; j < net->wiring[i].arrays; j++){
                if(net->wiring[i].array_type[j] == 'M'){
                    memtrack(net->bff[i][j] = malloc(net->wiring[i].size[j] * sizeof(float *)));
                    for(uint32_t k = 0; k < net->wiring[i].size[j]; k++){
                        switch(net->wiring[i].src_type[j][k]){
                            case 'N':
                                net->bff[i][j][k] = &net->nn[net->wiring[i].src_layer[j][k]][net->wiring[i].src_index[j][k]].out;
                                break;
                            case 'I':
                                net->bff[i][j][k] = net->in[net->wiring[i].src_index[j][k]];
                                break;
                            case 'O':
                                net->bff[i][j][k] = net->out[net->wiring[i].src_index[j][k]];
                                break;
                            default:
                                net->bff[i][j][k] = NULL;
                                break;
                        }
                    }
                }
            }
        }
        for(uint16_t i = 0; i < net->layers - 1 ; i++) for( uint16_t j = 0; j < net->wiring[i].arrays; j++) switch( net->wiring[i].array_type[j] ){
            case 'M':
                break;
            case 'N':
                net->bff[i][j] = net->bff[net->wiring[i].src_layer[j][0]][net->wiring[i].src_index[j][0]];
                net->wiring[i].size[j]= net->wiring[net->wiring[i].src_layer[j][0]].size[net->wiring[i].src_index[j][0]];
                break;
            case 'I':
                net->bff[i][j] = net->in;
                net->wiring[i].size[j]= net->inputs;
                break;
            case 'O':
                net->bff[i][j] = net->out;
                net->wiring[i].size[j]= net->neurons[net->layers - 1];
                break;
            default:
                net->bff[i][j] = NULL;
                net->wiring[i].size[j]= 1;
                break;
        }
    }
    for(uint16_t i = 1; i < net->layers; i++){
        for(uint16_t j = 0; j < net->neurons[i]; j++){
            net->nn[i][j].in     = net->bff[i - 1][net->nn[i][j].bff_idx];
            net->nn[i][j].inputs = net->wiring[i - 1].size[net->nn[i][j].bff_idx];
            memtrack(net->nn[i][j].w = malloc(net->nn[i][j].inputs * sizeof(float)));
        }
    }
    return net;
}