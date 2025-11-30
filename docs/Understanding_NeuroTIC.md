# Understanding NeuroTIC

*A conceptual view of how neural networks live inside the framework*  

NeuroTIC treats a neural network as a collection of explicit parts wired together through pointers. The system emphasizes transparency and flexibility: connections are explicit, buffers are routing maps, and neurons hold their own parameters while reading inputs by reference.

This page describes the conceptual model you should keep in mind while using the framework.

---

## 1. Neurons: the smallest unit

A neuron is a compact computational unit that contains:

* its own weight array and bias (local parameters),
* a pointer array that declares where each input comes from,
* an activation index that selects how it transforms its weighted sum,
* a storage slot for the last computed output.

Inputs are never copied into the neuron. Instead, each input is a `float*` that points to a live value somewhere else: a network input, another neuron’s output, a buffer entry, or an external variable. Because neurons own their parameters but reference inputs, they combine local state with external wiring.

**Concept:** a neuron is a local parameter holder and a pointer-driven processor.

---

## 2. Buffers: corridors of references

Buffers do not store numeric values. They store references to values. Every buffer cell is a pointer that resolves to a float somewhere else, or—when buffers are aliased—a pointer array reused across locations.

There is always at least one buffer between adjacent layers. The shape and origin of each buffer depend on its declared type.

**Concept:** buffers are routing tables that map neuron inputs to real data sources.

---

## 3. Buffer types (user-friendly view)

**Mixed buffers (‘M’)**  

* The default and most flexible buffer type.
* The framework allocates a pointer array for the buffer.
* Each element has its own `src_type` that determines where it points (network input, neuron output, network output, or an external source).
* If an element’s `src_type` is unknown, that pointer is initialized to `NULL` and can be assigned manually by the user.

**Shared buffers (non-‘M’ types such as ‘N’, ‘I’, ‘O’)**  

* These do not allocate a new pointer array.
* Instead, they become aliases to an existing pointer array: an earlier buffer, the network input pointer array, or the network output pointer array.
* Aliasing avoids duplication and lets multiple layers read the exact same pointers.

**Undefined / custom types**  

* If a buffer type is not recognized, the builder leaves `net->bff[i][j]` as `NULL` and sets a placeholder size.
* The user must allocate or assign a pointer array and set the buffer size and any affected neuron `.inputs` values manually.
* This mechanism acts as an explicit extension port for wiring external sources or unusual topologies.

**Concept:** mixed buffers are per-element wiring panels; shared buffers mirror existing pointer arrays; custom buffers are manual expansion ports.

---

## 4. Wiring: the topology blueprint

Wiring descriptors declare how buffers relate to neuron inputs. They describe:

* how many buffer arrays exist between two layers,
* the type of each buffer array,
* for mixed buffers, the source type/layer/index of each element.

Wiring is the declarative plan used by `buildnet()` to allocate pointer arrays, resolve aliases, and link `neuron.in` to the correct buffer pointers.

**Concept:** wiring is the blueprint; buffers and pointers are the built result.

---

## 5. The network container (`net_s`)

A `net_s` instance is the assembled machine that groups neurons, wires, and buffers:

* It exposes input pointer ports and output pointer ports.
* It owns the neuron matrix and most runtime buffers.
* It holds wiring descriptors used to build the pointer graph.

After `buildnet()` runs, the network’s internal pointers are resolved and memory layout remains stable until the network is destroyed or modified explicitly.

**Concept:** `net_s` is a self-contained device with visible ports and owned internal memory.

---

## 6. Execution model (high level)

1. External inputs are provided by assigning addresses into the network’s input pointer array.
2. `buildnet()` resolves wiring into concrete pointer arrays and assigns each neuron’s `in` pointer array.
3. During evaluation, each neuron reads its inputs by dereferencing its `in` pointers, computes its weighted sum, applies its activation, and stores the result in its `out` slot.
4. Buffers are the routes used to compose inputs for downstream neurons; shared buffers and aliases mean multiple consumers can read the same pointer array.

There is no implicit value copying or caching: data flows by pointer resolution and direct dereference.

**Concept:** execution is literal pointer-driven propagation.

---

## 7. Composing multiple networks

`net_s` instances are modular: you can combine multiple networks in different arrangements while preserving each instance’s memory ownership.

* **Parallel**: several networks share the same input pointers. Each network processes identical stimuli independently.
* **Series**: the output pointers of one network can be used as the input pointers of another, creating processing stages.
* **Derived**: a network can read from a specific buffer or internal value of another network (not necessarily the former’s public input array), enabling partial or hierarchical wiring.

All inter-network connections are explicit pointer assignments; no internal structure is shared implicitly.

**Concept:** networks are composable modules connected by pointers.

---

## 8. Practical implications for users

* You can inspect and mutate every pointer at runtime.
* You must manage allocation and lifetime for custom buffer pointers and any external sources you attach.
* Mixed buffers make it easy to build heterogenous topologies; shared buffers let you reuse pointer arrays efficiently.
* Undefined buffer slots provide a safe expansion point for integrating external data or hardware.
