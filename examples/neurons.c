/**
 * @file neurons.c
 * @author Oscar Sotomayor
 * @brief Standalone neuron_s example with explicit connectivity.
 * 
 * This file provides a minimal, executable example that demonstrates
 * how multiple neuron_s instances can be connected and evaluated
 * without using the full NeuroTIC network container.
 *
 * The example:
 * - constructs a small set of neurons manually,
 * - connects them using direct pointer references,
 * - evaluates them using an explicit external loop,
 * - and prints the resulting output.
 *
 * The purpose of this file is illustrative.
 * It shows how neuron_s behaves as a passive structure
 * when driven by user-defined execution code.
 *
 * This example does not represent the complete NeuroTIC execution model.
 * No net_s, wiring_s, training, or activation dispatch is involved.
 *
 * For the structural definition of neuron_s, see:
 * - ntcore.h
 * - neuron_s.dox
 *
 * @section neuron_example Example topology
 *
 * The following diagram represents the concrete topology
 * implemented in this example.
 *
 * @dot
 * digraph neuron_example {
 *   rankdir=LR;
 *   node [shape=record];
 *
 *   InputA [label="Input A"];
 *   InputB [label="Input B"];
 *   Neuron0 [label="Neuron 0\n(NAND)"];
 *   Neuron1 [label="Neuron 1\n(OR)"];
 *   Neuron2 [label="Neuron 2\n(AND)"];
 *   Output [label="Output"];
 *
 *   InputA -> Neuron0;
 *   InputB -> Neuron0;
 *   InputA -> Neuron1;
 *   InputB -> Neuron1;
 *   Neuron0 -> Neuron2;
 *   Neuron1 -> Neuron2;
 *   Neuron2 -> Output;
 * }
 * @enddot
 * @code{.c}
 */
#include "ntcore.h"

#define NEURON_0 NAND
#define NEURON_1 OR
#define NEURON_2 AND

#define AND           .w= ( float[] ){ 1.0f , 1.0f } , .b= -1.5f
#define NAND          .w= ( float[] ){ -1.0f , -1.0f } , .b= 1.5f
#define OR            .w= ( float[] ){ 1.0f , 1.0f } , .b= -0.5f
#define NOR           .w= ( float[] ){ -1.0f , -1.0f } , .b= 0.5f
#define CONJUNCION_A  .w= ( float[] ){ -1.0f , 1.0f } , .b= -0.5f
#define CONJUNCION_B  .w= ( float[] ){ 1.0f , -1.0f } , .b= -0.5f
#define IMPLICATION_A .w= ( float[] ){ -1.0f , 1.0f } , .b= 0.5f
#define IMPLICATION_B .w= ( float[] ){ 1.0f , -1.0f } , .b= 0.5f

#include <stdio.h>

int main( void ) {
    float inputs[2];

    neuron_s neuron[]={
        [0]= { .inputs= sizeof( inputs ) / sizeof( float ) , .bff_idx= 0 , .in= ( float *[] ){ &inputs[0], &inputs[1] } , NEURON_0 , .fn= 0 , .out= 0.0f },
        [1]= { .inputs= sizeof( inputs ) / sizeof( float ) , .bff_idx= 0 , .in= neuron[0].in , NEURON_1 , .fn= 0 , .out= 0.0f },
        [2]= { .inputs= 2 , .bff_idx= 0 , .in= ( float *[] ){ &neuron[0].out, &neuron[1].out } , NEURON_2 , .fn= 0 , .out= 0.0f }
    };

    for( uint8_t i= 0 ; i < 4 ; i++ ){
        for( size_t j= 0 , f= sizeof( inputs ) / sizeof( float ) ; j < f ; j++ ) inputs[j]= !!( float )( i & ( 1 << j ) );
        for( size_t j= 0 , f= sizeof( neuron ) / sizeof( neuron_s ) ; j < f ; j++ ){
            float sum= neuron[j].b;
            for( uint16_t k= 0 ; k < neuron[j].inputs ; k++ ) sum+= *neuron[j].in[k] * neuron[j].w[k];
            neuron[j].out= (float)( sum >= 0.0f );
        }
        printf( "| input_A: %0.0f | input_B: %0.0f | output: %0.0f |\n", inputs[0] , inputs[1] , neuron[2].out );
    }

    return 0;
}
/** @endcode */