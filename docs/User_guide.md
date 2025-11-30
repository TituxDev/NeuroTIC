# NeuroTIC - User guide.md

## Introduction

NeuroTIC is a modular framework for designing and training neural networks in C. It lets you assemble networks quickly, experiment freely, and inspect every internal component whenever you need to. Nothing is hidden; every part of computation is transparent.

The framework isn’t meant to teach neural network theory. Instead, it encourages learning by interacting directly with the system: building architectures, modifying their behavior, and observing how they evolve during training.

## Scope and Intent

NeuroTIC focuses on the mechanics of neural networks—their structure, wiring, activation behavior, and training routines. It does not interpret data, manage datasets, or implement higher-level AI logic.

Inputs and outputs are treated purely as numerical values. The meaning of those numbers is entirely up to the user. This design keeps the framework general-purpose, whether you're experimenting with logic tables, regression models, toy problems, or custom topologies.

Helper utilities occasionally appear to simplify tasks such as preparing float-based input/output matrices, but anything related to scaling, encoding, or application-level logic stays outside the framework.

## How Networks Are Represented Internally

NeuroTIC treats a network as a concrete data structure, not an opaque object. Every element involved in computation lives directly inside a net_s instance, making the entire network fully inspectable.

A network consists of three primary structural elements:

• **Neurons (neuron_s)**  
Each neuron holds its inputs, weights, bias, activation index, and runtime values.  
Neurons are arranged in a matrix shaped as layers × neurons-per-layer.

• **Wiring (wiring_s)**  
This descriptor dictates how values move between layers.  
It defines all buffer arrays, their sizes, and the origin of every pointer.  
Helpers can generate standard layouts, but custom wiring is always valid.

• **Buffers**  
Intermediate values live inside the internal buffer system.  
The `bff` field exposes these as multi-level pointers, allowing networks to share memory between layers or dedicate independent regions where needed.

Nothing is encoded through hidden rules. Layer counts, neuron counts, activation functions, and all connection patterns are explicitly stored in the structure. Builders simply fill these fields in predictable ways.

To keep execution stable, the network never reallocates memory after construction. All buffers and neuron arrays remain fixed for the network’s entire lifetime.

**API note:**  
Every function operating on a network takes a pointer to a net_s instance and returns that same pointer.  
This keeps operations chainable and makes copying or modifying networks straightforward.

## Typical Workflow

NeuroTIC follows a short, predictable sequence from empty structure to a trained network.  
Each step maps directly to a specific API function.

1. **Define the network structure**  
Set `inputs` and `layers` inside a `net_s` instance, then call NEWNET.  
This allocates the neuron matrix `nn[layer][neuron]` and stores your neuron-count array.

2. **Describe the topology**  
Topology can be generated automatically or written manually.  
Generators like `newfeedforward()` and `newdense()` fill the `wiring_s` structure with standard patterns.  
Custom wiring is equally acceptable.

3. **Build internal relations**  
Call `buildnet()`.  
This resolves all pointer connections using the metadata already stored in the structure.  
It does not assign activations or alter weights.

4. **Assign activation functions**  
Set each neuron’s `fn` field to the enum value of the desired activation.  
The framework never assigns activations automatically.

5. **Initialize weights**  
Call `randnet()`.  
This initializes only the weights for every neuron.  
The initialization range depends on the assigned activation.  
Biases are untouched here.

6. **Prepare training data**  
A `traindata_t` structure defines:  
learning_rate, tolerance, max_attempts, and samples.  
Call `newtraindata(&data, &net)` to allocate data.in and data.results.  
Their shapes are:  
net.inputs values per sample, and  
number of neurons in the last layer as expected output size.  
You fill these matrices manually.

7. **Train the network**  
Use `backpropagation(&net, &data)`.  
Weights and biases update until reaching tolerance or max_attempts.  
Returns the number of attempts used.

8. **Evaluate results**  
Assign `net.in[j]` to point at your input values, then call feedforward(&net).  
The function returns a pointer to the final output array.  
How you feed values is up to you—in[] is just a pointer array.

9. **Save or reload a network**  
Call `savenet(&net, "filename")` to serialize the network into a `.ntic` file.  
Use `loadnet("filename")` to reconstruct it later.

---

## INDEX

- [Understandig NeuroTIC](Understanding_NeuroTIC.md)

### BASIC FUNCTIONS

- [Compilig tool](../README.md)
- [Workspace](Create_project.md)

### KNOWING THE SYSTEM

- ntcore.h
- [Activation Functions](Activation_functions.md)
- ntmemory.h

### Custom configurations

- [Wiring system](Wiring.md)
