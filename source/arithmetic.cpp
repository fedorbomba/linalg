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

Matrix operator*(double factor, const Matrix& source) {
    Matrix result(source);
    result *= factor;
    return result;
}

Matrix operator*(const Matrix& source, double factor) {
    return factor * source;
}

}  // namespace linalg
