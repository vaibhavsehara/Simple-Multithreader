#include <functional>
#include <pthread.h>
#include <chrono>
#include <iostream>
#include <mutex>

class SimpleMultithreader {
public:
    // For one-dimensional loops
    void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads);

    // For two-dimensional loops
    void parallel_for(int low1, int high1, int low2, int high2, std::function<void(int, int)> &&lambda, int numThreads);

private:
    struct ThreadData {
        std::function<void(int)> lambda1D;
        std::function<void(int, int)> lambda2D;
        int threadId;
        int low1, high1, low2, high2;
        std::mutex mutex;
    };

    static void *threadFunction(void *arg);
};

void SimpleMultithreader::parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads) {
    auto start_time = std::chrono::high_resolution_clock::now();

    pthread_t threads[numThreads];

    for (int i = 0; i < numThreads; ++i) {
        ThreadData *data = new ThreadData{lambda, nullptr, i, low, high, 0, 0};
        pthread_create(&threads[i], nullptr, threadFunction, (void *)data);
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Total execution time: " << duration.count() << " seconds" << std::endl;
}

void SimpleMultithreader::parallel_for(int low1, int high1, int low2, int high2, std::function<void(int, int)> &&lambda, int numThreads) {
    auto start_time = std::chrono::high_resolution_clock::now();

    pthread_t threads[numThreads];

    for (int i = 0; i < numThreads; ++i) {
        ThreadData *data = new ThreadData{nullptr, lambda, i, low1, high1, low2, high2};
        pthread_create(&threads[i], nullptr, threadFunction, (void *)data);
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Total execution time: " << duration.count() << " seconds" << std::endl;
}

void *SimpleMultithreader::threadFunction(void *arg) {
    ThreadData *data = static_cast<ThreadData *>(arg);

    struct sched_param param;
    param.sched_priority = 0;

    if (pthread_setschedparam(pthread_self(), SCHED_OTHER, &param) != 0) {
        perror("pthread_setschedparam");
        pthread_exit(nullptr);
    }

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    if (data->lambda1D) {
        for (int i = data->low1; i < data->high1; ++i) {
            data->mutex.lock();
            data->lambda1D(i);
            data->mutex.unlock();
        }
    } else if (data->lambda2D) {
        for (int i = data->low1; i < data->high1; ++i) {
            for (int j = data->low2; j < data->high2; ++j) {
                data->mutex.lock();
                data->lambda2D(i, j);
                data->mutex.unlock();
            }
        }
    }

    clock_gettime(CLOCK_REALTIME, &end);

    double time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    std::cout << "Thread " << data->threadId << ": " << time << " seconds" << std::endl;

    if (data->lambda1D) {
        delete data;
    } else if (data->lambda2D) {
        delete data;
    }

    return nullptr;
    }

