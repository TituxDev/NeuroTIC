---
 @file README.md

 NeuroTIC introduction
---

# NeuroTIC

A framework for understanding, exploring, and experimenting with neural networks. `Built in C`, it gives you full visibility into the source code and the freedom to modify, extend, and hack every component.

**NeuroTIC** helps you design, build, train, save, and load networks with complete creative control over their topology—without dragging you through the usual low-level chores.

This workspace is meant for people who care about transparency and control: developers, students, and tinkerers who want to understand neural networks from the inside out and shape them on their own terms.

## About the framework

**NeuroTIC** offers a modular environment where every part of a neural network—layers, neurons, activation flows, learning rules, serialization—is open and accessible.

The design is intentionally minimal: you get a clean API for common tasks, but nothing is hidden.

## Architecture overview

The core idea behind NeuroTIC is simple:
a neural network should be fully visible, fully editable, and never trapped inside a rigid template.

The framework arranges neurons, layers, and data flows in a compact structure where everything is explicit. Nothing happens behind your back, and anything you dislike can be rewritten on the spot.

At the center sits the `net_s` structure. It exposes the external input ports, records the layer count and the neuron count per layer, stores the entire neuron matrix, and carries the wiring descriptors and buffer system that shuttle values between layers. After the build step the network has all connections resolved, the output ports assigned, and the internal memory under its control. External inputs—or any custom data sources you plug in—remain entirely yours. In practice, net_s becomes the fully inspectable device you run, train, save, reload, and tinker with.

Helper modules step in for convenience—dense wiring, feed-forward layouts, activation dispatch, weight initialization, training routines, persistence—but they’re always optional. They accelerate your work without limiting what you can build.

Because the network stays transparent at every stage, you can:

- assemble a classic dense model with a single call,
- mix automatic wiring with handcrafted connections,
- shape exotic topologies layer by layer,
- poke weights and activation paths directly,
- experiment with training rules outside the usual canon,
- and run inference without a single runtime allocation.

NeuroTIC avoids the black-box mentality altogether. Everything arrives as plain C code sitting inside your project, ready for inspection, extension, mutation, or the occasional glorious detonation in the name of learning.

[USER GUIDE](docs/doxy/html/index.html)

## Quick use guide

### 1. Install

Download the makefile from the repository and run:

```sh
make install 
```

This checks for required tools (gcc, ar) and offers to install them if missing. It will also guide you through cloning the repository.

### 2. Compile an example

To compile examples/logic_gates.c:

```sh
make compile PROJECT_LOCATION=examples PROJECT_NAME=logic_gates PLATFORM=CPU
```

Alternatively, run `make compile` with no arguments and follow the prompts:

```sh
Enter project location (e.g., workspace, examples): examples
Enter project name (filename without extension): logic_gates
Enter platform (e.g., CPU): CPU
```

**NOTE:** *This only compiles the source code into an executable; it does not run it.*

**Expected compile log**  

```sh
Compiling project in location: examples
Project name: logic_gates
Platform: CPU
```

### 3. Run the example

```sh
./examples/logic_gates
```

The program will:

- Train a neural network that learns all 16 possible binary logic functions simultaneously
- Save the trained network to a `.ntic` file
- Load the network back into memory
- Run both the trained and reloaded versions for comparison

The console will print two complete truth tables, one for the live network and one for the loaded copy.
Each table includes:

> A, B, NULL, NOR, EXA, NOTB, EXB, NOTA, XOR, NAND, AND, XNOR, A, IMPB, B, IMPA, OR, ALL

Every cell shows a 0 or 1 according to the network’s predictions.

### 4. Clean up

```sh
rm examples/logic_gates
```

### 5. Shortcut

A helper script is available inside `examples/`:

```sh
cd examples
bash ./test.sh logic_gates
```

This compiles, runs, and removes the binary automatically.

### 6. Start your first project

Ready to build your own?  
Check out:
[Create project](docs/Create_project.md)

## Author

Oscar Sotomayor (Titux) – Creator and maintainer.

## Licensing

NeuroTIC is available under dual licensing to support both open source and commercial use.

### Open Source (GPLv3)

This software is available under the GNU GPLv3 license for open source projects, academic use, and personal projects.

### Commercial License

For companies needing proprietary usage, a [commercial license](COMERCIAL_LICESE.md) is available.  
[Future implementations](ROADMAP.md)

## Contact

 ocrstmy@gmail.com for commercial licensing.