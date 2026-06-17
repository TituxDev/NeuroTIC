# NeuroTIC

A lightweight C library for building, training, saving and loading
neural networks from scratch — no dependencies, no frameworks, just
clean C11.

Built for developers who want to understand and control every layer
of their network, without fighting a framework to do it.

**What you can do with it:**
- Define any topology: layers, neurons, activation functions and
  connections are fully yours to configure
- Train networks and serialize trained models to portable `.ntic`
  binary files
- Load and run models in any C project — from servers to
  microcontrollers — with zero external dependencies

## Quick example

```c
// Full working example: logic gate network
```
[See complete example](./examples/logic_gates.c)

## Documentation

[User Guide](https://tituxdev.github.io/NeuroTIC/docs/doxy/result/html/index.html)

## Learn how it works from scratch

NeuroTIC was built as part of a hands-on learning project.

- [NeuroticLab](https://neuroticlab.com) — step-by-step guide in C
- [Book on Amazon](https://a.co/d/0dphqSgm) — the same journey in book format
- [NeuroSys](https://neurosys.onrender.com) — design and export `.ntic` files in your browser

## License

NeuroTIC is free for personal, educational and open source use under
[GPL-3.0](./LICENSE).  
For commercial use, see [commercial license](./COMMERCIAL_LICENSE.md).