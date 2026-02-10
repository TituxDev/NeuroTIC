/**
<<<<<<< HEAD
<<<<<<< HEAD
=======
 * @defgroup NTCore
>>>>>>> 47ec0ed (Documentation created)
 * @file ntcore.h
 * @author Oscar Sotomayor (Titux)
 * @ingroup NTCore
 * @brief Semantic and structural core of the NeuroTIC framework.
 *
<<<<<<< HEAD
 * All higher-level behaviors—construction, initialization, activation dispatch,
 * training, and persistence—are built on top of these definitions. The core
 * structures remain intentionally small, explicit, and fully transparent, making
 * every network inspectable at runtime and fully compatible with both automated
 * and hand-written topologies.
=======
 * @addtogroup core_description Core Description
 * @brief Fundamental data structures for neural networks
 * @details
 * This module defines the core data structures used to represent neural networks
 * within the NeuroTIC framework. These structures are designed to be transparent
 * and easily inspectable, forming the backbone of the framework's data handling.
 * @{
 */

/**
 * @file ntcore.h
 * @brief Core neural network data structures for NeuroTIC framework
 * 
 * Defines the fundamental structures used to represent neural networks
 * in a transparent and inspectable manner. These structures form the
 * data backbone of the framework.
 * 
 * @see Core.md
>>>>>>> documentation
=======
 * This header defines the minimal, authoritative, and invariant data
 * structures that describe a NeuroTIC network in memory.
 *
 * ntcore.h is a special-case header:
 * - It defines *structure only*, never behavior.
 * - It establishes the semantic contract of the framework.
 * - It is stable by design and changes only under major revisions.
 *
 * No construction logic, execution flow, memory management,
 * activation dispatch, training, mutation, or persistence rules
 * are defined here.
 *
 * All higher-level behavior is implemented elsewhere and must
 * conform strictly to the structures defined in this file.
 *
 * @todo Propagate typedef domain adjustments to dependent code.
 * @todo Create const inputs for read-only references
 * 
 * ---------------------------------------------------------------------------
 * Pointer hierarchy overview
 * ---------------------------------------------------------------------------
 *
 * The following diagram illustrates the ownership and indirection levels
 * inside the root network container (net_s).
 *
 * This diagram is descriptive, not procedural.
 * It reflects memory layout and access relationships only.
 *
 * @code{.txt}
 * net_s
 * ├── inputs   :   input_t @ref net_s.inputs
 * ├── layers   :   layer_t @ref net_s.layers
 * ├── neurons  :   uint16_t*
 * │   └── [layers] @ref net_s.neurons
 * ├── in       :   data_t**
 * │   └── [inputs]* @ref net_s.in
 * ├── nn       :   neuron_s**
 * │   └── [layers]*
 * │       └── [neurons[l]] @ref neuron_s
 * │           ├── inputs   :   input_t @ref neuron_s.inputs
 * │           ├── bff_idx  :   index_t @ref neuron_s.bff_idx
 * │           ├── in   :   data_t** @ref neuron_s.in
 * │           ├── w    :   weight_t*
 * |           |   └── [inputs] @ref neuron_s.w
 * │           ├── b    :   bias_t @ref neuron_s.b
 * │           ├── fn   :   index_t @ref neuron_s.fn
 * │           └── out  :   data_t @ref neuron_s.out
 * ├── wiring   :   wiring_s*
 * │   └── [layers - 1] @ref wiring_s
 * │       ├── arrays   :   uint16_t @ref wiring_s.arrays
 * │       ├── array_type   :   type_t*
 * |       |    └── [arrays] @ref wiring_s.array_type
 * │       ├── size :   input_t*
 * |       |   └── [arrays] @ref wiring_s.size
 * │       ├── src_type :   type_t**
 * |       |    └── [arrays]*
 * |       |        └── [size[a]] @ref wiring_s.src_type
 * │       ├── src_layer    :   layer_t**
 * |       |    └── [arrays]*
 * |       |        └──[size[a]] @ref wiring_s.src_layer
 * │       └── src_index    :   uint16_t**
 * |            └── [arrays]*
 * |                └── [size[a]] @ref wiring_s.src_index
 * ├── bff  :   data_t****
 * │   └── [layers - 1]***
 * │       └── [wiring[l].arrays]**
 * │           └── [wiring[l].size[a]]* @ref net_s.bff
 * └── out  :   data_t**
 *     └── [neurons[last layer]]* @ref net_s.out
 *@endcode
 * ---------------------------------------------------------------------------
 * Design notes
 * ---------------------------------------------------------------------------
 *
 * - All pointer fields assume valid memory for the lifetime of the network.
 * - No structure in this file allocates or frees memory.
 * - No implicit ownership rules are encoded beyond what is structurally evident.
 * - External inputs are treated as peripherals to the core.
 *
 * This file is the single source of truth for the internal shape
 * of a NeuroTIC network.
>>>>>>> 47ec0ed (Documentation created)
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
<<<<<<< HEAD
<<<<<<< HEAD
 * @ingroup NTCore
=======
>>>>>>> 47ec0ed (Documentation created)
 * @struct neuron_s
 * @ingroup NTCore
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
 * @struct wiring_s
 * @ingroup NTCore
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
 * @struct net_s
 * @ingroup NTCore
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
=======
 * @addtogroup atomic_uint Atomic Unit Management
 * @{
 * @struct neuron_s
 * @brief Minimal data container for a single neuron.
 * 
 * Contains all data needed to compute a neuron's output: input pointers,
 * weights, bias, activation function, and output storage.
 * 
 */
