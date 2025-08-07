# 🔢 Matrix Inversion in C

This project implements inversion of a square matrix using C. The program reads a matrix from a text file, checks if it is invertible (by computing the determinant), and calculates the inverse matrix.

## 📌 Features

- Matrix input from file
- Determinant calculation
- Inversion using cofactor method (or another implemented in `InverseMatrix`)
- Memory management for dynamic 2D arrays
- Debugging mode using `#define DEBUG`

## 📁 Files

- `macierz.cpp` – main logic and program entry
- `matrix.c`, `matrix.h` – matrix operations (create, delete, print, transpose, determinant, inverse)
- `example_input.txt` – example of matrix input

## 🛠️ How to build and run

```bash
gcc macierz.cpp matrix.c -o matrix_inverse -lm
./matrix_inverse example_input.txt
