/**
 * @file ntactivation.c
 * @brief Activation Functions List.  
 * @ref http://tituxdev.github.io/NeuroTIC/src/CPU/ntactivation.c
 * 
 * @author Oscar Sotomayor (Titux)
 */

#include "ntactivation.h"
#include <math.h>

/**
 * @name Activation Functions and Derivatives
 * @brief Implementations of supported activation functions and their derivatives.
 * 
 * @details
 * Each activation function is implemented as a separate static function, along with its derivative.  
 * The functions are designed to be efficient and suitable for use in the forward and backward passes of neural network training.  
 * The derivative functions are implemented to return non-zero values to avoid issues with zero gradients during training, particularly for the boolean activation function.  
 * The activation functions currently implemented include:
 * - Boolean Step Function
 * - Sigmoid Function
 * 
 * Additional activation functions can be added following the same pattern, ensuring that both the function and its derivative are defined and included in the `ntact_activation` dispatch table.
 * 
 * @param x 
 * @return float 
 * 
 * @code{.c}
 */
//BOOLEAN
static float boolean( float x ){
    return x >= 0.0f ? 1.0f : 0.0f;
}
// [!!] This derivative is not mathematically correct.
// It intentionally returns a constant value to avoid zero-gradient behavior.
static float boolean_d( float x ){
    return x= 1.0f;
}
//SIGMOID
static float sigmoid( float x ){
    return 1.0f / ( 1.0f + expf( -x ) );
}
static float sigmoid_d( float x ){
    x = sigmoid( x );
    return x * ( 1.0f - x );
}
// ...
/** @endcode */

/**
 * @details
 * Defines the mapping between activation identifiers and their associated execution functions.  
 * Indexing:
 * - First index  : activation identifier
 * - Second index : [0] activation
 *                  [1] derivative
 * 
 * The table is initialized with the supported activation functions, and can be extended to include additional functions as needed.
 */
float ( *ntact_activation[NTACT_TOTAL_FUNCTIONS][2] )( float )={
    [NTACT_BOOLEAN]= { boolean , boolean_d },
    [NTACT_SIGMOID]= { sigmoid , sigmoid_d }
//  [NTACT_<NAME>]= { <func> , <func>_d }
};

/**
 * @details
 * Defines the random initialization ranges for each activation function.  
 * Each row corresponds to an activation function, with the first column representing the minimum value and the second column representing the maximum value for weight initialization.  
 * These ranges are used in the `randnet` function to ensure that weights are initialized within appropriate bounds for each activation function, which can help improve training performance and convergence.
 * 
 * The current ranges are set to [-1.0f, 1.0f] for both the boolean and sigmoid activation functions, but these can be adjusted based on the specific requirements of the network architecture and training regimen.  
 * Additional activation functions can be added to this table as they are implemented, ensuring that the `randnet` function can properly initialize weights for all supported activation types.  
 * 
 * @code{.c}
 */
float ntact_rand_range[NTACT_TOTAL_FUNCTIONS][2]={
    [NTACT_BOOLEAN]= { -1.0f , 1.0f },
    [NTACT_SIGMOID]= { -1.0f , 1.0f }
//  [NTACT_<NAME>]= { <min> , <max> }
};
/** @endcode */