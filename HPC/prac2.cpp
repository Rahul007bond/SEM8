#include <iostream>
#include <ctime>
#include <cstdlib>
#include <omp.h>

using namespace std;

void p_mergesort(int *a, int i, int j);
void s_mergesort(int *a, int i, int j);
void merge(int *a, int i1, int j1, int i2, int j2);

// Sequential Bubble Sort
void bubble_sort_seq(int* arr, int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort
void bubble_sort_par(int* a, int n) {
      for (int i = 0; i < n; i++) {
        int first = i % 2;
#pragma omp parallel for shared(a, first) num_threads(4)
        for (int j = first; j < n - 1; j += 2) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
            }
        }
    }
}
// Merge function for Merge Sort
void merge(int* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = new int[n1];
    int* R = new int[n2];

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

    delete[] L;
    delete[] R;
}

// Sequential Merge Sort
void merge_sort_seq(int* arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    merge_sort_seq(arr, l, m);
    merge_sort_seq(arr, m + 1, r);
    merge(arr, l, m, r);
}

// Parallel Merge Sort
void merge_sort_par(int* arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;

    #pragma omp parallel sections
    {
        #pragma omp section
        merge_sort_par(arr, l, m);
        #pragma omp section
        merge_sort_par(arr, m + 1, r);
    }

    merge(arr, l, m, r);
}

int main() {
    // Generate random data
    const int SIZE = 10000;
    int* data = new int[SIZE];
    srand(time(NULL));
    for (int i = 0; i < SIZE; ++i) {
        data[i] = rand() % SIZE;
    }

    // Sequential Bubble Sort
    int* seq_bubble_data = new int[SIZE];
    for (int i = 0; i < SIZE; ++i)
        seq_bubble_data[i] = data[i];
    clock_t start_time = clock();
    bubble_sort_seq(seq_bubble_data, SIZE);
    clock_t end_time = clock();
    double seq_bubble_time = double(end_time - start_time) / CLOCKS_PER_SEC;

    // Parallel Bubble Sort
    int* par_bubble_data = new int[SIZE];
    for (int i = 0; i < SIZE; ++i)
        par_bubble_data[i] = data[i];
    start_time = clock();
    bubble_sort_par(par_bubble_data, SIZE);
    end_time = clock();
    double par_bubble_time = double(end_time - start_time) / CLOCKS_PER_SEC;

    // Sequential Merge Sort
    int* seq_merge_data = new int[SIZE];
    for (int i = 0; i < SIZE; ++i)
        seq_merge_data[i] = data[i];
    start_time = clock();
    merge_sort_seq(seq_merge_data, 0, SIZE - 1);
    end_time = clock();
    double seq_merge_time = double(end_time - start_time) / CLOCKS_PER_SEC;

    // Parallel Merge Sort
    int* par_merge_data = new int[SIZE];
    for (int i = 0; i < SIZE; ++i)
        par_merge_data[i] = data[i];
    start_time = clock();
    merge_sort_seq(par_merge_data, 0, SIZE - 1);
    end_time = clock();
    double par_merge_time = double(end_time - start_time) / CLOCKS_PER_SEC;

    // Output timing results
    cout << "Sequential Bubble Sort Time: " << seq_bubble_time << " seconds" << endl;
    cout << "Parallel Bubble Sort Time: " << par_bubble_time << " seconds" << endl;
    cout << "Sequential Merge Sort Time: " << seq_merge_time << " seconds" << endl;
    cout << "Parallel Merge Sort Time: " << par_merge_time << " seconds" << endl;


    return 0;
}
