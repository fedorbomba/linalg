#include "linalg/matrix.hpp"

#include <algorithm>
#include <cstdio>
#include <iomanip>
#include <string>
#include <vector>

namespace linalg {

namespace {

std::string to_text(double value) {
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%g", value);
    return std::string(buffer);
}

}  // namespace

std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
    const std::size_t rows = matrix.rows();
    const std::size_t columns = matrix.columns();
    if (rows == 0 || columns == 0) {
        return out;
    }

    // Сначала превращаем все числа в строки, чтобы узнать ширину
    // каждого столбца и вывести элементы выровненными.
    std::vector<std::string> text(rows * columns);
    std::vector<std::size_t> width(columns, 1);
    for (std::size_t row = 0; row < rows; ++row) {
        for (std::size_t column = 0; column < columns; ++column) {
            text[row * columns + column] = to_text(matrix(row, column));
            width[column] = std::max(width[column], text[row * columns + column].size());
        }
    }

    for (std::size_t row = 0; row < rows; ++row) {
        out << '|';
        for (std::size_t column = 0; column < columns; ++column) {
            const int size_of_column = static_cast<int>(width[column]);
            out << std::setw(size_of_column) << text[row * columns + column];
            if (column + 1 < columns) {
                out << ' ';
            }
        }
        out << '|';
        if (row + 1 < rows) {
            out << '\n';
        }
    }
    return out;
}

}  // namespace linalg
