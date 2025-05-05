#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
using namespace std;

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
}

// Fixed parallel bubble sort using odd-even transposition
void parallelBubbleSort(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        int start = i % 2;
        #pragma omp parallel for
        for (int j = start; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = new int[n1];
    int *R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void parallelMergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, left, mid);

            #pragma omp section
            parallelMergeSort(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << endl;
}

void measurePerformance(int arr[], int n) {
    int *arr1 = new int[n];
    int *arr2 = new int[n];
    int *arr3 = new int[n];
    int *arr4 = new int[n];
    copy(arr, arr + n, arr1);
    copy(arr, arr + n, arr2);
    copy(arr, arr + n, arr3);
    copy(arr, arr + n, arr4);

    cout << "\nOriginal Array: ";
    printArray(arr, n);

    // Sequential Bubble Sort
    clock_t bubbleStart = clock();
    bubbleSort(arr1, n);
    clock_t bubbleEnd = clock();
    double bubbleDuration = double(bubbleEnd - bubbleStart) / CLOCKS_PER_SEC;
    cout << "Sequential Bubble Sort Time: " << bubbleDuration << " seconds\n";
    printArray(arr1, n);

    // Parallel Bubble Sort (Fixed)
    clock_t parBubbleStart = clock();
    parallelBubbleSort(arr2, n);
    clock_t parBubbleEnd = clock();
    double parBubbleDuration = double(parBubbleEnd - parBubbleStart) / CLOCKS_PER_SEC;
    cout << "Parallel Bubble Sort Time: " << parBubbleDuration << " seconds\n";
    printArray(arr2, n);

    // Sequential Merge Sort
    clock_t mergeStart = clock();
    mergeSort(arr3, 0, n - 1);
    clock_t mergeEnd = clock();
    double mergeDuration = double(mergeEnd - mergeStart) / CLOCKS_PER_SEC;
    cout << "Sequential Merge Sort Time: " << mergeDuration << " seconds\n";
    printArray(arr3, n);

    // Parallel Merge Sort
    clock_t parMergeStart = clock();
    parallelMergeSort(arr4, 0, n - 1);
    clock_t parMergeEnd = clock();
    double parMergeDuration = double(parMergeEnd - parMergeStart) / CLOCKS_PER_SEC;
    cout << "Parallel Merge Sort Time: " << parMergeDuration << " seconds\n";
    printArray(arr4, n);

    delete[] arr1;
    delete[] arr2;
    delete[] arr3;
    delete[] arr4;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;
    int *arr = new int[n];
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) cin >> arr[i];

    measurePerformance(arr, n);

    delete[] arr;
    return 0;
}

