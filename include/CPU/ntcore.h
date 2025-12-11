/**
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
 */

#ifndef NTCORE_H
#define NTCORE_H

#include <stdint.h>

/**
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