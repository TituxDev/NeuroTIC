/**
 * @file logic_gates.c
 * @brief Example: Train a neural network to compute all 16 two-input logic functions.
 * @author Oscar Sotomayor (Titux)
 * @date 2026
 *
 * This example creates a 3-16 neural network and trains it to learn all 16 possible
 * two-input logic functions. It demonstrates network construction, training, evaluation,
 * and model serialization.
 * Expected output:
 *
 * ```sh
 * ~/NeuroTIC/examples$ time bash test.sh logic_gates
 * Compiling project in location: examples
 * Project name: logic_gates
 * Platform: CPU
 *
 * Attemps: 7438254
 *
 * =========================================================================================================================
 * | A | B | NULL |  NOR |  EXA | NOTB |  EXB | NOTA |  XOR | NAND |  AND | XNOR |   A  | IMPA |   B  | IMPB |  OR  |  ALL |
 * |---|---|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|
 * | 0 | 0 |   0  |   1  |   0  |   1  |   0  |   1  |   0  |   1  |   0  |   1  |   0  |   1  |   0  |   1  |   0  |   1  |
 * | 1 | 0 |   0  |   0  |   1  |   1  |   0  |   0  |   1  |   1  |   0  |   0  |   1  |   1  |   0  |   0  |   1  |   1  |
 * | 0 | 1 |   0  |   0  |   0  |   0  |   1  |   1  |   1  |   1  |   0  |   0  |   0  |   0  |   1  |   1  |   1  |   1  |
 * | 1 | 1 |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   1  |   1  |   1  |   1  |   1  |   1  |   1  |   1  |
 * =========================================================================================================================
 *
 * =========================================================================================================================
 * | A | B | NULL |  NOR |  EXA | NOTB |  EXB | NOTA |  XOR | NAND |  AND | XNOR |   A  | IMPA |   B  | IMPB |  OR  |  ALL |
 * |---|---|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|
 * | 0 | 0 |   0  |   1  |   0  |   1  |   0  |   1  |   0  |   1  |   0  |   1  |   0  |   1  |   0  |   1  |   0  |   1  |
 * | 1 | 0 |   0  |   0  |   1  |   1  |   0  |   0  |   1  |   1  |   0  |   0  |   1  |   1  |   0  |   0  |   1  |   1  |
 * | 0 | 1 |   0  |   0  |   0  |   0  |   1  |   1  |   1  |   1  |   0  |   0  |   0  |   0  |   1  |   1  |   1  |   1  |
 * | 1 | 1 |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   1  |   1  |   1  |   1  |   1  |   1  |   1  |   1  |
 * =========================================================================================================================
 *
 * 
 * real    0m10.910s
 * user    0m10.728s
 * sys     0m0.183s
 * ```
 * 
 * @code{.c}
 */

#include <stdio.h>
#include "ntcomplete.h"

int main( void ){

// Network structure: 2 inputs, 2 layers (one hidden layer with 3 neurons and output layer with 16 neurons)
    CREATE_NET_FEEDFORWARD( network , 2 , ((uint16_t []){3,16}) );

// Set activation functions to sigmoid for all neurons
    for( layer_t i= 0 ; i < network.layers ; i++ ) for( uint16_t j= 0 ; j < network.neurons[i] ; j++ ) network.nn[i][j].fn= NTACT_SIGMOID;

// Initialize weights randomly
    randnet( &network );

// Prepare training data for all 16 two-input logic functions
    traindata_t data={
        .learning_rate= (precision_t)(2.5),
        .tolerance= (precision_t)(0.49),
        .max_attempts= 10000000,
        .samples= 4
    };
// Allocate training data
    newtraindata( &data , &network );
// Define input-output pairs for all logic functions
    for( uint64_t i= 0 ; i < data.samples ; i++ ){
        for( input_t j= 0 ; j < network.inputs ; j++ ) data.in[i][j]= ( i >> j ) & 1;
        data.results[i][0]= 0;                                      // NULL 0000
        data.results[i][1]= !( data.in[i][0] || data.in[i][1] );    // NOR  1000
        data.results[i][2]= data.in[i][0] && !data.in[i][1];        // EXA  0100
        data.results[i][3]= !data.in[i][1];                         // NOTB 1100
        data.results[i][4]= !data.in[i][0] && data.in[i][1];        // EXB  0010
        data.results[i][5]= !data.in[i][0];                         // NOTA 1010
        data.results[i][6]= data.in[i][0] != data.in[i][1];         // XOR  0110
        data.results[i][7]= !( data.in[i][0] && data.in[i][1] );    // NAND 1110
        data.results[i][8]= data.in[i][0] && data.in[i][1];         // AND  0001
        data.results[i][9]= data.in[i][0] == data.in[i][1];         // XNOR 1001
        data.results[i][10]= data.in[i][0];                         //  A   0101
        data.results[i][11]= data.in[i][0] || !data.in[i][1];       // IMPB 1101
        data.results[i][12]= data.in[i][1];                         //  B   0011
        data.results[i][13]= !data.in[i][0] || data.in[i][1];       // IMPA 1011
        data.results[i][14]= data.in[i][0] || data.in[i][1];        //  OR  0111
        data.results[i][15]= 1;                                     // ALL  1111
    }

// Train the network using backpropagation
    printf( "\nAttemps: %li" , backpropagation( &network , &data ) );

// Evaluate and display results
    printf( "\n\n=========================================================================================================================" );
    printf( "\n| A | B | NULL |  NOR |  EXA | NOTB |  EXB | NOTA |  XOR | NAND |  AND | XNOR |   A  | IMPA |   B  | IMPB |  OR  |  ALL |" );
    printf( "\n|---|---|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|" );
    for( sample_t i= 0 ; i < data.samples ; i++ ){
        for( input_t j= 0 ; j < network.inputs ; j++ ) network.in[j]= &data.in[i][j];
        feedforward( &network );
        printf( "\n| %.0f | %.0f |" , data.in[i][0] , data.in[i][1] );
        for( uint16_t j= 0 ; j < network.neurons[network.layers - 1] ; j++ ) printf( "   %.0f  |" , *network.out[j] );
    }
    printf( "\n=========================================================================================================================");

// Save and reload the trained network
    savenet( &network , "logic_gates" );
    net_s network_copy= loadnet( "logic_gates" );

// Display results from the loaded network to verify persistence
    printf( "\n\n=========================================================================================================================" );
    printf( "\n| A | B | NULL |  NOR |  EXA | NOTB |  EXB | NOTA |  XOR | NAND |  AND | XNOR |   A  | IMPA |   B  | IMPB |  OR  |  ALL |" );
    printf( "\n|---|---|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|" );
    for( sample_t i= 0 ; i < data.samples ; i++ ){
        for( input_t j= 0 ; j < network_copy.inputs ; j++ ) network_copy.in[j]= &data.in[i][j];
        feedforward( &network_copy );
        printf( "\n| %.0f | %.0f |" , data.in[i][0] , data.in[i][1] );
        for( uint16_t j= 0 ; j < network_copy.neurons[network_copy.layers - 1] ; j++ ) printf( "   %.0f  |" , *network_copy.out[j] );
    }
    printf( "\n=========================================================================================================================\n\n");

// Clean up
    remove( "logic_gates.ntic" );
    return 0;
}
/** @endcode */