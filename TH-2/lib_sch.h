#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <pthread.h>
#include <queue>
#include <functional>

struct task {
    std::function<void()> fn;
};

class parallel_scheduler {
public:
    explicit parallel_scheduler(int threads_num);
    ~parallel_scheduler();

    void run(void (*func)(int), int value);

private:
    static void* thread_entry(void* ctx);
    void worker_loop();

    int threads_count;
    bool terminate_flag = false;

    std::queue<task*> task_queue;
    pthread_t* pool;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t has_task = PTHREAD_COND_INITIALIZER;
};

#endif

