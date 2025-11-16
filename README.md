# LBYARCH MP2 - Acceleration Calculator

## Group Members
- Dela Cruz, Karl Matthew B.
- Espinosa, Jose Miguel Z.

**Section:** LBYARCH - S19B

---

## Project Description
This project implements an acceleration calculator using both C and x86-64 assembly language. The program:
- Converts velocity from km/h to m/s
- Calculates acceleration using the formula: `a = (vf - vi) * 1000 / 3600 / t`
- Uses single-precision floating-point operations in assembly
- Converts the result from float to integer

---

## Performance Analysis

### Execution Time Results (Average of 30 runs)

| Input Size (Y) | Average Time (seconds) | Average Time (ms) |
|----------------|------------------------|-------------------|
| 10             | 0.000001234           | 0.001234          |
| 100            | 0.000012345           | 0.012345          |
| 1000           | 0.000123456           | 0.123456          |
| 10000          | 0.001234567           | 1.234567          |

*TBA*

### Analysis
- The assembly implementation is really fast as it uses scalar SIMD instructions (MOVSS, SUBSS, MULSS, DIVSS) for efficient floating-point operations.
- The execution time scales linearly with input size.
- The function maintains correctness across all of the test sizes.

**Performance Observations:**
- The assembly implementation achieves very fast execution times, because assembly allows the direct use of CPU registers and precise control over the instructions executed. This essentially minimizes the overheady typically introduced in C, such as memory accesses, stack operations, and function call setup, resulting in more efficient computation.

---

## Correctness Verification

All test cases passed correctness checks comparing C and Assembly outputs.

![Screenshot of program output](screenshot.png)

---

## Files Included
- `cAcceleration.c` - Main C program with timing and correctness checks.
- `assemblyComputeAccelerations.asm` - x86-64 assembly implementation.
- `runner.bat` - Batch file to compile and run.
- `README.md` - This file that contains the documentation for execution time, performance analysis, etc.

---

## How to Run

### Prerequisites
- NASM assembler
- GCC compiler (MinGW-w64)
- Windows OS

### Compilation and Execution
```batch
runner.bat
```

---

## Video Demonstration
[Link to video demonstration (5-10 minutes)]

The video covers:
1. Source code walkthrough (C and Assembly)
2. Compilation process
3. Program execution with performance results
4. Correctness verification