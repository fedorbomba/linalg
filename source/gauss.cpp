#include "linalg/matrix.hpp"

#include <cmath>
#include <stdexcept>

namespace linalg {

namespace {

bool is_zero(double value) {
    return std::abs(value) < PIVOT_EPS;
}

}  // namespace

void Matrix::swap_rows(std::size_t first, std::size_t second) {
    if (first == second) {
        return;
    }
    for (std::size_t column = 0; column < m_columns; ++column) {
        const double saved = element(first, column);
        element(first, column) = element(second, column);
        element(second, column) = saved;
    }
}

void Matrix::subtract_row(std::size_t target, std::size_t source, double factor) {
    for (std::size_t column = 0; column < m_columns; ++column) {
        element(target, column) -= factor * element(source, column);
    }
}

std::size_t Matrix::make_row_echelon() {
    std::size_t swaps = 0;
    std::size_t pivot_row = 0;
    for (std::size_t column = 0; column < m_columns && pivot_row < m_rows; ++column) {
        // Ищем строку с самым большим по модулю элементом в столбце: делить на
        // маленький ведущий элемент нельзя, погрешность станет огромной.
        std::size_t best = pivot_row;
        for (std::size_t row = pivot_row + 1; row < m_rows; ++row) {
            if (std::abs(element(row, column)) > std::abs(element(best, column))) {
                best = row;
            }
        }
        if (is_zero(element(best, column))) {
            continue;  // весь столбец нулевой, ведущего элемента здесь нет
        }
        swap_rows(pivot_row, best);
        if (best != pivot_row) {
            ++swaps;
        }
        for (std::size_t row = pivot_row + 1; row < m_rows; ++row) {
            if (is_zero(element(row, column))) {
                continue;
            }
            const double factor = element(row, column) / element(pivot_row, column);
            subtract_row(row, pivot_row, factor);
            element(row, column) = 0.0;  // обнуляем точно, без погрешности деления
        }
        ++pivot_row;
    }
    return swaps;
}

Matrix& Matrix::gauss_forward() {
    make_row_echelon();
    return *this;
}

double Matrix::norm() const {
    double sum = 0.0;
    for (std::size_t i = 0; i < size(); ++i) {
        sum += m_ptr[i] * m_ptr[i];
    }
    return std::sqrt(sum);
}

double Matrix::trace() const {
    if (m_rows != m_columns) {
        throw std::runtime_error("trace: matrix is not square");
    }
    double sum = 0.0;
    for (std::size_t i = 0; i < m_rows; ++i) {
        sum += element(i, i);
    }
    return sum;
}

}  // namespace linalg
