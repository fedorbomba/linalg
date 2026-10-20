#pragma once

#include <cstddef>

namespace linalg {

// Матрица произвольного размера (размер можно менять во время работы).
//
// Все элементы лежат в одном динамическом массиве m_ptr, который идёт
// построчно: элемент (i, j) стоит в массиве под номером i * m_columns + j.
// Память выделяется с запасом, как у std::vector: m_capacity - это сколько
// элементов помещается в массив прямо сейчас, а m_rows * m_columns -
// сколько из них реально занято матрицей.
class Matrix {
public:
    // --- constructors and assignment ---
    Matrix() noexcept;
    explicit Matrix(std::size_t rows);
    Matrix(std::size_t rows, std::size_t columns);
    ~Matrix() noexcept;

    // --- state ---
    bool empty() const noexcept;
    std::size_t rows() const noexcept;
    std::size_t columns() const noexcept;
    std::size_t size() const noexcept;

private:

    void destroy() noexcept;
    void fill_zero();

    double* m_ptr;
    std::size_t m_rows;
    std::size_t m_columns;
    std::size_t m_capacity;
};

}  // namespace linalg
