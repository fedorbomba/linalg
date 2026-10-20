#pragma once

#include <cstddef>

#include "linalg/matrix.hpp"

namespace linalg {

// --- element wise operators ---
Matrix operator+(const Matrix& left, const Matrix& right);
Matrix operator-(const Matrix& left, const Matrix& right);

}  // namespace linalg
