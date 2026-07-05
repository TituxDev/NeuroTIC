/**
 * @file ntdefinition.c
 * @brief Network Definition Functions.
 *
 * @author Oscar Sotomayor (Titux)
 *
 * @details
 * This module implements an interactive, console-driven interface that
 * lets a user manually define a NeuroTIC network's structure and topology.
 */

#include "ntbuilder.h"
#include "ntdefinition.h"
#include "ntmemory.h"
#include <stdio.h>
#include <stdlib.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"

/**
 * @details
 * Prompts interactively for the network's external input count, total
 * layer count, and neuron count per layer -- each defaulting to 1 if
 * entered as zero. Builds the network's base structure from the collected
 * parameters.
 */
net_s *definestructure( net_s *net ){
    net->in= NULL;
    net->nn= NULL;
    net->wiring= NULL;
    net->bff= NULL;
    net->out= NULL;
    printf( "Network structure\n" );
    printf( "  Inputs: " );
    scanf( " %u" , &net->inputs );
    net->inputs+= !net->inputs;
    printf( "  Layers: " );
    scanf( " %hu" , &net->layers );
    net->layers+= !net->layers;
    uint16_t *neurons= calloc( net->layers , sizeof( uint16_t ) );
    printf( "  Neurons per layer: \n" );
    for( layer_t i= 0 ; i < net->layers ; i++ ){
        printf( "  [%u]: " , i );
        scanf( " %hu" , &neurons[i] );
        neurons[i]+= !neurons[i];
    }
    newnet( net , neurons , net->layers );
    free( neurons );
    return net;
}

/**
 * @details
 * For each layer from the second onward, prompts interactively for that
 * layer's wiring descriptor -- array count, per-array type, and the
 * type-specific fields required by each type -- followed by every neuron's
 * bff_idx selection for that layer. Once every layer has been defined,
 * builds the network.
 *
 * @todo An invalid top-level array type is sentineled as 'X', but an
 * invalid element-level type inside an 'M' array is left unmarked in
 * src_type. Resolve this inconsistency once input validation is added.
 */
net_s *definetopology( net_s *net ){
    printf( "Network topology\n" );
    for( layer_t i= 1 ; i < net->layers ; i++ ){
        layer_t L= i - 1;
        printf( "Layer %u" , i );
        printf( "\n  Input sets: " );
        scanf( " %hhu" , &net->wiring[L].arrays );
        net->wiring[L].arrays+= !net->wiring[L].arrays;
        net->wiring[L].array_type= createregister( net , calloc( net->wiring[L].arrays , sizeof( type_t ) ) );
        net->wiring[L].size= createregister( net , calloc( net->wiring[L].arrays , sizeof( input_t ) ) );
        net->wiring[L].src_layer= createregister( net , calloc( net->wiring[L].arrays , sizeof( layer_t * ) ) );
        net->wiring[L].src_index= createregister( net , calloc( net->wiring[L].arrays , sizeof( uint16_t * ) ) );
        net->wiring[L].src_type= createregister( net , calloc( net->wiring[L].arrays , sizeof( type_t * ) ) );
        for( index_t j= 0 ; j < net->wiring[L].arrays ; j++ ){
            printf( "  [%u] Type (N/I/O/M): " , j );
            scanf( " %c" , (char*)&net->wiring[L].array_type[j] );
            switch( net->wiring[L].array_type[j]) {
                case 'N':
                    net->wiring[L].src_layer[j]= createregister( net , calloc( 1 , sizeof( layer_t ) ) );
                    net->wiring[L].src_index[j]= createregister( net , calloc( 1 , sizeof( uint16_t ) ) );
                    net->wiring[L].src_type[j]= NULL;
                    printf( "      Source layer: " );
                    scanf( "%hu" , net->wiring[L].src_layer[j] );
                    printf( "      Source BFF: " );
                    scanf( "%hu" , net->wiring[L].src_index[j] );
                    break;
                case 'I':
                    net->wiring[L].src_layer[j]= NULL;
                    net->wiring[L].src_index[j]= NULL;
                    net->wiring[L].src_type[j]= NULL;
                    break;
                case 'O':
                    net->wiring[L].src_layer[j]= NULL;
                    net->wiring[L].src_index[j]= NULL;
                    net->wiring[L].src_type[j]= NULL;
                    break;
                case 'M':
                    printf( "      BFF size: " );
                    scanf( " %u" , &net->wiring[L].size[j] );
                    net->wiring[L].src_layer[j]= createregister( net , calloc( net->wiring[L].size[j] , sizeof( layer_t ) ) );
                    net->wiring[L].src_index[j]= createregister( net , calloc( net->wiring[L].size[j] , sizeof( uint16_t ) ) );
                    net->wiring[L].src_type[j]= createregister( net , calloc( net->wiring[L].size[j] , sizeof( type_t ) ) );
                    for( uint16_t k= 0 ; k < net->wiring[L].size[j] ; k++ ){
                        printf( "      [%u] Source type (N/I/O): " , k );
                        scanf( " %c" , &net->wiring[L].src_type[j][k] );
                        switch( net->wiring[L].src_type[j][k] ){
                            case 'N':
                                printf( "          Source layer: " );
                                scanf( "%hu" , &net->wiring[L].src_layer[j][k] );
                                printf( "          Source index: " );
                                scanf( "%hu" , &net->wiring[L].src_index[j][k] );
                                break;
                            case 'I':
                                net->wiring[L].src_layer[j][k]= 0;
                                printf( "          Source index: " );
                                scanf( "%hu" , &net->wiring[L].src_index[j][k] );
                                break;
                            case 'O':
                                net->wiring[L].src_layer[j][k]= 0;
                                printf( "          Source index: " );
                                scanf( "%hu" , &net->wiring[L].src_index[j][k] );
                                break;
                            default:
                                net->wiring[L].src_layer[j][k]= 0;
                                net->wiring[L].src_index[j][k]= 0;
                                break;
                        }
                    }
                    break;
                default:
                    net->wiring[L].array_type[j]= 'X';
                    break;
            }
        }
        printf( "\n  Neuron: \n" );
        for( uint16_t j= 0 ; j < net->neurons[i] ; j++ ){
            printf( "  [%u]: " , j );
            scanf( " %hhu" , &net->nn[i][j].bff_idx );
        }
    }
    buildnet( net );
    return net;
}

#pragma GCC diagnostic pop