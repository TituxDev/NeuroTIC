#ifndef NEUROTIC_H
#define NEUROTIC_H
/* LIBRARIES */
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
/* DEFININITIONS */
struct neuron{
    int inputs;
    float **IN;
    float OUT;
    float *W;
    float B;
    int FUNC;
};

struct net{
    int inputs;
    float **IN;
    int layers;
    int *neurons;
    struct neuron **N;
    float ***B;
    float **OUT;
};
/* BUILD */
struct net *build_net( struct net *Net , int *neurons ){
    Net->neurons= malloc( Net->inputs * sizeof( float ) );
    memcpy( Net->neurons , neurons , Net->layers * sizeof( int ) );
    Net->N= malloc( Net->layers * sizeof( struct neuron * ) );
    for( int i= 0 ; i < Net->layers ; i++ ) Net->N[i]=malloc( Net->neurons[i] * sizeof( struct neuron ) );
    Net->IN= malloc( Net->inputs * sizeof( float * ) );
    for( int i= 0 ; i < Net->neurons[0] ; i++ ){
        Net->N[0][i].inputs= Net->inputs;
        Net->N[0][i].IN= Net->IN;
        Net->N[0][i].W= malloc( Net->inputs * sizeof( float ) );
    }
    Net->B= malloc( ( Net->layers - 1 ) * sizeof( float ** ) );
    for( int i= 0 ; i < Net->layers - 1 ; i++ ){
        Net->B[i]= malloc( Net->neurons[i] * sizeof( float * ) );
        for( int j= 0 ; j < Net->neurons[i] ; j++ ) Net->B[i][j]= &Net->N[i][j].OUT;
    }
    for( int i= 1 ; i < Net->layers ; i++ ) for( int j= 0 ; j < Net->neurons[i] ; j++ ){
        Net->N[i][j].inputs= Net->neurons[i - 1];
        Net->N[i][j].IN= Net->B[i - 1];
        Net->N[i][j].W= malloc( Net->neurons[i - 1] * sizeof( float ) );
    }
    Net->OUT= malloc( Net->neurons[Net->layers -1] * sizeof( float * ) );
    for( int i= 0 ; i < Net->neurons[Net->layers - 1] ; i++ ) Net->OUT[i]= &Net->N[Net->layers - 1][i].OUT;
    return Net;
}
/* ACTIVATION */
enum functions{
    BOOLEAN,
    SIGMOID,

    TOTAL_FUNCTIONS
};
float boolean( float x ){
    return x >= 0;
}
float boolean_d( float x ){
    return 1;
}

float sigmoid( float x ){
    return 1 / ( 1 + exp( -x ) );
}
float sigmoid_d( float x ){
    x= sigmoid( x );
    return x * ( 1 - x );
}

float ( *activation[TOTAL_FUNCTIONS][2] )( float )={
    [BOOLEAN]= { boolean , boolean_d },
    [SIGMOID]= { sigmoid , sigmoid_d }
};

int rand_range[TOTAL_FUNCTIONS][2]={
    [BOOLEAN]= { -1 , 1 },
    [SIGMOID]= { -1 , 1 }
};
/* INITIALIZE */
void rand_net( struct net *Net ){
    srand( time( NULL ) );
    for( int i= 0, j, k ; i < Net->layers ; i++ ) for( j= 0 ; j < Net->neurons[i] ; j++ ){
        for( k= 0 ; k < Net->N[i][j].inputs ; k++ ) Net->N[i][j].W[k]= ( ( float )rand() / RAND_MAX ) * ( rand_range[Net->N[i][j].FUNC][1] - rand_range[Net->N[i][j].FUNC][0] ) + rand_range[Net->N[i][j].FUNC][0];
        Net->N[i][j].B= ( ( float )rand() / RAND_MAX ) * ( rand_range[Net->N[i][j].FUNC][1] - rand_range[Net->N[i][j].FUNC][0] ) + rand_range[Net->N[i][j].FUNC][0];
    }
}
/* CALTULATE */
float weighing( struct neuron N ){
    float w= N.B;
    for( int i= 0; i < N.inputs ; i++ ) w+= *N.IN[i] * N.W[i];
    return w;
}

float evaluate( struct neuron *N ){
   return N->OUT= activation[N->FUNC][0]( weighing( *N ) );
}

float *run_net( struct net *Net ){
    for( int i= 0, j ; i < Net->layers ; i++ ) for( j= 0 ; j < Net->neurons[i] ; j++ ) evaluate( &Net->N[i][j] );
    return *Net->OUT;
}
/* TRAINIG */
int train( struct net *Net , float **inputs , float **results , int samples , float learning_rate , float tolerance , int max_attepts ){
    int attempt= 0, max_mem= 0, inputs_size= Net->inputs * sizeof( float ), prev_layer= Net->layers - 1, next_layer, penu_layer= prev_layer - 1;
    for( int i= 0 ; i < Net->layers ; i++ ) max_mem= fmax( max_mem , Net->neurons[i] );
    max_mem*= sizeof( float );
    float err_total, *delta= malloc( max_mem ), *delta_h= malloc( max_mem ), *IN= malloc( inputs_size );
    for( int i= 0 ; i < Net->inputs ; i++ ) Net->IN[i]= &IN[i];
    do{
        err_total= 0;
        for( int i= 0, j, k, l ; i < samples ; i++ ){
            memcpy( IN , inputs[i] , inputs_size );
            run_net( Net );
            for( j= 0 ; j < Net->neurons[prev_layer] ; j++ ){
                err_total+= fabs( delta[j]= results[i][j] - *Net->OUT[j] );
                delta[j]*= activation[Net->N[prev_layer][j].FUNC][1]( weighing( Net->N[prev_layer][j]));
            }
            if( err_total < tolerance ) continue;
            for( j= penu_layer ; j >= 0 ; j-- ){
                next_layer= j + 1;
                memset( delta_h , 0 , max_mem );
                for( k= 0 ; k < Net->neurons[next_layer] ; k++ ) for( l= 0 ; l < Net->N[next_layer][k].inputs ; l++ ) delta_h[l]+= delta[k] * Net->N[next_layer][k].W[l];
                for( k= 0 ; k < Net->neurons[j] ; k++ ) delta_h[k]*= activation[Net->N[j][k].FUNC][1]( weighing( Net->N[j][k] ) );
                for( k= 0 ; k < Net->neurons[next_layer] ; k++ ){
                    for( l= 0 ; l < Net->N[next_layer][k].inputs ; l++ ) Net->N[next_layer][k].W[l]+= delta[k] * learning_rate * *Net->N[next_layer][k].IN[l];
                    Net->N[next_layer][k].B+= delta[k] * learning_rate;
                }
                memcpy( delta , delta_h , max_mem );
            }
            for( j= 0 ; j < Net->neurons[0] ; j++ ){
                for( k= 0 ; k < Net->N[0][j].inputs ; k++ ) Net->N[0][j].W[k]+= delta[j] * learning_rate * *Net->N[0][j].IN[k];
                Net->N[0][j].B+= delta[j] * learning_rate;
            }
        }
    } while( ++attempt < max_attepts && err_total > tolerance );
    free( delta );
    free( delta_h );
    free( IN );
    return attempt;
}
#endif
