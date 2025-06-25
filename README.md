# Long Arithmetic

A high-performance C++ library for arbitrary-precision arithmetic (integers and floating-point numbers).
**Educational project:** Learn modular architecture, design, and optimization for fast computations (all operations complete in under a second on an average PC).

# Features

- Arbitrary-precision arithmetic (integer and floating-point)
- Basic operations: addition, subtraction, multiplication, division, modulo
- Comparison operators
- Optimized algorithms for large numbers
- Modular, extensible architecture
- Procedural C-style API
- Comprehensive tests (GoogleTest)
- Performance benchmarks (Google Benchmark)
- All specifications available in the `docs` directory

# Requirements

- C++20 or newer
- CMake 3.10+
- Git
- Make
- **clang++** (default compiler)

# Getting Started

## Clone the Repository

```bash
git clone https://github.com/literally-bug-creator/long-arithmetic.git
cd long-arithmetic
```

Update Submodules

```bash
git submodule update --init --recursive
```

## Build and Run with Make

The project provides a convenient `Makefile` with the following targets:

- **all** (default): Build the project in release mode.
- **release**: Build with optimizations (`Release` mode).
- **debug**: Build with debug information.
- **test**: Run all unit tests (requires build).
- **benchmark**: Run performance benchmarks (requires build).
- **clean**: Remove all build artifacts.

### Example Usage

```bash
make           # Build in release mode
make debug     # Build in debug mode
make test      # Run unit tests
make benchmark # Run benchmarks
make clean     # Clean build directory
```

# Documentation

- All specifications and design documents are in the [`docs`](./docs) directory.

# Contributing

Contributions, issues, and suggestions are welcome!
Feel free to open an issue or submit a pull request.

# License

This project is for educational purposes. See [LICENSE](./LICENSE) for details.
