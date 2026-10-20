#pragma once

#include <cstddef>
#include <initializer_list>

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
    Matrix(std::initializer_list<double> values);
    Matrix(std::initializer_list<std::initializer_list<double>> values);
    ~Matrix() noexcept;

    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;
    Matrix& operator=(std::initializer_list<double> values);
    Matrix& operator=(std::initializer_list<std::initializer_list<double>> values);

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
    void swap(Matrix& other) noexcept;

    // --- element access, throws std::out_of_range ---
    double& operator()(std::size_t row, std::size_t column);
    const double& operator()(std::size_t row, std::size_t column) const;

    // --- arithmetic assignment ---
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(double factor);

private:
    // Доступ без проверки границ. Используется внутри библиотеки,
    // где индексы уже заведомо правильные.
    double& element(std::size_t row, std::size_t column) {
        return m_ptr[row * m_columns + column];
    }
    const double& element(std::size_t row, std::size_t column) const {
        return m_ptr[row * m_columns + column];
    }

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
