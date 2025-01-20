#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
#include "matrix.hpp"

std::vector<std::vector<float>> generateRandomMatrix(std::size_t rows, std::size_t cols, float minValue, float maxValue) {
    std::random_device rd; // Seed the random number generator
    std::mt19937 gen(rd()); // Mersenne Twister generator
    std::uniform_real_distribution<float> dis(minValue, maxValue); // Distribution for float values

    std::vector<std::vector<float>> matrix(rows, std::vector<float>(cols));

    for (std::size_t i = 0; i < rows; ++i) 
    {
        for (std::size_t j = 0; j < cols; ++j) 
        {
            matrix[i][j] = dis(gen); // Fill each element with a random float
        }
    }

    return matrix;
}

void PrintMatrix(std::size_t rows, std::size_t cols, const std::vector<std::vector<float>>& matrix)
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

bool CompareVectors(const std::vector<std::vector<float>>& lhs, const std::vector<std::vector<float>>& rhs, const auto rows, const auto cols)
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

int main()
{
    auto vec2D = generateRandomMatrix(25, 25, -100, 100);
    Matrix copyFrom(vec2D);

    Matrix copyTo(10, 10);
    copyTo = copyFrom;

    std::cout << std::boolalpha << "IsSame: " << CompareVectors(copyTo.GetData(), copyFrom.GetData(), copyTo.GetNumRows(), copyTo.GetNumCols()) << "\n";

    return 0;
}