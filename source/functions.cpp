#include "linalg/functions.hpp"
#include "linalg/matrix.hpp"
#include <stdexcept>

namespace linalg {

void swap(Matrix& first, Matrix& second) noexcept {
    first.swap(second);
}

Matrix identity(std::size_t size) {
    Matrix result(size, size);
    for (std::size_t i = 0; i < size; ++i) {
        result(i, i) = 1.0;
    }
    return result;
}

Matrix transpose(const Matrix& source) {
    Matrix result(source.columns(), source.rows());
    for (std::size_t row = 0; row < source.rows(); ++row) {
        for (std::size_t column = 0; column < source.columns(); ++column) {
            result(column, row) = source(row, column);
        }
    }
    return result;
}

Matrix concatenate(const Matrix& left, const Matrix& right) {
    if (left.rows() != right.rows()) {
        throw std::runtime_error("concatenate: matrices have different row counts");
    }
    Matrix result(left.rows(), left.columns() + right.columns());
    for (std::size_t row = 0; row < left.rows(); ++row) {
        for (std::size_t column = 0; column < left.columns(); ++column) {
            result(row, column) = left(row, column);
        }
        for (std::size_t column = 0; column < right.columns(); ++column) {
            result(row, left.columns() + column) = right(row, column);
        }
    }
    return result;
}

}  // namespace linalg
