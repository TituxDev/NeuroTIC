/*
 * Example usage of NeuroTIC - A neural network library by Oscar Sotomayor.
 * This example is provided under the terms of the Mozilla Public License v. 2.0.
 * See: https://mozilla.org/MPL/2.0/
 */

/* INSTRUCTIONS:
* This file is a simple example of how to use NeuroTIC.h V1.
* It shows how to train a neural network on logic functions.
* To change the logic function, select the corresponding macro
* and assign it to FUNCTION. You can also tweak the training
* parameters: learning_rate, error_tolerance, or max_attempts.
*
* Compile with math support: gcc logic_gates.c -lm
*
* The algorithm creates a network with 2 inputs, 2 input neurons,
* and 1 output neuron. The network will be trained according to
*  the logic function selected in FUNCTION.
*/
// CONTROL PANEL
#define FUNCTION XOR  // Logic function to train

#define AND           (data.in[i][0] && data.in[i][1])
#define NAND          (!AND)
#define OR            (data.in[i][0] || data.in[i][1])
#define NOR           (!OR)
#define CONJUNCION_A  (!data.in[i][0] && data.in[i][1])
#define CONJUNCION_B  (data.in[i][0] && !data.in[i][1])
#define IMPLICATION_A (!data.in[i][0] || data.in[i][1])
#define IMPLICATION_B (data.in[i][0] || !data.in[i][1])
#define XOR           (data.in[i][0] != data.in[i][1])
#define XNOR          (data.in[i][0] == data.in[i][1])

// === LIBRAIES ===
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ntcore.h"
#include "ntactivation.h"
#include "ntbuilder.h"
#include "ntinitialize.h"
#include "ntcalculate.h"
#include "nttrain.h"
#include "ntmemory.h"
#include "ntfeedforward.h"
#include "ntfile.h"

// === START ===
int main(){
// === NETWORK DEFINITION ===
    net_s Net= { .inputs= 2 , .layers= 2 };
    buildnet( newfeedforward( NEWNET( Net , ( (uint16_t []){ 2 , 1 }) ) ) );
    for( unsigned int i= 0 ; i < Net.layers ; i++ ) for( unsigned int j= 0 ; j < Net.neurons[i] ; j++ ) Net.nn[i][j].fn= NTACT_SIGMOID;
    randnet( &Net );
// === DATA SET DEFINITION ===
    traindata_t data={
        .samples=4,
        .learning_rate=0.2,
        .tolerance=0.5,
        .max_attempts=1000000
    };
    newtraindata( &data , &Net );
    for( unsigned int i= 0 ; i < data.samples ; i++ ){
        for( unsigned int j= 0 ; j < Net.inputs ; j++ ) data.in[i][j]= ( i >> j ) & 1;
        for( unsigned int j= 0 ; j < Net.neurons[Net.layers - 1] ; j++ ) data.results[i][j]= FUNCTION;
    }
// === TRAINING ===
    printf( "\nAttempts: %i\n" , backpropagation( &Net , &data ) );
    // === RESULTS ===
    printf( "\n| A | B | O |\n" );
    for( unsigned int i= 0 ; i < data.samples ; i++ ){
        for( unsigned int j= 0 ; j < Net.inputs ; j++ ) Net.in[j]= &data.in[i][j];
        printf( "| %.0f | %.0f | %.0f |\n" , data.in[i][0] , data.in[i][1] , feedforward( &Net )[0] );
    }
    printf( "\n" );
// === SAVE ===
    savenet( &Net , "examples/logic_gate" );
    net_s Net2= loadnet( "examples/logic_gate" );
    if( !Net2.inputs ) return 1;
    printf( "\n\n| A | B | O |\n" );
    for( unsigned int i= 0 ; i < data.samples ; i++ ){
        for( unsigned int j= 0 ; j < Net2.inputs ; j++ ) Net2.in[j]= &data.in[i][j];
        printf( "| %.0f | %.0f | %.0f |\n" , data.in[i][0] , data.in[i][1] , feedforward( &Net2 )[0] );
    }
    printf( "\n" );
// === END ===
    remove( "examples/logic_gate.ntic" );
    return 0;
}
