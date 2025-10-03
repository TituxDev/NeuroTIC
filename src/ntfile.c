#include "ntfile.h"
#include "ntbuilder.h"
#include "ntmemory.h"
#include "ntbuilder.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define NAME_LENGTH 30

#define STRICT_LE16( x ) &( uint16_t ){ little_endian ? ( x ) : bswap16( x )}
#define STRICT_LE32( x ) &( uint32_t ){ little_endian ? ( x ) : bswap32( x )}

uint8_t checkendian( void ){
    uint16_t x= 1;
    return *(uint8_t *)&x;
}
uint16_t bswap16( uint16_t x ){
    return ( x >> 8 ) | ( x << 8 );
}
uint32_t bswap32( uint32_t x ){
    return
        ( ( x >> 24 ) & 0xFF ) |
        ( ( x >> 8 ) & 0xFF00 ) |
        ( ( x << 8 ) & 0xFF0000 ) |
        ( ( x << 24 ) & 0xFF000000 );
}

uint32_t float32( float x ){
    if( x == 0.0f ) return 0;
    uint32_t result= 0;
    uint32_t mantissa= 0;
    uint8_t exponent= 127;
    if( x < 0.0f ) result= 1;
    result<<= 8;
    x= fabsf( x );
    exponent+= ( int8_t )floorf( log2f( x ) );
    result|= exponent;
    result<<= 23;
    float frac= x / powf( 2.0f , exponent - 127 ) - 1.0f;
    mantissa= ( uint32_t ) (frac * (1 << 23 ) ) & 0x7FFFFF;
    result |= mantissa;
    return result;
}
float floatsys(uint32_t x){
    if (x == 0) return 0.0f;

    uint8_t sign = (x >> 31) & 1;
    uint8_t exponent = (x >> 23) & 0xFF;
    uint32_t mantissa = x & 0x7FFFFF;

    // Manejo de NaN e Inf
    if (exponent == 255) return NAN; 

    // Cero denormalizado
    if (exponent == 0) return sign ? -0.0f : 0.0f;

    exponent -= 127; // Ajuste del sesgo
    float result = (float)mantissa / (1 << 23); // Normalización de la mantisa
    result += 1.0f; // Agregar el 1 implícito
    result *= powf(2.0f, exponent);

    return sign ? -result : result;
}

unsigned char savenet( net_t * net , const char *name ){
    char NAME[ NAME_LENGTH ];
    strcpy( NAME , name );
    strcat( NAME , ".ntic" );

    FILE *fp= fopen( NAME , "wb" );

    fprintf( fp , "NeuroTIC%c" , '\0' );

    fwrite( &net->inputs , sizeof( uint32_t ) , 1 , fp );
    fwrite( &net->layers , sizeof( uint16_t ) , 1 , fp );
    fwrite( net->neurons , sizeof( uint16_t ) , net->layers , fp );

    for( uint16_t i= 0 ; i < net->layers ; i ++ ) for( uint16_t j= 0 ; j < net->neurons[i] ; j++ ){
        fwrite( &net->nn[i][j].inputs , sizeof( uint32_t ) , 1 , fp );
        fwrite( &net->nn[i][j].bff_idx , sizeof( uint16_t ) , 1 , fp );
    }

    if( net->layers > 1 ) for( uint16_t i= 0 ; i < net->layers - 1 ; i++ ){
        fwrite( &net->bff_wiring[i].arrays , sizeof( uint16_t ) , 1 , fp );
        for( uint16_t j= 0 ; j < net->bff_wiring[i].arrays ; j++ ){
            fwrite( &net->bff_wiring[i].array_type[j] , sizeof( uint8_t ) , 1 , fp );
            switch( net->bff_wiring[i].array_type[j] ){
                case 'M':
                    fwrite( &net->bff_wiring[i].size[j] , sizeof( uint32_t ) , 1 , fp );
                    for( uint32_t k= 0 ; k < net->bff_wiring[i].size[j] ; k++ ){
                        fwrite( &net->bff_wiring[i].src_type[j][k] , sizeof( uint8_t ) , 1 , fp );
                        switch( net->bff_wiring[i].src_type[j][k] ){
                        case 'N':
                            fwrite( &net->bff_wiring[i].src_layer[j][k] , sizeof( uint16_t ) , 1 , fp );
                            fwrite( &net->bff_wiring[i].src_index[j][k] , sizeof( uint16_t ) , 1 , fp );
                            break;
                        case 'O':
                        case 'I':
                            fwrite( &net->bff_wiring[i].src_index[j][k] , sizeof( uint16_t ) , 1 , fp );
                        }
                    }
                    break;
                case 'N':
                    fwrite( &net->bff_wiring[i].src_layer[j][0] , sizeof( uint16_t ) , 1 , fp );
                    fwrite( &net->bff_wiring[i].src_index[j][0] , sizeof( uint16_t ) , 1 , fp );
                    break;
            }
        }
    }

    for( uint16_t i= 0 ; i < net->layers ; i ++ ) for( uint16_t j= 0 ; j < net->neurons[i] ; j++ ){
        fwrite( &net->nn[i][j].fn , sizeof( uint8_t ) , 1 , fp );
        fwrite( &net->nn[i][j].b , sizeof( float ) , 1 , fp );
        for( uint32_t k= 0 ; k < net->nn[i][j].inputs ; k++ ) fwrite( &net->nn[i][j].w[k] , sizeof( float ) , 1 , fp );
    }

    fclose( fp );
    return 0;
}

