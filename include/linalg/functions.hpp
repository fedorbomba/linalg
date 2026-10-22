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

// единичная матрица n x n
Matrix identity(std::size_t n);

Matrix transpose(const Matrix& source);

// склеивает по горизонтали [left | right], строк должно быть поровну
Matrix concatenate(const Matrix& left, const Matrix& right);

// обратная матрица; для вырожденной - std::runtime_error
Matrix invert(const Matrix& source);

// целая степень; при отрицательном показателе берётся обратная матрица
Matrix power(const Matrix& base, int exponent);

// решает систему A * x = f, f - матрица столбец
Matrix solve(const Matrix& coefficients, const Matrix& right_side);

}  // namespace linalg
