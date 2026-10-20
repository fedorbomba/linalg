#pragma once

#include <cstddef>

#include "linalg/matrix.hpp"

namespace linalg {

// --- unary operators ---
Matrix operator+(const Matrix& source);
Matrix operator-(const Matrix& source);

// --- element wise operators ---
Matrix operator+(const Matrix& left, const Matrix& right);
Matrix operator-(const Matrix& left, const Matrix& right);
Matrix operator*(double factor, const Matrix& source);
Matrix operator*(const Matrix& source, double factor);

// --- matrix product ---
Matrix operator*(const Matrix& left, const Matrix& right);

// --- comparison with EPS accuracy ---
bool operator==(const Matrix& left, const Matrix& right);
bool operator!=(const Matrix& left, const Matrix& right);

// --- global functions ---
void swap(Matrix& first, Matrix& second) noexcept;

// Единичная матрица размера n x n.
Matrix identity(std::size_t n);

// Транспонирование.
Matrix transpose(const Matrix& source);

// Соединение двух матриц с одинаковым числом строк: [left | right].
Matrix concatenate(const Matrix& left, const Matrix& right);

}  // namespace linalg
