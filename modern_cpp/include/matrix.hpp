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

    //Move ctor
    Matrix(Matrix&& other);
    Matrix& operator=(Matrix&& other);

    inline std::size_t GetNumRows() const { return m_rows; }
    inline std::size_t GetNumCols() const { return m_cols; }

    inline void SetValue(std::size_t row, std::size_t col, T value) { m_data[row][col] = value; }

    template<typename U>
    requires std::is_convertible_v<U, T>
    Matrix& operator+=(const Matrix<U>& lhs);
    
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
    template<typename Functor>
    void parallelizeByRow(Functor&& functor);

    template<typename U>
    class CopyImplFunctor;

    template<typename U>
    void CopyImpl(const Matrix<U>& other);

    template<typename U>
    class AdditionImplFunctor;
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

template<typename T>
Matrix<T>::Matrix(Matrix&& other) : m_rows(other.m_rows), m_cols(other.m_cols), m_data(std::move(other.m_data)), m_threadPool()
{
    m_threadPool.ChangeNumWorkingThreads(other.m_threadPool.GetNumWorkingThreads());
    other.m_threadPool.ChangeNumWorkingThreads(0);
    m_threadPool.Pause();
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix&& other)
{
    if (this != &other)
    {
        m_rows = other.m_rows;
        m_cols = other.m_cols;
        m_data = std::move(other.m_data);
        m_threadPool.ChangeNumWorkingThreads(other.m_threadPool.GetNumWorkingThreads());
        other.m_threadPool.ChangeNumWorkingThreads(0);
        m_threadPool.Pause();
    }
    return *this;
}

/********************************************************************************/
/*                            Matrix Arithmatic Methods                         */
/********************************************************************************/
template<typename T>
template<typename U>
class Matrix<T>::AdditionImplFunctor
{
public:
    AdditionImplFunctor(Matrix<T>& matrix, const Matrix<U>& other, std::size_t startRow, std::size_t endRow)
        : m_matrix(matrix), m_other(other), m_startRow(startRow), m_endRow(endRow) 
    {}

    void operator()() const 
    {
        if (m_startRow < m_endRow)
        {
            // Use ranges to iterate over the selected rows
            auto range1 = m_matrix.m_data | std::ranges::views::drop(m_startRow) | std::ranges::views::take(m_endRow - m_startRow);
            auto range2 = m_other.m_data  | std::ranges::views::drop(m_startRow) | std::ranges::views::take(m_endRow - m_startRow);

            auto it1 = range1.begin();
            auto it2 = range2.begin();

            // Iterate through rows and apply std::transform for element-wise addition
            for (; it1 != range1.end(); ++it1, ++it2) 
            {
                auto& row1 = *it1;
                auto& row2 = *it2;
                
                // Use std::transform to add elements of row2 to row1
                std::transform(row2.begin(), row2.end(), row1.begin(), row1.begin(),
                    [&](const U& val2, T& val1) { return val1 + static_cast<T>(val2); });
            }
        }
    }

private:
    Matrix<T>& m_matrix;        // Pointer to the target matrix
    const Matrix<U>& m_other;   // Reference to the source matrix
    std::size_t m_startRow;     // Start row index
    std::size_t m_endRow;       // End row index
};

template<typename T>
template<typename U>
requires std::is_convertible_v<U, T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<U>& lhs)
{
    if (m_rows != lhs.GetNumRows() || m_cols != lhs.GetNumCols())
        return *this;

    std::size_t numWorkingThreads = m_threadPool.GetNumWorkingThreads();
    std::size_t rowsPerThread = (m_rows + numWorkingThreads - 1) / numWorkingThreads;

    std::vector<std::future<std::any>> futureVec(numWorkingThreads);
    m_threadPool.Resume();
    for (size_t threadIdx = 0; threadIdx < numWorkingThreads; threadIdx++)
    {
        std::size_t startRow = threadIdx * rowsPerThread;
        std::size_t endRow = std::min(startRow + rowsPerThread, m_rows);

        TaskWrapper AdditionTask(AdditionImplFunctor(*this, lhs, startRow, endRow));
        futureVec[threadIdx] = m_threadPool.AddTask(AdditionTask);
    }
    
    // Wait for all threads to finish
    for (auto& future : futureVec) {
        future.get();
    }

    m_threadPool.Pause();
    return *this;
}

/********************************************************************************/
/*                            Matrix Private Methods                            */
/********************************************************************************/

template<typename T> 
template<typename U>
class Matrix<T>::CopyImplFunctor
{
public:
    CopyImplFunctor(Matrix<T>* matrix, const Matrix<U>& other, std::size_t startRow, std::size_t endRow)
        : m_matrix(matrix), m_other(other), m_startRow(startRow), m_endRow(endRow)
    {}

    void operator()() const 
    {
        for (std::size_t i = m_startRow; i < m_endRow; ++i) 
        {
            m_matrix->m_data[i].resize(m_matrix->m_cols);
            
            auto transformedView = m_other.m_data[i] | std::views::transform(
                [](const U& val) { return static_cast<T>(val); });

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
        
        TaskWrapper copyWrapper(CopyImplFunctor<U>(this, other, startRow, endRow));
        futuresVec[threadIdx] = m_threadPool.AddTask(copyWrapper);
    }

    // Wait for all threads to finish
    for (auto& future : futuresVec) {
        future.get();
    }

    m_threadPool.Pause();
}

#endif //__MATRIX_GAL_D_90__