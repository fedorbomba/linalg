#include "linalg/matrix.hpp"

#include <cmath>

namespace linalg {

double Matrix::norm() const {
    double sum = 0.0;
    for (std::size_t i = 0; i < size(); ++i) {
        sum += m_ptr[i] * m_ptr[i];
    }
    return std::sqrt(sum);
}

}  // namespace linalg
