/*
 * neurotic_555.h
 *
 * This version is distributed unlicensed, as-is, with no warranty.
 * Use at your own risk.
 *  
 * Inspired by the 555 timer IC, this header defines a minimal neuron structure
 * intended for experimental or educational neural network development.
 * It includes fields for input references, weights, a bias term, and an array
 * of function pointers that can be used for activation and derivative functions.
 *
 * This file also includes minimal dependencies: stdlib.h for memory management
 * and rand() usage, and math.h for building your own activation functions.
 * 
 * The rest of the system is entirely up to the user to build from scratch.
 *
 * Compatibility Note:
 * This structure is based on the core neuron type used in the official
 * NeuroTIC library, with one key modification: the 'activ' field is implemented
 * as an array of function pointers instead of an index.
 *
 * Licensing Condition:
 * Although this header is distributed unlicensed, any derivative work that:
 * 
 *   - Replaces the 'activ' function pointer array with a numeric index,
 *   - Uses an index-based mechanism to select or dispatch activation functions,
 *   - Or integrates components or types defined in the official NeuroTIC source
 * 
 * Will be considered a functional extension of the NeuroTIC project,
 * and must therefore comply with the terms of the NeuroTIC license.
 */



/*
* This is an exaple of how to use this header:
*
* #include "neurotic_555.h"
* float sigmoid(float x) { return 1.0f / (1.0f + exp(-x)); }
* float sigmoid_derivative(float x) { return sigmoid(x) * (1.0f - sigmoid(x)); }
* struct neuron555 my_neuron[]={
*   { .inputs = 2, .activ = { sigmoid, sigmoid_derivative }},
*   { .inputs = 2, .activ = { sigmoid, sigmoid_derivative }},
*   { .inputs = 2, .activ = { sigmoid, sigmoid_derivative }},
* };
*

int main( ) {
   for (int i = 0; i < sizeof(my_neuron) / sizeof(my_neuron[0]); i++) {
       float min= -1.0f, max = 1.0f;
       my_neuron[i].IN = malloc(my_neuron[i].inputs * sizeof(float *));
       my_neuron[i].W= malloc(my_neuron[i].inputs * sizeof(float));
       my_neuron[i].B = RAND_RANGE(min, max);
       for (unsigned int j = 0; j < my_neuron[i].inputs; j++) my_neuron[i].W[j] = RAND_RANGE(min, max);
   }
   my_neuron[2].IN[0]= &my_neuron[0].OUT;
   my_neuron[2].IN[1]= &my_neuron[1].OUT;
   // YOUR CODE HERE...
   for( int i= 0 ; i < sizeof(my_neuron) / sizeof(my_neuron[0]); i++ ) {
       free(my_neuron[i].IN);
       free(my_neuron[i].W);
   }
   return 0;
}
*/

#ifndef NEUROTIC_555_H
#define NEUROTIC_555_H

#ifndef RAND_RANGE
#define RAND_RANGE(min, max) (((float)rand() / RAND_MAX) * ((max) - (min)) + (min))
#endif // RAND_RANGE

#include <stdlib.h>
#include <math.h>

struct neuron555 {
    unsigned int inputs;
    float **IN;
    float *W;
    float B;
    float ( *activ[2] )( float );
    float OUT;
};

#endif // NEUROTIC_555_H