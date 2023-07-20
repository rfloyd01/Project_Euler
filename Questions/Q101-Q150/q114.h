#pragma once

#include <Header_Files/pch.h>

//Counting Block Combinations I
std::pair<std::string, double> q114()
{
	auto run_time = std::chrono::steady_clock::now();
	
	long long ways[51] = { 1, 1, 1, 2 };

	for (int i = 4; i <= 50; i++)
	{
		ways[i] = 1 + ways[i - 4] + ways[i - 1];
		ways[i - 3] += ways[i - 4];
	}

	long long answer = ways[50];

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 16475640049
	//ran in 0.0000011 seconds
}

//NOTES
/*The code below is for solving the HackerRank version. It's slower for smaller wall sizes, but exponentially better 
for larger wall sizes
std::vector<std::vector<long long>> matrixMultiply114(const std::vector<std::vector<long long>>& A, const std::vector<std::vector<long long>>& B)
{
    long long n = A.size();
    long long m = B[0].size();
    long long p = B.size();

    std::vector<std::vector<long long>> result(n, std::vector<long long>(m, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < p; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

// Function to raise a matrix to a power
std::vector<std::vector<long long>> matrixPower114(const std::vector<std::vector<long long>>& matrix, long long power)
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
            result = matrixMultiply114(result, base);
        }
        base = matrixMultiply114(base, base);
        power /= 2;
    }

    return result;
}

std::pair<std::string, double> q114()
{
    auto run_time = std::chrono::steady_clock::now();


    std::vector<std::vector<long long> > A, B;
    A = { {1, 1, 1, 1, 2, 4} };
    B = { {1, 0, 0, 0, 0, 1},
         {0, 1, 0, 0, 0, 1},
         {0, 1, 0, 0, 0, 1},
         {0, 0, 1, 0, 0, 0},
         {0, 0, 0, 1, 0, 0},
         {0, 0, 0, 0, 1, 1}
    };

    B = matrixPower114(B, 46);
    A = matrixMultiply114(A, B);
    long long answer = A[0].back();

    return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

    //the answer is 16475640049
    //ran in 0.0000011 seconds
}
*/