typedef struct neuron_s {
    uint32_t inputs;     /**< Number of input connections. */
    uint16_t bff_idx;    /**< Buffer array index for inputs (see net_s::bff). */
    float **in;          /**< Array of pointers to input values. */
    float *w;            /**< Weight array, one per input. */
    float b;             /**< Bias term. */
    uint8_t fn;          /**< Activation function index (see ntactivation.h). */
    float out;           /**< Output value after activation. */
} neuron_s;
/**
 * @}
 */

/**
 * @addtogroup connection_system Connetction System
 * @{
 * @struct wiring_s
 * @brief Describes buffer wiring between adjacent network layers.
 * 
 * Defines how buffers are allocated and connected between layers. Buffers can
 * reference network inputs, outputs, other buffers, or mixed sources.
 * 
 * @see CoreStructures.md for buffer types and connection examples.
 */
typedef struct wiring_s {
    uint16_t arrays;        /**< Number of buffer arrays between two layers. */
    uint8_t *array_type;    /**< Buffer type per array: 'I','O','N', or 'M'. */
    uint32_t *size;         /**< Number of elements in each buffer array. */
    uint8_t **src_type;     /**< Source type per element (for 'M' buffers). */
    uint16_t **src_layer;   /**< Source layer index (for 'M' buffers). */
    uint16_t **src_index;   /**< Source element index (for 'M' buffers). */
} wiring_s;
/**
 * @}
 */

/**
 * @addtogroup network_structure Network Structure
 * @{
 * @struct net_s
 * @brief Complete neural network container.
 * 
 * Holds all neurons, layer sizes, wiring descriptions, and buffer pointers
 * that define a neural network. Use ntbuilder.h functions to create and build.
 * 
 * @see CoreStructures.md for network topology and memory layout.
 */
typedef struct net_s {
    uint32_t inputs;         /**< Number of external input variables. */
    uint16_t layers;         /**< Total number of layers. */
    uint16_t *neurons;       /**< Neuron count per layer (size = layers). */
    float **in;              /**< Input pointers (size = inputs). */
    neuron_s **nn;           /**< Neuron matrix [layer][neuron]. */
    wiring_s *wiring;        /**< Wiring between layers (size = layers-1). */
    float ****bff;           /**< Buffer arrays connecting layers. */
    float **out;             /**< Output pointers (size = neurons[layers-1]). */
} net_s;
/**
 * @}
 */

#endif // NTCORE_H

/**
 * @}
 */
>>>>>>> documentation
