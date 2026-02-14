/**
 * @file ntcore.h
 * @brief Fundamental structural definitions of the NeuroTIC runtime model.
 * @author Oscar Sotomayor (Titux)
 *
 * @details
 * Defines the minimal structural components required to represent a network:
 * neurons, wiring descriptors, and the root network container.
 *
 * Defines structure only.
 * No construction or execution semantics are provided.
 */

#ifndef NTCORE_H
#define NTCORE_H

#include <stdint.h>

/**
 * @name Type Aliases
 *
 * Fundamental scalar types for NeuroTIC structures.
 *
 * These aliases abstract primitive C types to decouple structural
 * semantics from numeric representation and centralize precision control.
 *
 * @code
 */
typedef float
    data_t,
    weight_t,
    bias_t;

typedef uint8_t
    type_t,
    index_t;

typedef uint16_t
    layer_t;

typedef uint32_t
    input_t;
/** @endcode */

/* ------------------------------------------------------------------------- */
/* Neuron                                                                    */
/* ------------------------------------------------------------------------- */

/**
 * @brief Structural unit representing a computation node.
 *
 * A neuron binds input references, weight coefficients,
 * a bias term, and an activation selector.
 * It does not define memory ownership beyond structural integrity requirements.
 */
typedef struct neuron_s {
    input_t     inputs;     /**< Logical number of inputs. */
    index_t     bff_idx;    /**< Selected buffer set index. */
    data_t      **in;       /**< Resolved input references. */
    weight_t    *w;         /**< Weight coefficients. */
    bias_t      b;          /**< Bias term. */
    index_t     fn;         /**< Activation function selector. */
    data_t      out;        /**< Last computed output value. */
} neuron_s;


/* ------------------------------------------------------------------------- */
/* Wiring                                                                    */
/* ------------------------------------------------------------------------- */

/**
 * @brief Declarative descriptor of inter-layer input resolution.
 *
 * Wiring defines how logical input sets are constructed for a layer.
 * Field interpretation is type-dependent and resolved externally.
 */
typedef struct wiring_s {
    uint16_t    arrays;         /**< Number of logical input sets. */
    type_t      *array_type;    /**< Type of each input set. */
    input_t     *size;          /**< Logical size per input set. */
    type_t      **src_type;     /**< Source type per element. */
    layer_t     **src_layer;    /**< Source layer reference. */
    uint16_t    **src_index;    /**< Source index reference. */
} wiring_s;


/* ------------------------------------------------------------------------- */
/* Network                                                                   */
/* ------------------------------------------------------------------------- */

/**
 * @brief Root structural container of a NeuroTIC network.
 *
 * Represents the complete structural topology and memory graph
 * required to execute a network instance.
 *
 * The network owns its internal structures.
 * External input sources are considered external references.
 */
typedef struct net_s {
    input_t     inputs;     /**< Number of external inputs. */
    layer_t     layers;     /**< Total number of layers. */
    uint16_t    *neurons;   /**< Neuron count per layer. */
    data_t      **in;       /**< External input references. */
    neuron_s    **nn;       /**< Layered neuron arrays. */
    wiring_s    *wiring;    /**< Wiring descriptors per layer. */
    data_t      ****bff;    /**< Buffer reference sets. */
    data_t      **out;      /**< Output references. */
} net_s;

#endif // NTCORE_H
