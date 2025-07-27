/* BIBLIOTECAS */
#include <stdio.h>
#include "NeuroTIC.h"
/* INCIO PROGRAMA */
int main(){
/* CONSTRUCCION */
    struct net Net={
        .inputs= 2,
        .layers= 2
    };
    int neurons[]={ 2 , 1 };
    build_net( &Net , neurons );
/* INICIALIZACION */
    for( int i= 0 ; i < Net.layers ; i++ ) for( int j= 0 ; j < Net.neurons[i] ; j++ ) Net.N[i][j].FUNC= SIGMOID;
    rand_net( &Net );
/* MUESTRAS */
    int samples= 4;
    float **tabla= calloc( samples , sizeof( float * ) );
    float **resultado= calloc( samples , sizeof( float * ) );
    for( int i= 0 ; i < samples ; i++ ){
        tabla[i]= calloc( Net.inputs , sizeof( float ) );
        resultado[i]= calloc( Net.neurons[Net.layers - 1] , sizeof( float ) );
    }
    for( int i= 0 ; i < samples ; i++ ) for( int j= 0 ; j < Net.inputs ; j++ ) tabla[i][j]= ( i >> j ) & 1;
    for( int i= 0 ; i < samples ; i++ ) for( int j= 0 ; j < Net.neurons[Net.layers - 1] ; j++ ) resultado[i][j]= tabla[i][0] != tabla[i][1];
/* ENTRENAMIENTO */
    printf( "\nEntrenamiento Intentos: %i\n" , train( &Net , tabla , resultado , samples, 0.1 , 0.1 , 1000000 ) );
/* CALCULAR E IMPRIMIR */
    printf( "\n\n| A | B | S |\n" );
    for( int i= 0 ; i < samples ; i++ ){
        for( int j= 0 ; j < Net.inputs ; j++ ) Net.IN[j]= &tabla[i][j];
        printf( "| %.0f | %.0f | %.0f |\n" , tabla[i][0] , tabla[i][1] , run_net( &Net )[0] );
    }
    printf( "\n" );
/* TERMINAR PROGRAMA */
    for( int i= 0 ; i < Net.layers ; i++ ){
        for( int j= 0 ; j < Net.neurons[i] ; j++ ) free( Net.N[i][j].W );
        free( Net.N[i][0].IN );
        free( Net.N[i] );
    }
    free( Net.N );
    free( Net.OUT );
    free( Net.B );
    for( int i= 0 ; i < samples ; i ++ ){
        free( tabla[i] );
        free( resultado[i] );
    }
    free( tabla );
    free( resultado );
    return 0;
}
