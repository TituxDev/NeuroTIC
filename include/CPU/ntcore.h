/**
 * @file ntcore.h
 * @brief Core neural network data structures for NeuroTIC framework
 * @author Oscar Sotomayor (Titux)
 *
 * @todo Propagate typedef domain adjustments to dependent code.
 * @todo Create const inputs for read-only references
 * 
 * @details
 * This header defines the fundamental data structures used to represent
 * the internal state of a NeuroTIC network. It includes definitions for
 * neurons, wiring configurations, and the overall network structure.
 */

#ifndef NTCORE_H
#define NTCORE_H

#include <stdint.h>

/* ------------------------------------------------------------------------- */
/* Fundamental scalar types                                                   */
/* ------------------------------------------------------------------------- */

typedef float    
    data_t,
    weight_t,
    bias_t
;
typedef uint8_t  
    type_t,
    index_t
;
typedef uint16_t
    layer_t
;
typedef uint32_t
    input_t
;

/* ------------------------------------------------------------------------- */
/* Neuron                                                                     */
/* ------------------------------------------------------------------------- */

/**
 * @brief Structural description of a single neuron.
 *
 * A neuron is a lightweight container that binds:
 * - a selected input buffer,
 * - its associated weights,
 * - a bias term,
 * - an activation function selector,
 * - and the last computed output value.
 * 
 * This structure does not own the memory referenced by its pointer members.
 * It assumes all referenced memory remains valid while the neuron exists.
 */
typedef struct neuron_s {
    input_t     inputs;     /**< Logical number of input connections. */
    index_t     bff_idx;    /**< Selected input buffer set index. */
    data_t      **in;       /**< Aliased input references. */
    weight_t    *w;         /**< Weight array (one per input). */
    bias_t      b;          /**< Bias term. */
    index_t     fn;         /**< Activation function selector. */
    data_t      out;        /**< Output value after activation. */
} neuron_s;

/* ------------------------------------------------------------------------- */
/* Wiring                                                                     */
/* ------------------------------------------------------------------------- */

/**
 * @brief Describes how logical input buffers are resolved between layers.
 *
 * Each wiring instance defines a set of input arrays that may be
 * selected by neurons in the destination layer.
 *
 * The semantic interpretation of the source fields depends on the
 * array type and is resolved during network construction.
 */
typedef struct wiring_s {
    uint16_t    arrays;         /**< Number of logical input sets. */
    type_t      *array_type;    /**< Type selector per input set. */
    input_t     *size;          /**< Logical input count per set. */
    type_t      **src_type;     /**< Source type per input reference. */
    layer_t     **src_layer;    /**< Source layer index, if applicable. */
    uint16_t    **src_index;    /**< Source element index, if applicable. */
} wiring_s;

/* ------------------------------------------------------------------------- */
/* Network                                                                    */
/* ------------------------------------------------------------------------- */

/**
 * @brief Root container describing a complete NeuroTIC network.
 *
 * This structure represents the full internal state of a network.
 *
 * It owns all internal memory used by the network, with the exception
 * of external input sources, which are treated as peripherals.
 *
 * The network is defined entirely by its structure; no behavioral
 * guarantees are implied by this definition alone.
 */
typedef struct net_s {
    input_t     inputs;     /**< Number of external input references. */
    layer_t     layers;     /**< Total number of layers. */
    uint16_t    *neurons;   /**< Neuron count per layer. */
    data_t      **in;       /**< External input references. */
    neuron_s    **nn;       /**< Per-layer neuron arrays. */
    wiring_s    *wiring;    /**< Wiring descriptors between layers. */
    data_t      ****bff;    /**< Input buffer reference sets. */
    data_t      **out;      /**< Output references (last layer). */
} net_s;

#endif // NTCORE_H