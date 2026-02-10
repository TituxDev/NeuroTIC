# NeuroTIC User Guide

## Introduction

NeuroTIC is a modular framework for designing and training neural networks in C. It enables rapid network assembly, free experimentation, and complete inspection of every computational component. Nothing is hidden; all operations are transparent.

This framework focuses on practical interaction with neural network mechanics rather than theoretical instruction. Learn by building architectures, modifying behaviors, and observing training evolution.

## Quick Start

### 1. Create Your First Project

Begin by setting up a new project using the interactive creator:

[**Create a New Project**](Create_project.md)

### 2. Explore Working Examples

Understand the framework through practical examples:

[**Logic Gates Example**](../examples/logic_gates.c) - Complete implementation learning all 16 binary logic functions.

## Framework Scope

NeuroTIC specializes in:

- Network structure and wiring mechanics
- Activation behaviors and training routines  
- Transparent, inspectable computation

The framework treats inputs and outputs as pure numerical values, leaving data interpretation, dataset management, and high-level AI logic to the user. This design maintains generality across logic tables, regression models, and custom topologies.

## Internal Network Representation

NeuroTIC represents networks as concrete data structures, not opaque objects. All computational elements reside directly within `net_s` instances.

### Core Structural Elements

#### Neurons (`neuron_s`)

Each neuron contains:

- Input connections and weights
- Bias term and activation function
- Runtime output value
- Buffer system references

Neurons are organized in a layer × neuron matrix structure.

#### Wiring (`wiring_s`)

This descriptor governs inter-layer value movement:

- Buffer array definitions and sizes
- Data source specifications
- Connection pattern descriptions

Standard layouts are available via helpers, but custom wiring remains fully supported.

#### Buffer System

Intermediate values reside in managed buffers:

- Multi-level pointer access via `bff` field
- Memory sharing between layers
- Independent region allocation

### Design Principles

- **Explicit Configuration**: All parameters are directly accessible
- **Runtime Stability**: No memory reallocation after construction
- **Chainable API**: Functions return network pointers for operation chaining

