/**
 * @file ntactivation.c
 * @brief Activation Functions List.
 *
 * @details
 * Implements the activation functions and derivatives referenced by
 * `ntact_function_id_t` and dispatched through `ntact_activation`.
 * Each entry also defines a weight-initialization range in `ntact_rand_range`.
 * New activation functions are added here; see the dispatch tables below
 * for the required pattern.
 *
 * @author Oscar Sotomayor
 * @date 2026
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
 * Derivative functions are implemented to return non-zero values to avoid zero-gradient behavior during training.
 *
 * Both a function and its derivative expect the same input: the
 * pre-activation value (e.g. the weighted sum before activation), never
 * the already-activated output.
 *
 * The set of supported activation functions is defined by `ntact_function_id_t` and may grow over time.
 * New entries follow the same pattern: implement the function and its derivative here, then register both
 * in the `ntact_activation` dispatch table and their corresponding range in `ntact_rand_range`.
 *
 * @param x The pre-activation input value.
 * @return The activation output, or its derivative, evaluated at x.
 *
 * @code{.c}
 */
//BOOLEAN
static float boolean( float x ){
    return x >= 0.0f ? 1.0f : 0.0f;
}
// [!]  This derivative is not mathematically correct.
//      It intentionally returns a constant value to avoid zero-gradient behavior.
// [!]  Assigning to `x` instead of using `(void)x;` avoids an unused-parameter
//      warning without resorting to compiler pragmas.
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
//TANH
// [!]  Named 'hyptan' instead of 'tanh' to avoid clashing with the
//      standard <math.h> tanhf/tanh symbols.
static float hyptan( float x ){
    return tanhf( x );
}
static float hyptan_d( float x ){
    x = tanhf( x );
    return 1.0f - x * x;
}
//RELU
static float relu( float x ){
    return x > 0.0f ? x : 0.0f;
}
static float relu_d( float x ){
    return x > 0.0f ? 1.0f : 0.0f;
}
//LEAKY RELU
#define NTACT_LRELU_ALPHA 0.01f
static float lrelu( float x ){
    return x > 0.0f ? x : NTACT_LRELU_ALPHA * x;
}
static float lrelu_d( float x ){
    return x > 0.0f ? 1.0f : NTACT_LRELU_ALPHA;
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
 */
float ( *ntact_activation[NTACT_TOTAL_FUNCTIONS][2] )( float )={
    [NTACT_BOOLEAN]= { boolean , boolean_d },
    [NTACT_SIGMOID]= { sigmoid , sigmoid_d },
    [NTACT_TANH]   = { hyptan  , hyptan_d  },
    [NTACT_RELU]   = { relu    , relu_d    },
    [NTACT_LRELU]  = { lrelu   , lrelu_d   }
//  [NTACT_<NAME>]= { <func> , <func>_d }
};

/**
 * @details
 * Defines the random initialization range for each activation function.
 * Each row corresponds to an activation function (indexed by `ntact_function_id_t`), with the first
 * column representing the minimum value and the second column representing the maximum value for
 * weight initialization. These ranges are used in `randnet()` to keep initial weights within bounds
 * appropriate for each activation function, which can help improve training performance and convergence.
 */
float ntact_rand_range[NTACT_TOTAL_FUNCTIONS][2]={
    [NTACT_BOOLEAN]= { -1.0f , 1.0f },
    [NTACT_SIGMOID]= { -1.0f , 1.0f },
    [NTACT_TANH]   = { -1.0f , 1.0f },
    [NTACT_RELU]   = { -0.5f , 0.5f },
    [NTACT_LRELU]  = { -0.5f , 0.5f }
//  [NTACT_<NAME>]= { <min> , <max> }
};