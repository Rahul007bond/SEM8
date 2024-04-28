#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Sequential Minimum
int sequential_min(const int* data, int size) {
    int min_val = data[0];
    for (int i = 1; i < size; ++i) {
        if (data[i] < min_val) {
            min_val = data[i];
        }
    }
    return min_val;
}

// Sequential Maximum
int sequential_max(const int* data, int size) {
    int max_val = data[0];
    for (int i = 1; i < size; ++i) {
        if (data[i] > max_val) {
            max_val = data[i];
        }
    }
    return max_val;
}

// Sequential Sum
int sequential_sum(const int* data, int size) {
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += data[i];
    }
    return sum;
}

// Sequential Average
double sequential_average(const int* data, int size) {
    int sum = sequential_sum(data, size);
    return static_cast<double>(sum) / size;
}

// Parallel Reduction for Minimum
int parallel_min(const int* data, int size) {
    int min_val = data[0];
    #pragma omp parallel for reduction(min: min_val) num_threads(4)
    for (int i = 0; i < size; ++i) {
        if (data[i] < min_val) {
            min_val = data[i];
        }
    }
    return min_val;
}

// Parallel Reduction for Maximum
int parallel_max(const int* data, int size) {
    int max_val = data[0];
    #pragma omp parallel for reduction(max: max_val) num_threads(5)
    for (int i = 0; i < size; ++i) {
        if (data[i] > max_val) {
            max_val = data[i];
        }
    }
    return max_val;
}

// Parallel Reduction for Sum
int parallel_sum(const int* data, int size) {
    int sum = 0;
    #pragma omp parallel for reduction(+: sum) num_threads(5)
    for (int i = 0; i < size; ++i) {
        sum += data[i];
    }
    return sum;
}

// Parallel Reduction for Average
double parallel_average(const int* data, int size) {
    int sum = parallel_sum(data, size);
    return static_cast<double>(sum) / size;
}

int main() {
    const int SIZE = 10000;
    int data[SIZE];

    // Generate random data
    srand(time(NULL));
    for (int i = 0; i < SIZE; ++i) {
        data[i] = rand() % 100;
    }

    // Sequential execution
    double start_time = omp_get_wtime();
    int seq_min_val = sequential_min(data, SIZE);
    int seq_max_val = sequential_max(data, SIZE);
    int seq_sum = sequential_sum(data, SIZE);
    double seq_avg = sequential_average(data, SIZE);
    double seq_time = omp_get_wtime() - start_time;

    // Parallel execution
    start_time = omp_get_wtime();
    int par_min_val = parallel_min(data, SIZE);
    int par_max_val = parallel_max(data, SIZE);
    int par_sum = parallel_sum(data, SIZE);
    double par_avg = parallel_average(data, SIZE);
    double par_time = omp_get_wtime() - start_time;

    // Print results
    cout << "Sequential Minimum value: " << seq_min_val << endl;
    cout << "Sequential Maximum value: " << seq_max_val << endl;
    cout << "Sequential Sum: " << seq_sum << endl;
    cout << "Sequential Average: " << seq_avg << endl;
    cout << "Sequential Execution Time: " << seq_time << " seconds" << endl << endl;

    cout << "Parallel Minimum value: " << par_min_val << endl;
    cout << "Parallel Maximum value: " << par_max_val << endl;
    cout << "Parallel Sum: " << par_sum << endl;
    cout << "Parallel Average: " << par_avg << endl;
    cout << "Parallel Execution Time: " << par_time << " seconds" << endl;

    return 0;
}
