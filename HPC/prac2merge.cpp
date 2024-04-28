#include <iostream>
#include <omp.h>
#include <ctime>

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    int L[n1], R[n2];

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements (if any)
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        merge_sort(arr, left, mid); // Sort left half
        merge_sort(arr, mid + 1, right); // Sort right half

        merge(arr, left, mid, right); // Merge the sorted halves
    }
}

void merge_sort_parallel(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp task
        merge_sort_parallel(arr, left, mid);

        #pragma omp task
        merge_sort_parallel(arr, mid + 1, right);

        #pragma omp taskwait  // Wait for both tasks to complete before merging
        merge(arr, left, mid, right);
    }
}

int main() {
    int n = 100000; // Adjust array size as needed

    int arr[n];

    // Seed the random number generator
    srand(time(0));

    // Generate random data
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100;
    }
    

    double start_time, end_time;

    // Sequential merge sort timing
    start_time = omp_get_wtime();
    merge_sort(arr, 0, n - 1);
    end_time = omp_get_wtime();
    std::cout << "Sequential merge sort time: " << end_time - start_time << std::endl;

    // Reset the array with new random data
    for (int i = 0; i < n; i++) {
        arr[i] = rand() %100;
    }
 

    // Parallel merge sort timing
    start_time = omp_get_wtime();
    merge_sort_parallel(arr, 0, n - 1);
    end_time = omp_get_wtime();
    std::cout << "Parallel merge sort time: " << end_time - start_time << std::endl;

    return 0;
}
