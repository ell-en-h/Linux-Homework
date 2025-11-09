#include <iostream>
#include <vector>
#include <pthread.h>
#include <cstdlib>
#include <chrono>

struct ThreadData {
    std::vector<int>* arr;
    size_t start;
    size_t end;
    long long sum;
};

void* threadSum(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    long long localSum = 0;
    for (size_t i = data->start; i < data->end; ++i) {
        localSum += (*(data->arr))[i];
    }
    data->sum = localSum;
    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./array-summary <N> <M>\n";
        return 1;
    }

    size_t N = std::stoull(argv[1]);
    size_t M = std::stoull(argv[2]);

    std::vector<int> arr(N);
    for (size_t i = 0; i < N; ++i) {
        arr[i] = rand() % 100;
    }

    auto start1 = std::chrono::high_resolution_clock::now();
    long long sum1 = 0;
    for (size_t i = 0; i < N; ++i) sum1 += arr[i];
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration1 = end1 - start1;

    std::vector<pthread_t> threads(M);
    std::vector<ThreadData> threadData(M);
    size_t chunkSize = N / M;
    
    auto start2 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < M; ++i) {
        threadData[i].arr = &arr;
        threadData[i].start = i * chunkSize;
        threadData[i].end = (i == M - 1) ? N : (i + 1) * chunkSize;
        threadData[i].sum = 0;
        pthread_create(&threads[i], nullptr, threadSum, &threadData[i]);
    }

    long long sum2 = 0;
    for (size_t i = 0; i < M; ++i) {
        pthread_join(threads[i], nullptr);
        sum2 += threadData[i].sum;
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;

    std::cout << "Time spent without threads: " << duration1.count() << " seconds\n";
    std::cout << "Time spent with " << M << " threads: " << duration2.count() << " seconds\n";
    std::cout << "Sum (single thread): " << sum1 << "\n";
    std::cout << "Sum (multi thread): " << sum2 << "\n";

    return 0;
}

