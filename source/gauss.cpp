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

bool Matrix::is_row_echelon() const {
    int previous_column = -1;   // столбец предыдущего ведущего элемента
    bool seen_zero_row = false;
    for (std::size_t row = 0; row < m_rows; ++row) {
        int leading = -1;
        for (std::size_t column = 0; column < m_columns; ++column) {
            if (!is_zero(element(row, column))) {
                leading = static_cast<int>(column);
                break;
            }
        }
        if (leading == -1) {
            seen_zero_row = true;   // нулевые строки должны лежать снизу
            continue;
        }
        if (seen_zero_row || leading <= previous_column) {
            return false;
        }
        previous_column = leading;
    }
    return true;
}

std::size_t Matrix::make_row_echelon(double& pivot_product) {
    std::size_t swaps = 0;
    std::size_t pivot_row = 0;
    for (std::size_t column = 0; column < m_columns && pivot_row < m_rows; ++column) {
        // берём строку с максимальным по модулю элементом: делить на маленький
        // ведущий нельзя, погрешность станет огромной
        std::size_t best = pivot_row;
        for (std::size_t row = pivot_row + 1; row < m_rows; ++row) {
            if (std::abs(element(row, column)) > std::abs(element(best, column))) {
                best = row;
            }
        }
        if (is_zero(element(best, column))) {
            continue;  // нулевой столбец, ведущего тут нет
        }
        swap_rows(pivot_row, best);
        if (best != pivot_row) {
            ++swaps;
        }
        // делим ведущую строку на её ведущий элемент, чтобы на диагонали
        // осталась единица; множитель сохраняем - без него det не собрать
        const double pivot = element(pivot_row, column);
        pivot_product *= pivot;
        for (std::size_t col = column; col < m_columns; ++col) {
            element(pivot_row, col) /= pivot;
        }
        element(pivot_row, column) = 1.0;
        for (std::size_t row = pivot_row + 1; row < m_rows; ++row) {
            if (is_zero(element(row, column))) {
                continue;
            }
            const double factor = element(row, column) / element(pivot_row, column);
            subtract_row(row, pivot_row, factor);
            element(row, column) = 0.0;  // ставим точный ноль, без погрешности
        }
        ++pivot_row;
    }
    return swaps;
}

Matrix& Matrix::gauss_forward() {
    double unused = 1.0;
    make_row_echelon(unused);
    return *this;
}

Matrix& Matrix::gauss_backward() {
    if (!is_row_echelon()) {
        throw std::runtime_error("gauss_backward: run gauss_forward first");
    }
    std::size_t pivot_row = 0;
    for (std::size_t column = 0; column < m_columns && pivot_row < m_rows; ++column) {
        if (is_zero(element(pivot_row, column))) {
            continue;  // ведущий в этой строке стоит правее
        }
        const double pivot = element(pivot_row, column);
        for (std::size_t row = 0; row < m_rows; ++row) {
            if (row == pivot_row || is_zero(element(row, column))) {
                continue;
            }
            subtract_row(row, pivot_row, element(row, column) / pivot);
            element(row, column) = 0.0;
        }
        for (std::size_t column_of_pivot = 0; column_of_pivot < m_columns; ++column_of_pivot) {
            element(pivot_row, column_of_pivot) /= pivot;
        }
        ++pivot_row;
    }
    return *this;
}

double Matrix::norm() const {
    if (empty()) {
        throw std::runtime_error("norm: matrix is empty");
    }
    double sum = 0.0;
    for (std::size_t i = 0; i < size(); ++i) {
        sum += m_ptr[i] * m_ptr[i];
    }
    return std::sqrt(sum);
}

double Matrix::trace() const {
    if (empty()) {
        throw std::runtime_error("trace: matrix is empty");
    }
    if (m_rows != m_columns) {
        throw std::runtime_error("trace: matrix is not square");
    }
    double sum = 0.0;
    for (std::size_t i = 0; i < m_rows; ++i) {
        sum += element(i, i);
    }
    return sum;
}

double Matrix::det() const {
    if (empty()) {
        throw std::runtime_error("det: matrix is empty");
    }
    if (m_rows != m_columns) {
        throw std::runtime_error("det: matrix is not square");
    }
    Matrix work(*this);
    double pivot_product = 1.0;
    const std::size_t swaps = work.make_row_echelon(pivot_product);
    // ведущий для нулевой строки не делится, поэтому вырожденная матрица
    // узнаётся по нулю на диагонали после прямого хода
    for (std::size_t i = 0; i < m_rows; ++i) {
        if (std::abs(work.element(i, i)) < PIVOT_EPS) {
            return 0.0;
        }
    }
    const double sign = (swaps % 2 == 0) ? 1.0 : -1.0;
    return sign * pivot_product;
}

std::size_t Matrix::rank() const {
    Matrix work(*this);
    double unused = 1.0;
    work.make_row_echelon(unused);
    std::size_t result = 0;
    for (std::size_t row = 0; row < work.m_rows; ++row) {
        bool non_zero_row = false;
        for (std::size_t column = 0; column < work.m_columns; ++column) {
            if (!is_zero(work.element(row, column))) {
                non_zero_row = true;
            }
        }
        if (non_zero_row) {
            ++result;
        }
    }
    return result;
}

}  // namespace linalg
