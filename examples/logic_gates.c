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
float learning_rate    = 0.1;
float error_tolerance  = 0.01;
int max_attempts       = 10000;

#define AND           (table[i][0] && table[i][1])
#define NAND          (!(AND))
#define OR            (table[i][0] || table[i][1])
#define NOR           (!(OR))
#define CONJUNCION_A  (!AND)
#define CONJUNCION_B  (table[i][0] && !table[i][1])
#define IMPLICATION_A (!(table[i][0] || table[i][1]))
#define IMPLICATION_B (table[i][0] || !table[i][1])
#define XOR           (table[i][0] != table[i][1])
#define XNOR          (table[i][0] == table[i][1])

// === LIBRAIES ===
#include <stdio.h>
#include <stdlib.h>
#include "NTIC_neurotic.h"

// === START ===
int main(){
// === NETWORK DEFINITION ===
    struct net Net= define_net( 2 , 2 , ( int []){ 2 ,  1 } );
    build_net( &Net );
// === INITIALIZATION ===
    for( int i= 0 ; i < Net.layers ; i++ ) for( int j= 0 ; j < Net.neurons[i] ; j++ ) Net.N[i][j].FUNC= SIGMOID;
    rand_net( &Net );
// === DATA SET DEFINITION ===
    int samples= 4;
    float **table= calloc( samples , sizeof( float * ) );
    float **results= calloc( samples , sizeof( float * ) );
    for( int i= 0 ; i < samples ; i++ ){
        table[i]= calloc( Net.inputs , sizeof( float ) );
        results[i]= calloc( Net.neurons[Net.layers - 1] , sizeof( float ) );
    }
    for( int i= 0 ; i < samples ; i++ ) for( int j= 0 ; j < Net.inputs ; j++ ) table[i][j]= ( i >> j ) & 1;
    for( int i= 0 ; i < samples ; i++ ) for( int j= 0 ; j < Net.neurons[Net.layers - 1] ; j++ ) results[i][j]= FUNCTION;
// === TRAINING ===
    printf( "\nAttemps: %i\n" , train( &Net , table , results , samples, learning_rate , error_tolerance , max_attempts ) );
// === RESULTS ===
    printf( "\n| A | B | O |\n" );
    for( int i= 0 ; i < samples ; i++ ){
        for( int j= 0 ; j < Net.inputs ; j++ ) Net.IN[j]= &table[i][j];
        printf( "| %.0f | %.0f | %.0f |\n" , table[i][0] , table[i][1] , run_net( &Net )[0] );
    }
    printf( "\n" );
// === FREE MEMORY ===
    for( int i= 0 ; i < Net.layers ; i++ ){
        for( int j= 0 ; j < Net.neurons[i] ; j++ ) free( Net.N[i][j].W );
        free( Net.N[i][0].IN );
        free( Net.N[i] );
    }
    free( Net.N );
    free( Net.OUT );
    free( Net.B );
    for( int i= 0 ; i < samples ; i ++ ){
        free( table[i] );
        free( results[i] );
    }
    free( table );
    free( results );
// === END ===
    return 0;
}
