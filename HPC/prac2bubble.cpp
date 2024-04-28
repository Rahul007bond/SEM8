#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime> 

using namespace std;
void generate_random_data(vector<int>& arr, int size) {
    srand(time(nullptr)); // Seed the random number generator

    for (int i = 0; i < size; ++i) {
        arr.push_back(rand() % 1000); // Generate random integers between 0 and 999
    }
}



// Sequential Bubble Sort
void bubble_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Parallel Odd-Even Transposition Sort using OpenMP
void odd_even_sort(vector<int>& arr) {
    int n = arr.size();
    bool sorted = false;

    while (!sorted) {
        sorted = true;

        // Odd phase
        #pragma omp parallel for
        for (int i = 1; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                sorted = false;
            }
        }

        // Even phase
        #pragma omp parallel for
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                sorted = false;
            }
        }
    }
}


// Merge function for Merge Sort
void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// Sequential Merge Sort
void merge_sort_seq(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    merge_sort_seq(arr, l, m);
    merge_sort_seq(arr, m + 1, r);
    merge(arr, l, m, r);
}

// Parallel Merge Sort
void merge_sort_par(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;

    #pragma omp parallel sections num_threads(4)
    {
        #pragma omp section
        merge_sort_par(arr, l, m);
        #pragma omp section
        merge_sort_par(arr, m + 1, r);
    }

    merge(arr, l, m, r);
}
int main() {
      vector<int> arr;
    const int array_size = 100000; // Change the size as needed

    // Generate random data
    generate_random_data(arr, array_size);

    double start, end;

    // Measure performance of sequential bubble sort
    start = omp_get_wtime();
    bubble_sort(arr);
    end = omp_get_wtime();
    cout << "Sequential bubble sort time: " << end - start << " seconds" << endl;

    // Measure performance of parallel odd-even transposition sort
    start = omp_get_wtime();
    odd_even_sort(arr);
    end = omp_get_wtime();
    cout << "Parallel odd-even transposition sort time: " << end - start << " seconds" << endl;

    // Measure performance of sequential merge sort
    start = omp_get_wtime();
    merge_sort_seq(arr, 0, arr.size() - 1);
    end = omp_get_wtime();
    cout << "Sequential merge sort time: " << end - start << " seconds" << endl;

    // Measure performance of parallel merge sort
    start = omp_get_wtime();
    merge_sort_par(arr, 0, arr.size() - 1);
    end = omp_get_wtime();
    cout << "Parallel merge sort time: " << end - start << " seconds" << endl;

    return 0;
}