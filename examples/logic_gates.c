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

#define AND           (data.IN[i][0] && data.IN[i][1])
#define NAND          (!(AND))
#define OR            (data.IN[i][0] || data.IN[i][1])
#define NOR           (!(OR))
#define CONJUNCION_A  (!AND)
#define CONJUNCION_B  (data.IN[i][0] && !data.IN[i][1])
#define IMPLICATION_A (!OR)
#define IMPLICATION_B (data.IN[i][0] || !data.IN[i][1])
#define XOR           (data.IN[i][0] != data.IN[i][1])
#define XNOR          (data.IN[i][0] == data.IN[i][1])

// === LIBRAIES ===
#include <stdio.h>
#include <stdlib.h>
#include "NTIC_complete.h"

// === START ===
int main(){
// === NETWORK DEFINITION ===
    struct net Net= define_net( 2 , 2 , ( int [] ){ 2 ,  1 } );
    build_net( &Net );
    for( unsigned int i= 0 ; i < Net.layers ; i++ ) for( unsigned int j= 0 ; j < Net.neurons[i] ; j++ ) Net.N[i][j].FUNC= SIGMOID;
// === INITIALIZATION ===
    rand_net( &Net );
// === DATA SET DEFINITION ===
    struct train_data data={
        .samples=4,
        .learning_rate=0.1,
        .tolerance=0.0,
        .max_attempts=10000
    };
    traininit( &data , Net );
    for( unsigned int i= 0, j ; i < data.samples ; i++ ){
        for( unsigned int j= 0 ; j < Net.inputs ; j++ ) data.IN[i][j]= ( i >> j ) & 1;
        for( unsigned int j= 0 ; j < Net.neurons[Net.layers - 1] ; j++ ) data.results[i][j]= FUNCTION;
    }
// === TRAINING ===
    printf( "\nAttempts: %i\n" , train( &Net , data ) );
// === RESULTS ===
    printf( "\n| A | B | O |\n" );
    for( unsigned int i= 0 ; i < data.samples ; i++ ){
        for( unsigned int j= 0 ; j < Net.inputs ; j++ ) Net.IN[j]= &data.IN[i][j];
        printf( "| %.0f | %.0f | %.0f |\n" , data.IN[i][0] , data.IN[i][1] , run_net( &Net )[0] );
    }
    printf( "\n" );
// === END ===
    return 0;
}
