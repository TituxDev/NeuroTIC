/**
 * @file neurons.c
 * @author Oscar Sotomayor (TituxDev)
 * @date June 2025
 * @brief Documentation of neuron_s.
 * @details
 * This file extends the documentation of the neuron_s structure by explaining
 * its conceptual design and the relationships between its internal components.
 * Rather than performing any computation on its own, a neuron in NeuroTIC serves
 * as a *data container* that describes how a single processing unit should behave
 * when evaluated by an external function.
 *
 * @section neuron_structure Neuron Structure Overview
 * Each neuron defines the necessary elements for computation:
 *
 * | Member        | Type       | Description                                                                                |
 * |---------------|------------|--------------------------------------------------------------------------------------------|
 * | `inputs`      | uint16_t   | Number of input references and weights.                                                    |
 * | `in`          | float**    | Array of pointers to external data sources.                                                |
 * | `w`           | float*     | Array of weights, aligned with the `in` array.                                             |
 * | `b`           | float      | Bias term added to the weighted sum.                                                       |
 * | `out`         | float      | Field that stores the neuron’s computed output.                                            |
 * | `fn`          | uint8_t    | Index that identifies the activation function to apply. See  ntact_activation for details. |
 * | `bff_idx`     | uint8_t    | Buffer index used when operating inside a network. See  net_s for details.                 |
 *
 * However, the neuron itself does not execute the calculation — it merely provides
 * the structural context from which a computation routine derives its result.
 *
 * @section neuron_construction Neuron Construction
 * A neuron in NeuroTIC is dynamically constructed according to the value of
 * neuron_s::inputs. This value determines the size of the memory allocation
 * for both the input reference array (`neuron_s::in`) and its corresponding
 * weight array (`neuron_s::w`).
 *
 * Conceptually, once initialized, a neuron can be represented as follows:
 *
 * @code{.text}
 * neuron_s {
 *   uint16_t inputs = N;          // Number of input connections
 *   float*   in[N];               // Pointers to input values (external or shared)
 *   float    w[N];                // Associated weights
 *   float    b;                   // Bias term
 *   float    out;                 // Storage for computed output
 *   uint8_t  fn;                  // Activation function index
 *   uint8_t  bff_idx;             // Index to associated buffer in network system
 * }
 * @endcode
 *
 * @subsection neuron_manual_initialization Manual Neuron Initialization (Standalone Context)
 * The following pseudocode demonstrates how to manually construct and configure a
 * single neuron outside the network context, primarily for debugging or educational purposes.
 * 
 * Optionally connect inputs
 * This manual setup is only relevant for standalone neurons.
 * Within a complete NeuroTIC network, the following members are
 * automatically initialized by the internal wiring system (see net_s).
 * 
 * @section referential_connectivity Referential Connectivity
 * The design of  neuron_s prioritizes *referential connectivity* over data ownership.
 * Each neuron specifies its input sources through neuron_s::in, an array of pointers
 * that reference external values rather than owning them. This pointer-based approach
 * enables dynamic referential linking between computation units while maintaining
 * minimal memory overhead. This design grants a high level of flexibility:
 * neurons can reference external variables, other neurons’ outputs, or even their own
 * output field (neuron_s::out), allowing for self-referential or feedback connections.
 *
 * It is also possible for multiple neurons to share the same input region, since their
 * input arrays can point to overlapping or identical memory locations. This shared
 * referencing mechanism provides both flexibility and efficiency, enabling coordinated
 * processing without redundant data duplication.
 *
 * Direct pointer connections between neurons are technically supported and demonstrated
 * here for conceptual clarity. However, this is not the typical usage within the full
 * NeuroTIC framework. Under normal operation, input connections are established through
 * the network’s internal buffer system, managed by  net_s. In that context, neurons do
 * not connect directly to data sources; instead, their neuron_s::in arrays are populated
 * with pointers to sections within a centralized buffer, whose index is defined by
 * neuron_s::bff_idx. This buffer architecture ensures efficient memory reuse and consistent
 * data flow across layers.
 *
 * @code{.text}
 * // Allocate neuron
 * neuron = allocate(sizeof(neuron_s));
 *
 * // Define number of inputs
 * neuron->inputs = N;
 *
 * // Allocate input references and weights
 * neuron->in = allocate(neuron->inputs * sizeof(float*));
 * neuron->w  = allocate(neuron->inputs * sizeof(float));
 *
 * // Initialize bias and output
 * neuron->b = 0.0f;
 * neuron->out = 0.0f;
 *
 * // Assign activation function and buffer index
 * neuron->fn = ACT_IDENTITY;
 * neuron->bff_idx = 0;
 *
 * // Optionally connect inputs
 * for (i = 0; i < neuron->inputs; ++i)
 *     neuron->in[i] = &external_source[i];
 * @endcode
 * 
 * When operating within the net_s context, each neuron dynamically
 * shares memory regions through its buffer index, allowing multiple neurons
 * to reference the same data segment without explicit reallocation.
 * This design enables flexible connection topologies while maintaining
 * high computational efficiency.
 * 
 * The number of inputs (neuron->inputs) and the array of references
 * (neuron->in) are derived from the network’s connection map.
 * Each neuron uses the index (neuron->bff_idx) to point into the shared
 * buffer system (net_s::bff), ensuring efficient memory reuse and
 * eliminating redundant data copies.
 *
 * @section computation_execution Computation Execution
 * Once a neuron has been constructed and its connections defined—either manually or
 * by the network system—it becomes a passive computational unit awaiting evaluation
 * by the processing function.
 * 
 * During execution, an external function iterates over the neuron’s input references,
 * applies the associated weights and bias, and computes an activation result that
 * is stored in  neuron_s::out. In this example, a simple threshold operation
 * is used to demonstrate the flow of values, but within the complete framework,
 * the activation behavior is determined by the index stored in
 * neuron_s::fn, which maps to a table of predefined activation functions.
 * @code{ .text}
 * sum = neuron->b
 * for each i in neuron->inputs:
 *    sum += *neuron->in[i] * neuron->w[i];
 * neuron->out = activation_function(sum);
 * @endcode 
 *
 * The members  neuron_s::bff_idx and  neuron_s::fn are present here for
 * structural consistency but are not actively used in this minimal context.
 * They become relevant within the broader system, where neurons are integrated
 * into layered network containers (net_s) and interconnected via dynamic
 * buffer wiring.
 *
 * The concluding section of this file includes a compact example demonstrating
 * how multiple neurons can be linked directly through pointer references to
 * form a primitive network. This example focuses on connectivity and data flow,
 * providing a conceptual bridge between isolated neuron structures and full
 * network-level interactions.
 *
 * @note This file focuses on the conceptual organization of the neuron structure.
 * It does not implement or demonstrate the training or inference logic used by
 * the NeuroTIC framework.
 * 
 * @section neuron_integration Integration within net_s
 * When integrated into net_s, each neuron registers through the wiring_s system,
 * which defines how inputs and outputs are interconnected across layers. The wiring
 * system ensures that all neurons within the same layer share synchronized buffer
 * access patterns, allowing for vectorized operations and predictable memory layout.
 * 
 * @section neuron_example Simple Neuron Network Example
 * The following example illustrates how to create a simple network of three neurons.
 * @dot
 * digraph neuron_example {
 *   rankdir=LR;
 *   node [shape=record];
 *
 *   InputA [label="Input A"];
 *   InputB [label="Input B"];
 *   Neuron0 [label="Neuron 0\n(NAND)"];
 *   Neuron1 [label="Neuron 1\n(OR)"];
 *   Neuron2 [label="Neuron 2\n(AND)"];
 *   Output [label="Output"];
 *
 *   InputA -> Neuron0;
 *   InputB -> Neuron0;
 *   InputA -> Neuron1;
 *   InputB -> Neuron1;
 *   Neuron0 -> Neuron2;
 *   Neuron1 -> Neuron2;
 *   Neuron2 -> Output;
 * }
 * @enddot
 * @code{.c}
 */
