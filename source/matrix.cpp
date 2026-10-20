#include "linalg/matrix.hpp"

namespace linalg {

// --- constructors ---

Matrix::Matrix() noexcept : m_ptr(nullptr), m_rows(0), m_columns(0), m_capacity(0) {}

Matrix::~Matrix() noexcept {
    destroy();
}

// --- private helpers ---

void Matrix::destroy() noexcept {
    delete[] m_ptr;
    m_ptr = nullptr;
    m_rows = 0;
    m_columns = 0;
    m_capacity = 0;
}

}  // namespace linalg
