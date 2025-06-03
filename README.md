# Long Arithmetic

High-performance C++ library for arbitrary precision integer arithmetic operations.

Features
- Basic arithmetic operations (add, subtract, multiply, divide, modulo)
- Comparison operations
- Optimized algorithms for large numbers
- Procedural C-style API

# Build

Requirements:
- C++20 or higher
- CMake 3.10+
- Git

```bash
git clone git@github.com:literally-bug-creator/long-arithmetic.git
cd long-arithmetic
git submodule update --init --recursive
mkdir build && cd build
cmake ..
cmake --build .
```

# Usage

## Tests
```bash
./build/tests/run_tests
```

## Benchmarks

```bash
./build/benchmark/run_benchmark
```
