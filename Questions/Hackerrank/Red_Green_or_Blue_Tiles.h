#pragma once

#include <Header_Files/pch.h>

std::vector<std::vector<long long>> matrixMultiply116(const std::vector<std::vector<long long>>& A, const std::vector<std::vector<long long>>& B)
{
    long long n = A.size();
    long long m = B[0].size();
    long long p = B.size();

    std::vector<std::vector<long long>> result(n, std::vector<long long>(m, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < p; k++) {
                result[i][j] = (result[i][j] + (A[i][k] * B[k][j]) % 1000000007) % 1000000007;
            }
        }
    }

    return result;
}

// Function to raise a matrix to a power
std::vector<std::vector<long long>> matrixPower116(const std::vector<std::vector<long long>>& matrix, long long power)
{
    int n = matrix.size();
    int m = matrix[0].size();

    // Create an identity matrix
    std::vector<std::vector<long long>> result(n, std::vector<long long>(m, 0));
    for (int i = 0; i < n; i++) {
        result[i][i] = 1;
    }

    // Exponentiation by squaring
    std::vector<std::vector<long long>> base = matrix;
    while (power > 0) {
        if (power % 2 == 1) {
            result = matrixMultiply116(result, base);
        }
        base = matrixMultiply116(base, base);
        power /= 2;
    }

    return result;
}

int RGoB() {
    auto run_time = std::chrono::steady_clock::now();
    long long trials, n;
    //std::cin >> trials;
    trials = 1;

    for (int i = 0; i < trials; i++)
    {
        //std::cin >> n;
        n = 50;

        //First solve edge cases
        if (n <= 4)
        {
            if (n == 1) std::cout << 0 << std::endl;
            else if (n == 2) std::cout << 1 << std::endl;
            else if (n == 3) std::cout << 3 << std::endl;
            else std::cout << 7 << std::endl;
        }
        else
        {
            //Initialize the vectors and matrices needed for calculations
            std::vector<std::vector<long long> > a_red, a_green, a_blue;
            std::vector<std::vector<long long> > b_red, b_green, b_blue;

            a_red = { {1, 2} };
            a_green = { {1, 1, 2} };
            a_blue = { {1, 1, 1, 2} };

            b_red = { {0, 1}, {1, 1} };
            b_green = { {0, 0, 1}, {1, 0, 0}, {0, 1, 1} };
            b_blue = { {0, 0, 0, 1}, {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 1} };

            b_red = matrixPower116(b_red, n - 2);
            b_green = matrixPower116(b_green, n - 3);
            b_blue = matrixPower116(b_blue, n - 4);

            a_red = matrixMultiply116(a_red, b_red);
            a_green = matrixMultiply116(a_green, b_green);
            a_blue = matrixMultiply116(a_blue, b_blue);
            double yozus = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0;
            std::cout << "The answer is " << (a_red[0].back() + a_green[0].back() + a_blue[0].back() - 3) % 1000000007 << std::endl;
            std::cout << "Ran in " << yozus << " seconds." << std::endl;
        }
    }

    return 0;
}

//NOTES