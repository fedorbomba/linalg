#include "linalg/matrix.hpp"

#include <algorithm>

namespace linalg {

// --- constructors ---

Matrix::Matrix() noexcept : m_ptr(nullptr), m_rows(0), m_columns(0), m_capacity(0) {}

// Матрица из rows строк и одного столбца.
Matrix::Matrix(std::size_t rows) : Matrix(rows, 1) {}

Matrix::Matrix(std::size_t rows, std::size_t columns)
    : m_ptr(nullptr), m_rows(rows), m_columns(columns), m_capacity(rows * columns) {
    if (m_capacity != 0) {
        m_ptr = new double[m_capacity];
    }
    fill_zero();
}

Matrix::~Matrix() noexcept {
    destroy();
}

// --- state ---

bool Matrix::empty() const noexcept {
    return size() == 0;
}

std::size_t Matrix::rows() const noexcept {
    return m_rows;
}

std::size_t Matrix::columns() const noexcept {
    return m_columns;
}

std::size_t Matrix::capacity() const noexcept {
    return m_capacity;
}

std::size_t Matrix::size() const noexcept {
    return m_rows * m_columns;
}

void Matrix::reserve(std::size_t new_capacity) {
    if (new_capacity > m_capacity) {
        reallocate(new_capacity);
    }
}

// --- private helpers ---

void Matrix::destroy() noexcept {
    delete[] m_ptr;
    m_ptr = nullptr;
    m_rows = 0;
    m_columns = 0;
    m_capacity = 0;
}

void Matrix::reallocate(std::size_t new_capacity) {
    double* data = nullptr;
    if (new_capacity != 0) {
        data = new double[new_capacity];
        const std::size_t copy_count = std::min(size(), new_capacity);
        for (std::size_t i = 0; i < copy_count; ++i) {
            data[i] = m_ptr[i];
        }
    }
    delete[] m_ptr;
    m_ptr = data;
    m_capacity = new_capacity;
}

void Matrix::fill_zero() {
    for (std::size_t i = 0; i < size(); ++i) {
        m_ptr[i] = 0.0;
    }
}

}  // namespace linalg
