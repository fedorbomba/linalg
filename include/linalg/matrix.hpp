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
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    ~Matrix() noexcept;

    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;

    // --- state ---
    bool empty() const noexcept;
    std::size_t rows() const noexcept;
    std::size_t columns() const noexcept;
    std::size_t capacity() const noexcept;
    std::size_t size() const noexcept;
    double* begin() noexcept;
    double* end() noexcept;
    const double* begin() const noexcept;
    const double* end() const noexcept;

    // --- size and capacity ---
    void reshape(std::size_t rows, std::size_t columns);
    void reserve(std::size_t new_capacity);
    void clear() noexcept;
    void shrink_to_fit();

private:

    void destroy() noexcept;
    // Выделяет массив на new_capacity элементов и переносит в него старое
    // содержимое. Старая память освобождается только после удачного new.
    void reallocate(std::size_t new_capacity);
    void fill_zero();

    double* m_ptr;
    std::size_t m_rows;
    std::size_t m_columns;
    std::size_t m_capacity;
};

}  // namespace linalg
