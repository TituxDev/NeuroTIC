/**
 * @file logic_gates.c
 * Author: titux
 * Date: 2025-11-21
 * Description: fixing array net_t out reading.
*/

/**
 * Pro tip: use logic_gates.h to store global constants, macros, variables,
 * or include additional headers. Keep your main clean.
 */
#include <stdio.h>
#include "ntcomplete.h"

int main( void ){
    net_s network={
        .inputs= 2,
        .layers= 2,
    };
    buildnet( newfeedforward( NEWNET( network , ( (uint16_t[]){ 3 , 16 } ) ) ) );
    for( uint16_t i= 0 ; i < network.layers ; i++ ) for( uint16_t j= 0 ; j < network.neurons[i] ; j++ ) network.nn[i][j].fn= NTACT_SIGMOID;
    randnet( &network );
    traindata_t data={
        .learning_rate= 0.9f,
        .tolerance= 0.9f,
        .max_attempts= 10000000,
        .samples= 4
    };
    newtraindata( &data , &network );
    for( uint64_t i= 0 ; i < data.samples ; i++ ){
        for( uint32_t j= 0 ; j < network.inputs ; j++ ) data.in[i][j]= ( i >> j ) & 1;
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
        data.results[i][11]= !data.in[i][0] || data.in[i][1];       // IMPB 1101
        data.results[i][12]= data.in[i][1];                         //  B   0011
        data.results[i][13]= data.in[i][0] || !data.in[i][1];       // IMPA 1011
        data.results[i][14]= data.in[i][0] || data.in[i][1];        //  OR  0111
        data.results[i][15]= 1;                                     // ALL  1111        
    }
    printf( "\nAttemps: %i" , backpropagation( &network , &data ) );
    printf( "\n\n=========================================================================================================================");
    printf( "\n| A | B | NULL |  NOR |  EXA | NOTB |  EXB | NOTA |  XOR | NAND |  AND | XNOR |   A  | IMPB |   B  | IMPA |  OR  |  ALL |" );
    printf( "\n|---|---|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|" );
    for( uint64_t i= 0 ; i < data.samples ; i++ ){
        for( uint32_t j= 0 ; j < network.inputs ; j++ ) network.in[j]= &data.in[i][j];
        feedforward( &network );
        printf( "\n| %.0f | %.0f |" , data.in[i][0] , data.in[i][1] );
        for( uint16_t j= 0 ; j < network.neurons[network.layers - 1] ; j++ ) printf( "   %.0f  |" , *network.out[j] );
    }
    printf( "\n=========================================================================================================================");
    savenet( &network , "logic_gates" );
    net_s network_copy= loadnet( "logic_gates" );
    printf( "\n\n=========================================================================================================================");
    printf( "\n| A | B | NULL |  NOR |  EXA | NOTB |  EXB | NOTA |  XOR | NAND |  AND | XNOR |   A  | IMPB |   B  | IMPA |  OR  |  ALL |" );
    printf( "\n|---|---|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|------|" );
    for( uint64_t i= 0 ; i < data.samples ; i++ ){
        for( uint32_t j= 0 ; j < network_copy.inputs ; j++ ) network_copy.in[j]= &data.in[i][j];
        feedforward( &network_copy );
        printf( "\n| %.0f | %.0f |" , data.in[i][0] , data.in[i][1] );
        for( uint16_t j= 0 ; j < network_copy.neurons[network_copy.layers - 1] ; j++ ) printf( "   %.0f  |" , *network_copy.out[j] );
    }
    printf( "\n=========================================================================================================================\n\n");
    remove( "logic_gates.ntic" );
    return 0;
}
