/**
<<<<<<< HEAD
 * @file ntcore.h
 * @brief Core data structures for the NeuroTIC framework.
 * @details
 * This header defines the fundamental structures used to assemble and operate
 * neural networks in NeuroTIC. These types describe neurons, inter-layer wiring,
 * buffer systems, and the network container itself.
 *
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
 */

#ifndef NTCORE_H
#define NTCORE_H

#include <stdint.h>

/**
<<<<<<< HEAD
 * @ingroup NTCore
 * @struct neuron_s
 * @brief Minimal data container for a single neuron.
 * @details
 * This structure stores the data required to evaluate a single neuron:
 * input references, weights, bias, activation selector and last output.
 * The struct is intentionally lightweight and pointer-driven so it can be
 * used inside NeuroTIC or in custom code that follows the same data conventions.
 *
 * @warning
 * The structure itself does not manage the lifetime of pointers stored in it.
 */
typedef struct neuron_s {
    uint32_t inputs;     /**< Number of input connections. */
    uint16_t bff_idx;    /**< Index referencing net_s::bff. @see Wiring.md */
    float **in;          /**< Array of pointers to input values or sources. */
    float *w;            /**< Weight array corresponding to each input. */
    float b;             /**< Bias term of the neuron. */
    uint8_t fn;          /**< Index of the activation function @see ntactivation.h. */
    float out;           /**< Output value after activation function. */
} neuron_s;

/**
 * @ingroup NTCore
 * @struct wiring_s
 * @brief Describes buffer wiring between adjacent network layers.
 * @details Each instance defines how intermediate buffers are allocated and how
 * they connect neuron outputs from one layer to inputs of the next.
 * These connections can originate from various internal or external
 * data sources, defined by the array and source type descriptors.
 */
typedef struct wiring_s {
    uint16_t arrays;        /**< Number of buffer arrays between two layers. */
    uint8_t *array_type;    /**< Type of each array, defining the data source (input, output, intermediate, or external). */
    uint32_t *size;         /**< Length of each buffer array. */
    uint8_t **src_type;     /**< Source type for each pointer (e.g., neuron, input, output, or external). */
    uint16_t **src_layer;   /**< Layer index from which the data originates. */
    uint16_t **src_index;   /**< Index of the source element within the originating layer. */
} wiring_s;

/**
 * @ingroup NTCore
 * @struct net_s
 * @brief Encapsulates the full neural network definition.
 * @details This structure represents an entire neural network within the NeuroTIC system.
 * It contains the neurons, inter-layer buffer wiring, and connection references.
 * Through its flexible pointer-based design, it supports dynamic construction
 * of complex, non-linear network topologies.
 */
typedef struct net_s {
    uint32_t inputs;         /**< Number of external input variables to the network. */
    uint16_t layers;         /**< Total number of layers in the network. */
    uint16_t *neurons;       /**< Array specifying the number of neurons per layer. */
    float **in;              /**< Array of pointers to input data sources. */
    neuron_s **nn;           /**< Matrix of neurons organized by layer. */
    wiring_s *wiring;        /**< Array describing the buffer wiring between layers. */
    float ****bff;           /**< Multi-dimensional array of buffers connecting layers. */
    float **out;             /**< Array of pointers referencing the output neurons’ results. */
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