struct ntnet loadnet( char *name ){
    char NAME[ NAME_LENGTH ];
    strcpy( NAME , name );
    strcat( NAME , ".ntic" );
    FILE *fp= fopen( NAME , "rb" );

    fseek( fp , 9 , SEEK_SET );

    net_t net;
    size_t aux;
    aux= fread( &net.inputs , sizeof( uint32_t ) , 1 , fp );
    aux= fread( &net.layers , sizeof( uint16_t ) , 1 , fp );

    uint16_t *neurons;
    neurons= malloc( net.layers * sizeof( uint16_t ) );
    aux= fread( neurons , sizeof( uint16_t ) , net.layers , fp );
    newnet( &net , neurons , net.layers );
    free( neurons );

    for( uint16_t i= 0 ; i < net.layers ; i ++ ) for( uint16_t j= 0 ; j < net.neurons[i] ; j++ ){
        aux= fread( &net.nn[i][j].inputs , sizeof( uint32_t ) , 1 , fp );
        aux= fread( &net.nn[i][j].bff_idx , sizeof( uint16_t ) , 1 , fp );
    }

    if( net.layers > 1 ){
        memtrack( net.bff_wiring= malloc( ( net.layers - 1 ) * sizeof( bffwiring_t ) ) );
        for( uint16_t i= 0 ; i < net.layers - 1 ; i++ ){
            aux= fread( &net.bff_wiring[i].arrays , sizeof( uint16_t ) , 1 , fp );
            memtrack( net.bff_wiring[i].array_type= malloc( net.bff_wiring[i].arrays * sizeof( uint8_t ) ) );
            memtrack( net.bff_wiring[i].size= malloc( net.bff_wiring[i].arrays * sizeof( uint32_t ) ) );
            memtrack( net.bff_wiring[i].src_type= malloc( net.bff_wiring[i].arrays * sizeof( uint8_t * ) ) );
            memtrack( net.bff_wiring[i].src_layer= malloc( net.bff_wiring[i].arrays * sizeof( uint16_t * ) ) );
            memtrack( net.bff_wiring[i].src_index= malloc( net.bff_wiring[i].arrays * sizeof( uint16_t * ) ) );
            for( uint16_t j= 0 ; j < net.bff_wiring[i].arrays ; j++ ){
                aux= fread( &net.bff_wiring[i].array_type[j] , sizeof( uint8_t ) , 1 , fp );
                switch( net.bff_wiring[i].array_type[j] ){
                    case 'M':
                        aux= fread( &net.bff_wiring[i].size[j] , sizeof( uint32_t ) , 1 , fp );
                        memtrack( net.bff_wiring[i].src_type[j]= malloc( net.bff_wiring[i].size[j] * sizeof( uint8_t ) ) );
                        memtrack( net.bff_wiring[i].src_layer[j]= malloc( net.bff_wiring[i].size[j] * sizeof( uint16_t ) ) );
                        memtrack( net.bff_wiring[i].src_index[j]= malloc( net.bff_wiring[i].size[j] * sizeof( uint16_t ) ) );
                        for( uint32_t k= 0 ; k < net.bff_wiring[i].size[j] ; k++ ){
                            aux= fread( &net.bff_wiring[i].src_type[j][k] , sizeof( uint8_t ) , 1 , fp );
                            switch( net.bff_wiring[i].src_type[j][k] ){
                                case 'N':
                                    aux= fread( &net.bff_wiring[i].src_layer[j][k] , sizeof( uint16_t ) , 1 , fp );
                                    aux= fread( &net.bff_wiring[i].src_index[j][k] , sizeof( uint16_t ) , 1 , fp );
                                    break;
                                case 'O':
                                case 'I':
                                    aux= fread( &net.bff_wiring[i].src_index[j][k] , sizeof( uint16_t ) , 1 , fp );
                            }
                        }
                        break;
                    case 'N':
                        memtrack( net.bff_wiring[i].src_type[j]= malloc( sizeof( uint8_t ) ) );
                        memtrack( net.bff_wiring[i].src_layer[j]= malloc( sizeof( uint16_t ) ) );
                        memtrack( net.bff_wiring[i].src_index[j]= malloc( sizeof( uint16_t ) ) );
                        aux= fread( &net.bff_wiring[i].src_layer[j][0] , sizeof( uint16_t ) , 1 , fp );
                        aux= fread( &net.bff_wiring[i].src_index[j][0] , sizeof( uint16_t ) , 1 , fp );
                        break;
                }
            }
        }
    }

    buildnet( &net );

    for( uint16_t i= 0 ; i < net.layers ; i ++ ) for( uint16_t j= 0 ; j < net.neurons[i] ; j++ ){
        aux= fread( &net.nn[i][j].fn , sizeof( uint8_t ) , 1 , fp );
        aux= fread( &net.nn[i][j].b , sizeof( float ) , 1 , fp );
        for( uint32_t k= 0 ; k < net.nn[i][j].inputs ; k++ ) aux= fread( &net.nn[i][j].w[k] , sizeof( float ) , 1 , fp );
    }
    fclose( fp );
    return net;
}