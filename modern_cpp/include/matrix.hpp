#ifndef __MATRIX_GAL_D_90__
#define __MATRIX_GAL_D_90__

#include <vector>
#include <optional>
#include <ranges>
#include <functional>
#include "threadpool.hpp"
#include "task_wrapper.hpp"

template<typename T = float>
class Matrix
{
private:
    std::size_t m_rows;
    std::size_t m_cols;
    std::vector<std::vector<T>> m_data;
    mutable ThreadPool m_threadPool;

public:
    Matrix(std::size_t rows, std::size_t cols);
    Matrix(std::vector<std::vector<T>> data);

    ~Matrix();

    //Copy Ctor
    Matrix(const Matrix& other);

    //Copy Ctor from Matrix of type U
    template<typename U>
    Matrix(const Matrix<U>& other);

    //Assignment Operator
    Matrix& operator=(const Matrix& other);

    //Assignment Operator from Matrix of type U
    template<typename U>
    Matrix& operator=(const Matrix<U>& other);

    Matrix(Matrix&& other);
    Matrix& operator=(Matrix&& other);

    inline std::size_t GetNumRows() { return m_rows; }
    inline std::size_t GetNumCols() { return m_cols; }

    void SetValue(T value);

    Matrix& operator+=(const Matrix& lhs);
    Matrix& operator-=(const Matrix& lhs);
    Matrix& operator*=(const Matrix& lhs);
    //friend Matrix operator+<>(const Matrix& lhs, const Matrix& rhs);
    //friend Matrix operator-<>(const Matrix& lhs, const Matrix& rhs);
    //friend Matrix operator*<>(const Matrix& lhs, const Matrix& rhs);
    
    template<typename Scalar>
    requires std::is_convertible_v<Scalar, T>
    Matrix& operator+=(Scalar scalar);

    template<typename Scalar>
    requires std::is_convertible_v<Scalar, T>
    Matrix& operator-=(T scalar);

    template<typename Scalar>
    requires std::is_convertible_v<Scalar, T>
    Matrix& operator*=(T scalar);

    template<typename Scalar>
    requires std::is_convertible_v<Scalar, T>
    Matrix& operator/=(T scalar);

    //friend Matrix operator+<>(const Matrix& lhs, T scalar);
    //friend Matrix operator-<>(const Matrix& lhs, T scalar);
    //friend Matrix operator*<>(const Matrix& lhs, T scalar);
    //friend Matrix operator/<>(const Matrix& lhs, T scalar);

    Matrix HadamardProduct(const Matrix& other) const;

    void Transpose();
    std::optional<T> Determinant() const;
    std::optional<Matrix> Inverse() const;

    std::vector<std::vector<T>> GetData() const { return m_data; }

private:
    template<typename U>
    class CopyRowFunctor;

    template<typename U>
    void CopyImpl(const Matrix<U>& other);
    
    template<typename U>
    void CopyTask(Matrix<T>* matrix, const Matrix<U>& other, std::size_t startRow, std::size_t endRow);
};

/********************************************************************************/
/*                              Matrix Definition                               */
/********************************************************************************/

template<typename T>
Matrix<T>::Matrix(std::size_t rows, std::size_t cols) : m_rows(rows), m_cols(cols), m_data(rows, std::vector<T>(cols)), m_threadPool()
{
    m_threadPool.Pause();
}

template<typename T>
Matrix<T>::Matrix(std::vector<std::vector<T>> data) :   m_rows{data.size()}, 
                                                        m_cols{data[0].size()}, 
                                                        m_data(std::move(data)), 
                                                        m_threadPool()
{

}

template<typename T>
Matrix<T>::~Matrix()
{
    m_threadPool.Resume();
}

template<typename T>
Matrix<T>::Matrix(const Matrix& other) : m_rows(other.m_rows), m_cols(other.m_cols), m_data(other.m_data), m_threadPool()
{
    m_threadPool.ChangeNumWorkingThreads(other.m_threadPool.GetNumWorkingThreads());
    m_threadPool.Pause();
}

template<typename T>
template<typename U>
Matrix<T>::Matrix(const Matrix<U>& other) : m_rows(other.m_rows), m_cols(other.m_cols), m_data(other.m_data), m_threadPool()
{
    m_threadPool.ChangeNumWorkingThreads(other.m_threadPool.GetNumWorkingThreads());
    m_threadPool.Pause();
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other)
{
    if (this != &other)
    {
        CopyImpl(other);
    }
    return *this;
}

template<typename T>
template<typename U>
Matrix<T>& Matrix<T>::operator=(const Matrix<U>& other)
{
    if (this != &other)
    {
        CopyImpl(other);
    }
    return *this;
}

/********************************************************************************/
/*                            Matrix Private Methods                            */
/********************************************************************************/

template<typename T> 
template<typename U>
class Matrix<T>::CopyRowFunctor {
public:
    // Constructor to capture the necessary state
    CopyRowFunctor(Matrix<T>* matrix, const Matrix<U>& other, std::size_t startRow, std::size_t endRow)
        : m_matrix(matrix), m_other(other), m_startRow(startRow), m_endRow(endRow) {}

    // Overload the operator() to perform the copy operation
    void operator()() const {
        for (std::size_t i = m_startRow; i < m_endRow; ++i) {
            // Resize each row and use transform_view to create the transformed view
            m_matrix->m_data[i].resize(m_matrix->m_cols);
            auto transformedView = std::ranges::transform_view(
                m_other.m_data[i], 
                [](const U& val) { return static_cast<T>(val); });

            // Copy transformed data into the row
            std::ranges::copy(transformedView, m_matrix->m_data[i].begin());
        }
    }

private:
    Matrix<T>* m_matrix;        // Pointer to the target matrix
    const Matrix<U>& m_other;   // Reference to the source matrix
    std::size_t m_startRow;     // Start row index
    std::size_t m_endRow;       // End row index
};


template<typename T>
template<typename U>
void Matrix<T>::CopyImpl(const Matrix<U>& other)
{
    m_threadPool.ChangeNumWorkingThreads(other.m_threadPool.GetNumWorkingThreads());
    m_rows = other.m_rows;
    m_cols = other.m_cols;    
    m_data.resize(m_rows);

    std::size_t numWorkingThreads = m_threadPool.GetNumWorkingThreads();
    std::size_t rowsPerThread = (m_rows + numWorkingThreads - 1) / numWorkingThreads;
    std::vector<std::future<std::any>> futuresVec(numWorkingThreads);
    m_threadPool.Resume();

    for (std::size_t threadIdx = 0; threadIdx < numWorkingThreads; ++threadIdx){
        std::size_t startRow = threadIdx * rowsPerThread;
        std::size_t endRow = std::min(startRow + rowsPerThread, m_rows);
        
        TaskWrapper copyWrapper(CopyRowFunctor<U>(this, other, startRow, endRow));
        futuresVec[threadIdx] = m_threadPool.AddTask(copyWrapper);
    }

    // Wait for all threads to finish
    for (auto& future : futuresVec) {
        future.get();
    }

    m_threadPool.Pause();
}

#endif //__MATRIX_GAL_D_90__