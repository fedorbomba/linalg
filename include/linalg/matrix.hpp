#pragma once

#include <cstddef>
#include <initializer_list>
#include <iosfwd>

namespace linalg {

// точность сравнения double: 0.3 * 3 и 0.9 считаем равными
const double EPS = 1e-9;

// что меньше этого числа - для нас ноль при поиске ведущего
const double PIVOT_EPS = 1e-12;

// Матрица произвольного размера. Все элементы лежат в одном массиве m_ptr
// построчно, элемент (i, j) - это m_ptr[i * m_columns + j].
// Память берём с запасом, как у std::vector: m_capacity - сколько влезет
// сейчас, а m_rows * m_columns - сколько реально занято.
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
    Matrix& operator*=(const Matrix& other);
    Matrix& operator*=(double factor);

    // --- calculations ---
    double norm() const;
    double trace() const;
    double det() const;
    std::size_t rank() const;
    Matrix& gauss_forward();
    Matrix& gauss_backward();

private:
    // доступ без проверки границ, только для кода внутри библиотеки
    double& element(std::size_t row, std::size_t column) {
        return m_ptr[row * m_columns + column];
    }
    const double& element(std::size_t row, std::size_t column) const {
        return m_ptr[row * m_columns + column];
    }

    void destroy() noexcept;
    // выделяет массив на new_capacity элементов и переносит старое содержимое;
    // старую память освобождаем только после удачного new
    void reallocate(std::size_t new_capacity);
    void fill_zero();

    // прямой ход к ступенчатому виду: возвращает число перестановок строк,
    // а в pivot_product пишет произведение ведущих элементов (нужно det())
    std::size_t make_row_echelon(double& pivot_product);
    // стоит ли матрица уже в ступенчатом виде, то есть после прямого хода
    bool is_row_echelon() const;
    void swap_rows(std::size_t first, std::size_t second);
    // из строки target вычитает factor * строку source
    void subtract_row(std::size_t target, std::size_t source, double factor);

    double* m_ptr;
    std::size_t m_rows;
    std::size_t m_columns;
    std::size_t m_capacity;
};

// вывод матрицы в поток, элементы выровнены по столбцам
std::ostream& operator<<(std::ostream& out, const Matrix& matrix);

}  // namespace linalg
