/**
 * @file ntfile.c
 * @brief Implementation of network saving and loading for NeuroTIC.
 * 
 * Provides functions to persist network structures and weights, allowing training to be stored and reloaded.  
 * The file format is binary with a custom structure, including endianness and floating-point representation handling for cross-platform compatibility.  
 * Currently, the implementation focuses on core data serialization and deserialization, with future plans for validation and standardization of loaded data.
 * 
 * @todo Implement file validation and data standardization during loading to ensure compatibility and integrity of loaded networks.
 * @todo Consider adding support for versioning in the file format to allow for future extensions and backward compatibility.
 * @todo Explore options for compressing the saved network files to reduce disk space usage, especially for larger networks.
 * 
 * @author Oscar Sotomayor
 * @date 2026
 */

#include "ntfile.h"
#include "ntbuilder.h"
#include "ntmemory.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>

#define NAME_LENGTH 30
#define MAGIC "NeuroTIC"
#define VERSION 0x0

/**
 * @name Endianness and Floating-Point Handling
 * 
 * The STRICT_LE16 and STRICT_LE32 macros ensure that data is written in little-endian format, regardless of the host system's endianness.  
 * The checkendian function detects the system's endianness, while bswap16 and bswap32 perform byte swapping for 16-bit and 32-bit integers, respectively.  
 * The isieee754 function checks if the system uses IEEE 754 floating-point representation, and the float32 and floatsys functions convert between
 * float and uint32_t representations based on the IEEE 754 standard or a custom format if not supported.
 * 
 * @code
 */
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

uint8_t isieee754( void ){
    return
        ( sizeof( float ) == 4) &
        ( FLT_RADIX == 2 ) &
        ( FLT_MANT_DIG == 24 ) &
        ( FLT_MAX_EXP == 128 );
}
typedef struct{
    uint8_t sign;
    int32_t e;
    float m;
} float_parts_s;
uint32_t float32( float x , uint8_t ieee754 ){
    uint32_t result;
    float_parts_s float_parts= {0};
    if( ieee754 ){
        memcpy( &result , &x , sizeof( float ) );
        return result;
    }
    float_parts.sign= x < 0.0f;
    if( ( x= fabsf( x ) ) == 0.0f ) return 0;
    float_parts.m= frexpf( x , &float_parts.e );
    if( float_parts.e <= -126 ) return ( float_parts.sign << 31 );
    if( float_parts.e >= 129 ) return ( float_parts.sign << 31 ) | ( 0xFF << 23 );
    return ( float_parts.sign << 31 ) |
           ( ( uint32_t )( float_parts.e + 126 ) << 23 ) |
           ( ( uint32_t )( ( float_parts.m * 2.0f - 1.0f ) * ( 1u << 23 ) + 0.5f ) & 0x7FFFFF );
}
float floatsys( int32_t x , uint8_t ieee754 ){
    float result;
    if( ieee754 ){
        memcpy( &result , &x , sizeof( float ) );
        return result;
    }
    float_parts_s float_parts= { .sign= ( x >> 31 ) & 1 , .e= ( x >> 23 ) & 0xFF , .m= ( float )( x & 0x7FFFFF ) / ( 1u << 23 ) };
    if( float_parts.e == 0 && ( x & 0x7FFFFF ) == 0 ) return 0.0f;
    if( float_parts.e == 0 ) float_parts.e= -126;
    else {
        float_parts.m+= 1.0f;
        float_parts.e-= 127;
    }
    result= ldexpf( float_parts.m , float_parts.e );
    return float_parts.sign ? -result : result;
}
/** @endcode */

/**
 * @details
 * Saves the network's layers, neurons, weights, biases, and buffer wiring to a binary file.  
 * The file is structured with a header containing a magic string and version byte, followed by the network's core data.  
 * The function handles endianness and floating-point representation to ensure cross-platform compatibility.  
 * 
 * @todo Add validation checks for the input network structure before attempting to save, ensuring that all necessary data is present and correctly formatted.  
 * @todo Implement error handling for file operations, such as checking for write permissions and handling disk space issues.  
 * @todo Consider adding metadata to the file format, such as timestamps or training information, to provide more context when loading networks in the future.  
 * @todo Explore options for compressing the saved network files to reduce disk space usage, especially for larger networks.  
 * @todo Implement file validation and data standardization during loading to ensure compatibility and integrity of loaded networks.  
 */
