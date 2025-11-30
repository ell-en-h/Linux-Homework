#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <vector>
#include <functional>
#include <thread>

class mutex; // forward
// без condition_variable

class ParallelScheduler {
public:
    explicit ParallelScheduler(int threadsCount);
    ~ParallelScheduler();

    void schedule(const std::function<void()>& task);
    void wait();

private:
    void workerLoop();

private:
    std::vector<std::thread> threads;

    mutex* mtx;

    bool stop;
    bool working; // true если есть невыполненные задачи

    std::vector<std::function<void()>> tasks;
};

#endif

