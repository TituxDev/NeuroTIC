/**
 * @file ntactivation.c
 * @author Oscar Sotomayor (Titux)
 * @brief Internal implementation of activation behavior.
 *
 * @details
 * This file provides the concrete implementation of activation functions
 * and their derivatives used by the NeuroTIC execution context.
 *
 * It defines the internal activation dispatch tables and the
 * recommended initialization ranges associated with each activation.
 *
 * This file does not define public contracts.
 * All external access is mediated through ntactivation.h.
 * 
 * ---------------------------------------------------------------------------  
 * Activation execution model                                                 
 * ---------------------------------------------------------------------------  
 *
 * The activation subsystem implements a stateless execution model
 * for scalar transformations.
 *
 * A fixed set of activation operations is defined and resident
 * in memory. Each operation can be selected and evaluated
 * independently.
 *
 * Execution is defined by three elements:
 *
 * 1. Activation selector
 *    Identifies the operation to be evaluated.
 *
 * 2. Operation mode
 *    Selects between the activation function or its derivative.
 *
 * 3. Input operand
 *    The scalar value provided for evaluation.
 *
 * No internal state is preserved between evaluations.
 * All operations are pure and deterministic.
 *
 * Access to activation behavior is performed exclusively through
 * the activation dispatch table.
 * 
 */

#include "ntactivation.h"
#include <math.h>


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

/**
 * @brief Activation dispatch table.
 * @details
 * Defines the mapping between activation identifiers and their
 * associated execution functions.
 * Indexing:
 * - First index  : activation identifier
 * - Second index : [0] activation
 *                  [1] derivative
 */
float ( *ntact_activation[NTACT_TOTAL_FUNCTIONS][2] )( float )={
    [NTACT_BOOLEAN]= { boolean , boolean_d },
    [NTACT_SIGMOID]= { sigmoid , sigmoid_d }
//  [NTACT_<NAME>]= { <func> , <func>_d }
};

/**
 * @brief Random initialization range table for activation functions.
 * @details
 * Defines the preferred initialization range associated with each
 * activation function.
 */
float ntact_rand_range[NTACT_TOTAL_FUNCTIONS][2]={
    [NTACT_BOOLEAN]= { -1.0f , 1.0f },
    [NTACT_SIGMOID]= { -1.0f , 1.0f }
//  [NTACT_<NAME>]= { <min> , <max> }
};