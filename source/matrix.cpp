#include "linalg/matrix.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace linalg {

namespace {

// Длина строки в списке вида { {1, 2}, {3, 4} }.
// Все строки должны быть одной длины, иначе матрицы не получится.
std::size_t row_width(std::initializer_list<std::initializer_list<double>> values) {
    if (values.size() == 0) {
        return 0;
    }
    const std::size_t width = values.begin()->size();
    for (const auto& row : values) {
        if (row.size() != width) {
            throw std::runtime_error("Matrix: rows have different sizes");
        }
    }
    return width;
}

}  // namespace

// --- constructors ---

Matrix::Matrix() noexcept : m_ptr(nullptr), m_rows(0), m_columns(0), m_capacity(0) {}

// Матрица из rows строк и одного столбца.
Matrix::Matrix(std::size_t rows) : Matrix(rows, 1) {}

Matrix::Matrix(std::size_t rows, std::size_t columns)
    : m_ptr(nullptr), m_rows(rows), m_columns(columns), m_capacity(rows * columns) {
    // Если хотя бы одно измерение равно нулю, элементов нет вообще, поэтому
    // матрица становится пустой 0 x 0, а не, например, 0 x 5.
    if (m_capacity == 0) {
        m_rows = 0;
        m_columns = 0;
        return;
    }
    m_ptr = new double[m_capacity];
    fill_zero();
}

Matrix::Matrix(const Matrix& other) : Matrix(other.m_rows, other.m_columns) {
    for (std::size_t i = 0; i < size(); ++i) {
        m_ptr[i] = other.m_ptr[i];
    }
}

Matrix::Matrix(Matrix&& other) noexcept
    : m_ptr(other.m_ptr),
      m_rows(other.m_rows),
      m_columns(other.m_columns),
      m_capacity(other.m_capacity) {
    other.m_ptr = nullptr;
    other.m_rows = 0;
    other.m_columns = 0;
    other.m_capacity = 0;
}

// Плоский список { 1, 2, 3 } - это матрица столбец, по одному числу в строке.
Matrix::Matrix(std::initializer_list<double> values) : Matrix(values.size(), 1) {
    std::size_t index = 0;
    for (double value : values) {
        m_ptr[index] = value;
        ++index;
    }
}

// Список строк { {1, 2}, {3, 4} } - обычная матрица.
Matrix::Matrix(std::initializer_list<std::initializer_list<double>> values)
    : Matrix(values.size(), row_width(values)) {
    std::size_t index = 0;
    for (const auto& row : values) {
        for (double value : row) {
            m_ptr[index] = value;
            ++index;
        }
    }
}

Matrix::~Matrix() noexcept {
    destroy();
}

// --- assignment ---

Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) {
        return *this;
    }
    // Перевыделяем память только если старого массива не хватает на new размер.
    if (m_capacity < other.size()) {
        reallocate(other.size());
    }
    m_rows = other.m_rows;
    m_columns = other.m_columns;
    for (std::size_t i = 0; i < size(); ++i) {
        m_ptr[i] = other.m_ptr[i];
    }
    return *this;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this != &other) {
        destroy();
        m_ptr = other.m_ptr;
        m_rows = other.m_rows;
        m_columns = other.m_columns;
        m_capacity = other.m_capacity;
        other.m_ptr = nullptr;
        other.m_rows = 0;
        other.m_columns = 0;
        other.m_capacity = 0;
    }
    return *this;
}

Matrix& Matrix::operator=(std::initializer_list<double> values) {
    return *this = Matrix(values);
}

Matrix& Matrix::operator=(std::initializer_list<std::initializer_list<double>> values) {
    return *this = Matrix(values);
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

double* Matrix::begin() noexcept {
    return m_ptr;
}

double* Matrix::end() noexcept {
    return m_ptr + size();
}

const double* Matrix::begin() const noexcept {
    return m_ptr;
}

const double* Matrix::end() const noexcept {
    return m_ptr + size();
}

// --- size and capacity ---

void Matrix::reshape(std::size_t rows, std::size_t columns) {
    // то же соглашение, что и в конструкторе: нулевое измерение - пустая матрица
    if (rows * columns == 0) {
        rows = 0;
        columns = 0;
    }
    if (rows * columns > m_capacity) {
        reallocate(rows * columns);
    }
    m_rows = rows;
    m_columns = columns;
    fill_zero();
}

void Matrix::reserve(std::size_t new_capacity) {
    if (new_capacity > m_capacity) {
        reallocate(new_capacity);
    }
}

void Matrix::clear() noexcept {
    m_rows = 0;
    m_columns = 0;
}

void Matrix::shrink_to_fit() {
    if (m_capacity != size()) {
        reallocate(size());
    }
}

void Matrix::swap(Matrix& other) noexcept {
    std::swap(m_ptr, other.m_ptr);
    std::swap(m_rows, other.m_rows);
    std::swap(m_columns, other.m_columns);
    std::swap(m_capacity, other.m_capacity);
}

double& Matrix::operator()(std::size_t row, std::size_t column) {
    if (row >= m_rows || column >= m_columns) {
        throw std::out_of_range("Matrix: index out of range");
    }
    return element(row, column);
}

const double& Matrix::operator()(std::size_t row, std::size_t column) const {
    if (row >= m_rows || column >= m_columns) {
        throw std::out_of_range("Matrix: index out of range");
    }
    return element(row, column);
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