#include "ntcore.h"

#define NEURON_0 NAND
#define NEURON_1 OR
#define NEURON_2 AND

#define AND           .w= ( float[] ){ 1.0f , 1.0f } , .b= -1.5f
#define NAND          .w= ( float[] ){ -1.0f , -1.0f } , .b= 1.5f
#define OR            .w= ( float[] ){ 1.0f , 1.0f } , .b= -0.5f
#define NOR           .w= ( float[] ){ -1.0f , -1.0f } , .b= 0.5f
#define CONJUNCION_A  .w= ( float[] ){ -1.0f , 1.0f } , .b= -0.5f
#define CONJUNCION_B  .w= ( float[] ){ 1.0f , -1.0f } , .b= -0.5f
#define IMPLICATION_A .w= ( float[] ){ -1.0f , 1.0f } , .b= 0.5f
#define IMPLICATION_B .w= ( float[] ){ 1.0f , -1.0f } , .b= 0.5f

#include <stdio.h>

int main( void ) {
    float inputs[2];

    neuron_s neuron[]={
        [0]= { .inputs= sizeof( inputs ) / sizeof( float ) , .bff_idx= 0 , .in= ( float *[] ){ &inputs[0], &inputs[1] } , NEURON_0 , .fn= 0 , .out= 0.0f },
        [1]= { .inputs= sizeof( inputs ) / sizeof( float ) , .bff_idx= 0 , .in= neuron[0].in , NEURON_1 , .fn= 0 , .out= 0.0f },
        [2]= { .inputs= 2 , .bff_idx= 0 , .in= ( float *[] ){ &neuron[0].out, &neuron[1].out } , NEURON_2 , .fn= 0 , .out= 0.0f }
    };

    for( uint8_t i= 0 ; i < 4 ; i++ ){
        for( size_t j= 0 , f= sizeof( inputs ) / sizeof( float ) ; j < f ; j++ ) inputs[j]= !!( float )( i & ( 1 << j ) );
        for( size_t j= 0 , f= sizeof( neuron ) / sizeof( neuron_s ) ; j < f ; j++ ){
            float sum= neuron[j].b;
            for( uint16_t k= 0 ; k < neuron[j].inputs ; k++ ) sum+= *neuron[j].in[k] * neuron[j].w[k];
            neuron[j].out= (float)( sum >= 0.0f );
        }
        printf( "| input_A: %0.0f | input_B: %0.0f | output: %0.0f |\n", inputs[0] , inputs[1] , neuron[2].out );
    }

    return 0;
}
/** @endcode */