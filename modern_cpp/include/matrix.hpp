#ifndef __MATRIX_GAL_D_90__
#define __MATRIX_GAL_D_90__

#include <memory>
#include <optional>
#include "threadpool.hpp"
#include "task_wrapper.hpp"

template<typename T>
class Matrix
{
private:
    std::size_t m_rows;
    std::size_t m_cols;
    std::unique_ptr<std::vector<T>> m_data;
    mutable ThreadPool m_threadPool;

public:
    explicit Matrix(std::size_t rows, std::size_t cols);

    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);

    Matrix(Matrix&& other);
    Matrix& operator=(Matrix&& other);

    inline std::size_t GetNumRows { return m_rows; }
    inline std::size_t GetNumCols { return m_cols; }

    void SetValue(T value);

    Matrix& operator+=(const Matrix& lhs);
    Matrix& operator-=(const Matrix& lhs);
    Matrix& operator*=(const Matrix& lhs);
    friend Matrix operator+<>(const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator-<>(const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator*<>(const Matrix& lhs, const Matrix& rhs);
    
    Matrix& operator+=(T scalar);
    Matrix& operator-=(T scalar);
    Matrix& operator*=(T scalar);
    Matrix& operator/=(T scalar);

    friend Matrix operator+<>(const Matrix& lhs, T scalar);
    friend Matrix operator-<>(const Matrix& lhs, T scalar);
    friend Matrix operator*<>(const Matrix& lhs, T scalar);
    friend Matrix operator/<>(const Matrix& lhs, T scalar);

    Matrix HadamardProduct(const Matrix& other) const;

    void Transpose();
    std::optional<T> Determinant() const;
    std::optional<Matrix> Inverse() const;
};




#endif //__MATRIX_GAL_D_90__