unsigned char savenet( net_s * net , const char *name ){
    uint8_t little_endian= checkendian( ) , ieee754= isieee754( );
    char NAME[ NAME_LENGTH ];
    if( snprintf( NAME , sizeof( NAME ) , "%s%s" , name , ".ntic" ) >= ( int )sizeof( NAME ) ) return 1;
    FILE *fp= fopen( NAME , "wb" );
    if( fp == NULL ) return 2;
    fprintf( fp , "%s%c" , MAGIC , VERSION );
    fwrite( STRICT_LE32( net->inputs ) , sizeof( input_t ) , 1 , fp );
    fwrite( STRICT_LE32( net->layers ) , sizeof( layer_t ) , 1 , fp );
    for( layer_t i= 0 ; i < net->layers ; i ++ ) fwrite( STRICT_LE16( net->neurons[i] ) , sizeof( uint16_t ) , 1 , fp );
    for( layer_t i= 0 ; i < net->layers ; i ++ ) for( uint16_t j= 0 ; j < net->neurons[i] ; j++ ){
        fwrite( STRICT_LE32( net->nn[i][j].inputs ) , sizeof( input_t ) , 1 , fp );
        fwrite( &net->nn[i][j].bff_idx , sizeof( index_t ) , 1 , fp );
    }
    if( net->layers > 1 ) for( layer_t i= 0 ; i < net->layers - 1 ; i++ ){
        fwrite( &net->wiring[i].arrays , sizeof( index_t ) , 1 , fp );
        for( index_t j= 0 ; j < net->wiring[i].arrays ; j++ ){
            fwrite( &net->wiring[i].array_type[j] , sizeof( type_t ) , 1 , fp );
            switch( net->wiring[i].array_type[j] ){
                case 'M':
                    fwrite( STRICT_LE32( net->wiring[i].size[j] ) , sizeof( input_t ) , 1 , fp );
                    for( input_t k= 0 ; k < net->wiring[i].size[j] ; k++ ){
                        fwrite( &net->wiring[i].src_type[j][k] , sizeof( type_t ) , 1 , fp );
                        switch( net->wiring[i].src_type[j][k] ){
                        case 'N':
                            fwrite( STRICT_LE16( net->wiring[i].src_layer[j][k] ) , sizeof( layer_t ) , 1 , fp );
                            fwrite( STRICT_LE16( net->wiring[i].src_index[j][k] ) , sizeof( uint16_t ) , 1 , fp );
                            break;
                        case 'O':
                        case 'I':
                            fwrite( STRICT_LE16( net->wiring[i].src_index[j][k] ) , sizeof( uint16_t ) , 1 , fp );
                        }
                    }
                    break;
                case 'N':
                    fwrite( STRICT_LE16( net->wiring[i].src_layer[j][0] ) , sizeof( layer_t ) , 1 , fp );
                    fwrite( STRICT_LE16( net->wiring[i].src_index[j][0] ) , sizeof( uint16_t ) , 1 , fp );
                    break;
            }
        }
    }
    for( layer_t i= 0 ; i < net->layers ; i ++ ) for( uint16_t j= 0 ; j < net->neurons[i] ; j++ ){
        fwrite( &net->nn[i][j].fn , sizeof( index_t ) , 1 , fp );
        fwrite( STRICT_LE32( float32( net->nn[i][j].b , ieee754 ) ) , sizeof( data_t ) , 1 , fp );
        for( input_t k= 0 ; k < net->nn[i][j].inputs ; k++ ) fwrite( STRICT_LE32( float32( net->nn[i][j].w[k] , ieee754 ) ) , sizeof( data_t ) , 1 , fp );
    }
    fclose( fp );
    return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
/**
 * @details
 * Reconstructs the network structure, weights, biases, and buffer wiring from a binary file.  
 * The function reads the file header to validate the format and version, then proceeds to read the core network data while handling endianness and floating-point representation.
 * 
 * @todo Implement file validation and data standardization during loading to ensure compatibility and integrity of loaded networks.
 * @todo Consider adding support for versioning in the file format to allow for future extensions and backward compatibility.
 * @todo Explore options for compressing the saved network files to reduce disk space usage, especially for larger networks.
 */
struct net_s loadnet( char *name ){
    net_s net= {0};
    uint8_t little_endian= !checkendian( ) , ieee754= isieee754( );
    char NAME[ NAME_LENGTH ];
    if( snprintf( NAME , sizeof( NAME ) , "%s%s" , name , ".ntic" ) >= ( int )sizeof( NAME ) ) return net;
    FILE *fp= fopen( NAME , "rb" );
    if( fp == NULL ) return net;
    char magic[sizeof( MAGIC )];
    fread( magic , sizeof( char ) , sizeof( magic ) , fp );
    if( strncmp( magic , MAGIC , sizeof( MAGIC ) - 1 ) || magic[sizeof( magic ) - 1] != VERSION ) return net;
    fread( &net.inputs , sizeof( uint32_t ) , 1 , fp );
    if( little_endian ) net.inputs= bswap32( net.inputs );
    fread( &net.layers , sizeof( uint16_t ) , 1 , fp );
    if( little_endian ) net.layers= bswap16( net.layers );
    uint16_t *neurons;
    neurons= malloc( net.layers * sizeof( uint16_t ) );
    fread( neurons , sizeof( uint16_t ) , net.layers , fp );
    if( little_endian ) for( uint16_t i= 0 ; i < net.layers ; i++ ) neurons[i]= bswap16( neurons[i] );
    newnet( &net , neurons , net.layers );
    free( neurons );
    for( uint16_t i= 0 ; i < net.layers ; i ++ ) for( uint16_t j= 0 ; j < net.neurons[i] ; j++ ){
        fread( &net.nn[i][j].inputs , sizeof( uint32_t ) , 1 , fp );
        if( little_endian ) net.nn[i][j].inputs= bswap32( net.nn[i][j].inputs );
        fread( &net.nn[i][j].bff_idx , sizeof( index_t ) , 1 , fp );
    }
    if( net.layers > 1 ){
        net.wiring= memtrack( malloc( ( net.layers - 1 ) * sizeof( wiring_s ) ) );
        for( uint16_t i= 0 ; i < net.layers - 1 ; i++ ){
            fread( &net.wiring[i].arrays , sizeof( index_t ) , 1 , fp );
            if( little_endian ) net.wiring[i].arrays= bswap16( net.wiring[i].arrays );
            net.wiring[i].array_type= memtrack( malloc( net.wiring[i].arrays * sizeof( uint8_t ) ) );
            net.wiring[i].size= memtrack( malloc( net.wiring[i].arrays * sizeof( uint32_t ) ) );
            net.wiring[i].src_type= memtrack( malloc( net.wiring[i].arrays * sizeof( uint8_t * ) ) );
            net.wiring[i].src_layer= memtrack( malloc( net.wiring[i].arrays * sizeof( uint16_t * ) ) );
            net.wiring[i].src_index= memtrack( malloc( net.wiring[i].arrays * sizeof( uint16_t * ) ) );
            for( uint16_t j= 0 ; j < net.wiring[i].arrays ; j++ ){
                fread( &net.wiring[i].array_type[j] , sizeof( uint8_t ) , 1 , fp );
                switch( net.wiring[i].array_type[j] ){
                    case 'M':
                        fread( &net.wiring[i].size[j] , sizeof( uint32_t ) , 1 , fp );
                        if( little_endian ) net.wiring[i].size[j]= bswap32( net.wiring[i].size[j] );
                        net.wiring[i].src_type[j]= memtrack( malloc( net.wiring[i].size[j] * sizeof( uint8_t ) ) );
                        net.wiring[i].src_layer[j]= memtrack( malloc( net.wiring[i].size[j] * sizeof( uint16_t ) ) );
                        net.wiring[i].src_index[j]= memtrack( malloc( net.wiring[i].size[j] * sizeof( uint16_t ) ) );
                        for( uint32_t k= 0 ; k < net.wiring[i].size[j] ; k++ ){
                            fread( &net.wiring[i].src_type[j][k] , sizeof( uint8_t ) , 1 , fp );
                            switch( net.wiring[i].src_type[j][k] ){
                                case 'N':
                                    fread( &net.wiring[i].src_layer[j][k] , sizeof( uint16_t ) , 1 , fp );
                                    if( little_endian ) net.wiring[i].src_layer[j][k]= bswap16( net.wiring[i].src_layer[j][k] );
                                    fread( &net.wiring[i].src_index[j][k] , sizeof( uint16_t ) , 1 , fp );
                                    if( little_endian ) net.wiring[i].src_index[j][k]= bswap16( net.wiring[i].src_index[j][k] );
                                    break;
                                case 'O':
                                case 'I':
                                    fread( &net.wiring[i].src_index[j][k] , sizeof( uint16_t ) , 1 , fp );
                                    if( little_endian ) net.wiring[i].src_index[j][k]= bswap16( net.wiring[i].src_index[j][k] );
                            }
                        }
                        break;
                    case 'N':
                        net.wiring[i].src_type[j]= memtrack( malloc( sizeof( uint8_t ) ) );
                        net.wiring[i].src_layer[j]= memtrack( malloc( sizeof( uint16_t ) ) );
                        net.wiring[i].src_index[j]= memtrack( malloc( sizeof( uint16_t ) ) );
                        fread( &net.wiring[i].src_layer[j][0] , sizeof( uint16_t ) , 1 , fp );
                        if( little_endian ) net.wiring[i].src_layer[j][0]= bswap16( net.wiring[i].src_layer[j][0] );
                        fread( &net.wiring[i].src_index[j][0] , sizeof( uint16_t ) , 1 , fp );
                        if( little_endian ) net.wiring[i].src_index[j][0]= bswap16( net.wiring[i].src_index[j][0] );
                        break;
                }
            }
        }
    }
    buildnet( &net );
    for( uint16_t i= 0 ; i < net.layers ; i ++ ) for( uint16_t j= 0 ; j < net.neurons[i] ; j++ ){
        fread( &net.nn[i][j].fn , sizeof( uint8_t ) , 1 , fp );
        uint32_t aux;
        fread( &aux , sizeof( uint32_t ) , 1 , fp );
        if( little_endian ) aux= bswap32( aux );
        net.nn[i][j].b= floatsys( aux , ieee754 );
        for( uint32_t k= 0 ; k < net.nn[i][j].inputs ; k++ ){
            fread( &aux , sizeof( uint32_t ) , 1 , fp );
            if( little_endian ) aux= bswap32( aux );
            net.nn[i][j].w[k]= floatsys( aux , ieee754 );
        }
    }
    fclose( fp );
    return net;
}
#pragma GCC diagnostic pop