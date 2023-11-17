# SimpleMultithreader

SimpleMultithreader is a lightweight C++ multithreading library that enables easy parallelization of tasks using Pthreads. It is designed to be simple to use, allowing developers to parallelize loops and other operations without dealing with the complexity of managing thread pools or task queues.

## Getting Started

### Prerequisites

Make sure you have the following prerequisites installed:

- C++ compiler
- Pthreads library

### Usage

1. Include the "simple-multithreader.h" header file in your C++ code.

2. Create an instance of the `SimpleMultithreader` class.

3. Use the `parallel_for` method to parallelize loops or other tasks.

```cpp
#include "simple-multithreader.h"

int main() {
    SimpleMultithreader multithreader;

    multithreader.parallel_for(0, 100, [](int i) {
        // Your parallelized task here
        // For example, process element i
    }, 4); // 4 is the number of threads

    return 0;
}
```
# Examples
Check the "vector.cpp" and "matrix.cpp" files for examples of using SimpleMultithreader to perform parallel addition of vectors and multiplication of matrices.

# Project Structure
- simple-multithreader.h: Header file containing the implementation of the SimpleMultithreader class.
- vector.cpp: Example usage of SimpleMultithreader for parallel vector addition.
- matrix.cpp: Example usage of SimpleMultithreader for parallel matrix multiplication.
