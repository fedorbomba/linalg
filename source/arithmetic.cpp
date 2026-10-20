#include "linalg/functions.hpp"
#include "linalg/matrix.hpp"
#include <stdexcept>

namespace linalg {

// --- member operators ---

Matrix& Matrix::operator+=(const Matrix& other) {
    if (m_rows != other.m_rows || m_columns != other.m_columns) {
        throw std::runtime_error("operator+=: matrices have different sizes");
    }
    for (std::size_t i = 0; i < size(); ++i) {
        m_ptr[i] += other.m_ptr[i];
    }
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    if (m_rows != other.m_rows || m_columns != other.m_columns) {
        throw std::runtime_error("operator-=: matrices have different sizes");
    }
    for (std::size_t i = 0; i < size(); ++i) {
        m_ptr[i] -= other.m_ptr[i];
    }
    return *this;
}

Matrix& Matrix::operator*=(double factor) {
    for (std::size_t i = 0; i < size(); ++i) {
        m_ptr[i] *= factor;
    }
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    if (m_columns != other.m_rows) {
        throw std::runtime_error("operator*=: number of columns must match number of rows");
    }
    Matrix result(m_rows, other.m_columns);
    for (std::size_t row = 0; row < m_rows; ++row) {
        for (std::size_t column = 0; column < other.m_columns; ++column) {
            double sum = 0.0;
            for (std::size_t middle = 0; middle < m_columns; ++middle) {
                sum += element(row, middle) * other.element(middle, column);
            }
            result.element(row, column) = sum;
        }
    }
    // Новый результат собран целиком, поэтому старую матрицу не портим,
    // даже если внутри что-то бросило исключение.
    swap(result);
    return *this;
}

// --- unary operators ---

Matrix operator+(const Matrix& source) {
    return source;
}

Matrix operator-(const Matrix& source) {
    Matrix result(source);
    result *= -1.0;
    return result;
}

// --- binary operators ---

Matrix operator+(const Matrix& left, const Matrix& right) {
    Matrix result(left);
    result += right;
    return result;
}

Matrix operator-(const Matrix& left, const Matrix& right) {
    Matrix result(left);
    result -= right;
    return result;
}

Matrix operator*(const Matrix& left, const Matrix& right) {
    Matrix result(left);
    result *= right;
    return result;
}

Matrix operator*(double factor, const Matrix& source) {
    Matrix result(source);
    result *= factor;
    return result;
}

Matrix operator*(const Matrix& source, double factor) {
    return factor * source;
}

}  // namespace linalg
