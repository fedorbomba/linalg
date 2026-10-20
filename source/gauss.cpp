#include "linalg/matrix.hpp"

#include <cmath>
#include <stdexcept>

namespace linalg {

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
