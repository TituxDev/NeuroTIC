/**
 * @file CLI.c
 * @brief Example: Interactively build a custom (non-feedforward) wiring descriptor via CLI.
 * @author Oscar Sotomayor (titux)
 * @date 2026
 *
 * This example demonstrates how to construct a `wiring_s` descriptor by hand, through
 * an interactive command-line prompt, instead of relying on `newfeedforward()` to
 * auto-generate a standard dense feedforward topology.
 *
 * For each layer (except the input layer) the user defines, per neuron input set:
 * - how many logical input arrays feed that set,
 * - the type of each array:
 *     - 'I' : the network's own inputs,
 *     - 'O' : the size of the output layer,
 *     - 'N' : a single reference to another layer/index's output,
 *     - 'M' : a manually built fan-in ("BFF") mixing individual N/I/O elements.
 *
 * This lets you describe arbitrary connectivity (skip connections, multiple input
 * buffers, mixed sources, etc.) that a plain feedforward generator can't express.
 *
 * Expected output:
 *
 * ```sh
 * ~/NeuroTIC/examples$ bash test.sh cli
 * Compiling project in location: examples
 * Project name: cli
 * Platform: CPU
 *
 * 31296 bytes written to examples/cli
 *
 * Network structure
 *   Inputs: 1
 *   Layers: 3
 *   Neurons per layer: 
 *   [0]: 1
 *   [1]: 1
 *   [2]: 4
 * Network topology
 * Layer 1
 *   Input sets: 1
 *   [0] Type (N/I/O/M): M
 *       BFF size: 1
 *       [0] Source type (N/I/O): N
 *           Source layer: 0
 *           Source index: 0
 * 
 *   Neuron: 
 *   [0]: 0
 * Layer 2
 *   Input sets: 4
 *   [0] Type (N/I/O/M): M
 *       BFF size: 3
 *       [0] Source type (N/I/O): N
 *           Source layer: 1
 *           Source index: 0
 *       [1] Source type (N/I/O): I
 *           Source index: 0
 *       [2] Source type (N/I/O): O
 *           Source index: 3
 *   [1] Type (N/I/O/M): N
 *       Source layer: 0
 *       Source BFF: 0
 *   [2] Type (N/I/O/M): I
 *   [3] Type (N/I/O/M): O

 *   Neuron: 
 *   [0]: 0
 *   [1]: 1
 *   [2]: 2
 *   [3]: 3
 * 
 * === ACTIVATION FUNCTIONS ===
 * 
 * Max activation function index: 4
 * Layer 0
 *   Neuron: 
 *   [0]: 1
 * Layer 1
 *   Neuron: 
 *   [0]: 1
 * Layer 2
 *   Neuron: 
 *   [0]: 1
 *   [1]: 1
 *   [2]: 1
 *   [3]: 1
 * 
 * === PARAMETERS ===
 * 
 * Layer 0
 *   Neuron:
 *   [0]:  bias=0.000000  w[0]=0.221714
 * Layer 1
 *   Neuron:
 *   [0]:  bias=0.000000  w[0]=0.315200
 * Layer 2
 *   Neuron:
 *   [0]:  bias=0.000000  w[0]=0.425050 w[1]=0.043631 w[2]=-0.738705
 *   [1]:  bias=0.000000  w[0]=0.447784
 *   [2]:  bias=0.000000  w[0]=-0.409675
 *   [3]:  bias=0.000000  w[0]=-0.280592 w[1]=0.610635 w[2]=-0.190924 w[3]=-0.107999
 * 
 * === I/O CHECK ===
 * 
 * Inputs connected: 1/1  [OK]
 * 
 * === FEEDFORWARD RUNS ===
 * 
 * Sample 0: in=[ 0 ] -> out=[ 0.557060 0.555740 0.500000 0.521883 ]
 * Sample 1: in=[ 1 ] -> out=[ 0.472325 0.561835 0.398990 0.519492 ]
 * 
 * === SAVE ===
 * 
 * Saved to CLI.ntic (160 bytes)
 * ```
 *
 * @code{.c}
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ntcomplete.h"

#define NETWORK_NAME CLI

// Compact, uniform section headers
#define SECTION(title) printf( "\n=== %s ===" , title )

int main( void ){
    
    
    net_s *NETWORK_NAME= &(net_s){ 0 };
    definestructure( NETWORK_NAME );
    definetopology( NETWORK_NAME );
    
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-result"
    // ---- Activation function selection per neuron ----
    SECTION( "ACTIVATION FUNCTIONS" );
    printf( "\n\nMax activation function index: %i\n" , NTACT_TOTAL_FUNCTIONS - 1 );
    for( layer_t i= 0 ; i < NETWORK_NAME->layers ; i++ ){
        printf( "Layer %u" , i );
        printf( "\n  Neuron: \n" );
        for( uint16_t j= 0 ; j < NETWORK_NAME->neurons[i] ; j++ ){
            printf( "  [%u]: " , j  );
            scanf( " %hhu" , &NETWORK_NAME->nn[i][j].fn );
            NETWORK_NAME->nn[i][j].fn= NETWORK_NAME->nn[i][j].fn < NTACT_TOTAL_FUNCTIONS ? NETWORK_NAME->nn[i][j].fn : 0;
        }
    }
    #pragma GCC diagnostic pop

    
    // ---- Initialized parameters (bias + weights), one line per neuron ----
    randnet( NETWORK_NAME );
    SECTION( "PARAMETERS" );
    putchar( '\n' );
    for( layer_t i= 0 ; i < NETWORK_NAME->layers ; i++ ){
        printf( "\nLayer %u" , i );
        printf( "\n  Neuron:" );
        for( uint16_t j= 0 ; j < NETWORK_NAME->neurons[i] ; j++ ){
            printf( "\n  [%u]:  bias=%f " , j , NETWORK_NAME->nn[i][j].b );
            for( input_t k= 0 ; k < NETWORK_NAME->nn[i][j].inputs ; k++ ) printf( " w[%u]=%f" , k , NETWORK_NAME->nn[i][j].w[k] );
        }
    }

    // ---- Connect network inputs ----
    data_t inputs[NETWORK_NAME->inputs];
    for( input_t i= 0 ; i < NETWORK_NAME->inputs ; i++ ) NETWORK_NAME->in[i]= &inputs[i];

    putchar( '\n' );
    SECTION( "I/O CHECK" );
    printf( "\n\nInputs connected: %u/%u  " , NETWORK_NAME->inputs , NETWORK_NAME->inputs );
    for( input_t i= 0 ; i < NETWORK_NAME->inputs ; i++ ) printf( "%s%s[%s]%s" ,
        i ? " " : "" ,
        NETWORK_NAME->in[i] == &inputs[i] ? "\033[0;32m" : "\033[0;31m" ,
        NETWORK_NAME->in[i] == &inputs[i] ? "OK" : "FAIL" ,
        "\033[0m"
    );

    // ---- Feedforward runs, one compact line per sample ----
    putchar( '\n' );
    SECTION( "FEEDFORWARD RUNS" );
    putchar( '\n' ); 
    uint16_t sample= 1;
    for( uint16_t i= 0 ; i < NETWORK_NAME->inputs ; i++ ) sample<<= 1;
    for( input_t i= 0 ; i < sample ; i++ ){
        printf( "\nSample %u: in=[ " , i );
        for( input_t j= 0 ; j < NETWORK_NAME->inputs ; j++ ) printf( "%.0f " , (float)(inputs[j]= (i >> j) & 1) );
        feedforward( NETWORK_NAME );
        printf( "] -> out=[ " );
        for( uint16_t j= 0 ; j < NETWORK_NAME->neurons[NETWORK_NAME->layers-1] ; j++ ) printf( "%f " , *NETWORK_NAME->out[j] );
        printf( "]" );
    }

    // ---- Save network ----
    savenet( NETWORK_NAME , "CLI" );
    FILE *fp= fopen( "CLI.ntic" , "rb" );
    fseek( fp , 0 , SEEK_END );
    putchar( '\n' );
    SECTION( "SAVE" );
    putchar( '\n' );
    printf( "\nSaved to CLI.ntic (%ld bytes)\n" , ftell( fp ) );
    fclose( fp );
    remove( "CLI.ntic" );

    putchar( '\n' );
    return 0;
}
/** @endcode */