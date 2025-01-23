#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
#include <cassert>
#include "matrix.hpp"

template<typename T>
std::vector<std::vector<T>> generateRandomMatrix(std::size_t rows, std::size_t cols, T minValue, T maxValue);

template<typename T>
void PrintMatrix(std::size_t rows, std::size_t cols, const std::vector<std::vector<T>>& matrix);

template<typename T>
bool CompareVectors(const std::vector<std::vector<T>>& lhs, const std::vector<std::vector<T>>& rhs, const auto rows, const auto cols);

bool TestCtors();
bool TestCopyCtors();
bool TestMoveCtors();
bool TestArithmaticMethods();

int main()
{
    assert(TestCtors());
    assert(TestCopyCtors());
    assert(TestMoveCtors());
    assert(TestArithmaticMethods());

    return 0;
}


template<typename T>
std::vector<std::vector<T>> generateRandomMatrix(std::size_t rows, std::size_t cols, T minValue, T maxValue)
{
    std::random_device rd; // Seed the random number generator
    std::mt19937 gen(rd()); // Mersenne Twister generator
    std::uniform_real_distribution<T> dis(minValue, maxValue); // Distribution for float values

    std::vector<std::vector<T>> matrix(rows, std::vector<T>(cols));

    for (std::size_t i = 0; i < rows; ++i) 
    {
        for (std::size_t j = 0; j < cols; ++j) 
        {
            matrix[i][j] = dis(gen); // Fill each element with a random float
        }
    }

    return matrix;
}

template<typename T>
void PrintMatrix(std::size_t rows, std::size_t cols, const std::vector<std::vector<T>>& matrix)
{
    const int width = 8;  // Adjust the width for neatness
    const int precision = 2;  // Adjust the decimal precision

    for (std::size_t i = 0; i < rows; ++i) 
    {
        for (std::size_t j = 0; j < cols; ++j) 
        {
            // Set the width and precision for neat printing
            std::cout << std::setw(width) << std::fixed << std::setprecision(precision) << matrix[i][j];
        }
        std::cout << "\n";
    }
}

template<typename T>
bool CompareVectors(const std::vector<std::vector<T>>& lhs, const std::vector<std::vector<T>>& rhs, const auto rows, const auto cols)
{
    for (std::size_t i = 0; i < rows; ++i) 
    {
        for (std::size_t j = 0; j < cols; ++j) 
        {
            if (lhs[i][j] != rhs[i][j])
                return false;
        }
    }
    return true;
}

bool TestCtors()
{
    Matrix<int> mat1(6, 2);
    
    Matrix<float> mat2(12, 20);
    
    auto vec2D = generateRandomMatrix<float>(25, 25, -100.0f, 100.0f);
    Matrix mat3(vec2D);

    return true;
}


bool TestCopyCtors()
{
    auto vec2D = generateRandomMatrix<float>(125, 125, -100.0f, 100.0f);
    Matrix copyFrom(vec2D);

    Matrix copyTo(10, 10);
    copyTo = copyFrom;

    assert(CompareVectors(copyTo.GetData(), copyFrom.GetData(), copyTo.GetNumRows(), copyTo.GetNumCols()));

    return true;
}


bool TestMoveCtors()
{
    auto vec2D = generateRandomMatrix<float>(125, 125, -100.0f, 100.0f);
    Matrix mat1(vec2D);
    Matrix mat2(std::move(mat1));
    assert(CompareVectors(mat2.GetData(), vec2D, mat2.GetNumRows(), mat2.GetNumCols()));

    vec2D = generateRandomMatrix<float>(55, 75, -100.0f, 100.0f);
    Matrix mat3(vec2D);
    mat2 = std::move(mat3);
    assert(CompareVectors(mat2.GetData(), vec2D, mat2.GetNumRows(), mat2.GetNumCols()));

    mat3 = std::move(mat2);
    assert(CompareVectors(mat3.GetData(), vec2D, mat3.GetNumRows(), mat3.GetNumCols()));

    return true;
}

bool TestArithmaticMethods()
{
    auto vec2D = generateRandomMatrix<float>(10, 12, -10.0f, 10.0f);
    Matrix Mat1(vec2D);
    std::cout << "Mat1: \n";
    PrintMatrix(Mat1.GetNumRows(), Mat1.GetNumCols(), Mat1.GetData());
    std::cout << "\n\n";

    vec2D = generateRandomMatrix<float>(10, 12, -10.0f, 10.0f);
    Matrix Mat2(vec2D);
    std::cout << "Mat2: \n";
    PrintMatrix(Mat2.GetNumRows(), Mat2.GetNumCols(), Mat2.GetData());
    std::cout << "\n\n";

    Mat1 += Mat2;
    std::cout << "Mat1 after addition: \n";
    PrintMatrix(Mat1.GetNumRows(), Mat1.GetNumCols(), Mat1.GetData());


    return true;
}