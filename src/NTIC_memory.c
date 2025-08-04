/**
 * @note This file is part of NeuroTIC, a neural network library.
 * @author Oscar Sotomayor
 * @date 2025
 * @version 1.0
 * @license Mozilla Public License v. 2.0 (https://mozilla.org/MPL/2.0/)
 * @file NTIC_memory.c
 * @brief Memory management functions for the NeuroTIC neural network library.
 */

#include "NTIC_memory.h"

#include <stdio.h>
#include <stdlib.h>

void **mem_register= NULL;
unsigned int mem_count= 0;

void memfree( void ){
    while( mem_count ) if( mem_register[--mem_count] ){
        free( mem_register[mem_count] );
        mem_register[mem_count]= NULL;
    }
    free( mem_register );
    mem_register= NULL;
}

void memtrack( void *mem ){
    if( !mem ){
		fprintf( stderr , "Failed to allocate memory for neurons in define_net.\n" );
		exit( EXIT_FAILURE );
	}
	void **temp= realloc( mem_register , ++mem_count * sizeof( void * ) );
	if( !temp ){
        fprintf( stderr , "Failed to allocate memory for free tracking.\n" );
        exit( EXIT_FAILURE );
	}
	mem_register= temp;
	mem_register[mem_count - 1]= mem;
    static char called= 1;
	if( called ){
		atexit( memfree );
		called= 0;
	}
}