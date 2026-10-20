#include "linalg/functions.hpp"
#include "linalg/matrix.hpp"

#include <cmath>
#include <stdexcept>

namespace linalg {

namespace {

// Матрица уже приведена к ступенчатому виду. Если на диагонали встретился
// ноль, то строки линейно зависимы и обратной матрицы не существует.
bool is_singular(const Matrix& echelon, std::size_t size) {
    for (std::size_t i = 0; i < size; ++i) {
        if (std::abs(echelon(i, i)) < PIVOT_EPS) {
            return true;
        }
    }
    return false;
}

}  // namespace

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

Matrix invert(const Matrix& source) {
    if (source.rows() != source.columns()) {
        throw std::runtime_error("invert: matrix is not square");
    }
    const std::size_t size = source.rows();
    if (size == 0) {
        throw std::runtime_error("invert: matrix is empty");
    }

    // Пишем рядом [ исходная матрица | единичная ] и преобразовываем обе части
    // одинаковыми действиями. Когда слева получится единичная, справа будет
    // ровно такая же комбинация элементарных преобразований, то есть обратная.
    Matrix wide(size, 2 * size);
    for (std::size_t row = 0; row < size; ++row) {
        for (std::size_t column = 0; column < size; ++column) {
            wide(row, column) = source(row, column);
        }
        wide(row, size + row) = 1.0;
    }

    wide.gauss_forward();
    if (is_singular(wide, size)) {
        throw std::runtime_error("invert: matrix is singular");
    }
    wide.gauss_backward();

    Matrix result(size, size);
    for (std::size_t row = 0; row < size; ++row) {
        for (std::size_t column = 0; column < size; ++column) {
            result(row, column) = wide(row, size + column);
        }
    }
    return result;
}

}  // namespace linalg
