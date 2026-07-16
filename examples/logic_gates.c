/**
 * @file logic_gates.c
 * @brief Example: Train a neural network to compute all 16 two-input logic functions.
 * @author Oscar Sotomayor
 * @date 2026
 *
 * This example creates a 2-3-16 neural network and trains it to learn all 16 possible
 * two-input logic functions. It demonstrates network construction, training, evaluation,
 * and model serialization.
 * Expected output:
 *
 * Captured with the `#valgrind \` line in test.sh uncommented
 * (commented out by default).
 *
 * ```sh
 * ~/NeuroTIC/examples$ time bash test.sh logic_gates
 * Compiling project in location: examples
 * Project name: logic_gates
 * Platform: CPU
 *
 * 31320 bytes written to examples/logic_gates
 *
 * ==21562== Memcheck, a memory error detector
 * ==21562== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
 * ==21562== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
 * ==21562== Command: ./examples/logic_gates
 * ==21562== 
 *
 * Attempts: 567
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
 * Saved network size: 446 bytes
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
 * ==21562== 
 * ==21562== HEAP SUMMARY:
 * ==21562==     in use at exit: 0 bytes in 0 blocks
 * ==21562==   total heap usage: 181 allocs, 181 frees, 40,476 bytes allocated
 * ==21562== 
 * ==21562== All heap blocks were freed -- no leaks are possible
 * ==21562== 
 * ==21562== For lists of detected and suppressed errors, rerun with: -s
 * ==21562== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
 *
 * real    0m1.143s
 * user    0m0.886s
 * sys     0m0.262s
 * ```
 * 
 * @code{.c}
 */

#include <stdio.h>
#include <string.h>
#include "ntcomplete.h"

int main( void ){

// Network structure: 2 inputs, 2 layers (one hidden layer with 3 neurons and output layer with 16 neurons)
    net_s network={
        .inputs= 2,
        .layers= 2,
    };
    newnet( &network , (uint16_t []){3,16} , network.layers );
    newfeedforward( &network );
    buildnet( &network );
// Set activation functions to sigmoid for all neurons: Following activation functions distribution had shown the best efficiency equilibrium between training attempts, convergenece and computational work, in BOOLEAN vs SIGMOID tests.
    network.nn[0][0].fn= NTACT_BOOLEAN; //<- First hiden neuron.
    for( uint16_t j= 1 ; j < network.neurons[0] ; j++ ) network.nn[0][j].fn= NTACT_SIGMOID;
    for( uint16_t j= 0 ; j < network.neurons[1] ; j++ ) network.nn[1][j].fn= NTACT_BOOLEAN;

// Initialize weights randomly
    randnet( &network );

// Prepare training data for all 16 two-input logic functions
    traindata_t data={
        .learning_rate= 0.1,
        .tolerance= 0.0,
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
    printf( "\nAttempts: %li" , backpropagation( &network , &data ) );

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
    printf( "\n\nSaved network size: %li bytes" , savenet( &network , "logic_gates" ) );
    deleteowner( &network );
    memset( &network , 0 , sizeof( net_s ) );
    loadnet( &network , "logic_gates" );

// Display results from the loaded network to verify persistence
    printf( "\n\n=========================================================================================================================" );
    printf( "\n| A | B | NULL |  NOR |  EXA | NOTB |  EXB | NOTA |  XOR | NAND |  AND | XNOR |   A  | IMPA |   B  | IMPB |  OR  |  ALL |" );
    printf( "\n|---|---|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|" );
    for( sample_t i= 0 ; i < data.samples ; i++ ){
        for( input_t j= 0 ; j < network.inputs ; j++ ) network.in[j]= &data.in[i][j];
        feedforward( &network );
        printf( "\n| %.0f | %.0f |" , data.in[i][0] , data.in[i][1] );
        for( uint16_t j= 0 ; j < network.neurons[network.layers - 1] ; j++ ) printf( "   %.0f  |" , *network.out[j] );
    }
    printf( "\n=========================================================================================================================\n\n");

// Clean up
    remove( "logic_gates.ntic" );
    return 0;
}
/** @endcode */