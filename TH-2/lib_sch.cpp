#include "lib_sch.h"
#include <iostream>

parallel_scheduler::parallel_scheduler(int threads_num)
    : threads_count(threads_num)
{
    pool = new pthread_t[threads_count];

    for (int i = 0; i < threads_count; ++i) {
        pthread_create(&pool[i], nullptr, thread_entry, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    pthread_mutex_lock(&mutex);
    terminate_flag = true;
    pthread_mutex_unlock(&mutex);

    pthread_cond_broadcast(&has_task);

    for (int i = 0; i < threads_count; ++i) {
        pthread_join(pool[i], nullptr);
    }

    delete[] pool;

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&has_task);
}

void* parallel_scheduler::thread_entry(void* ctx) {
    static_cast<parallel_scheduler*>(ctx)->worker_loop();
    return nullptr;
}

void parallel_scheduler::worker_loop() {
    while (true) {
        pthread_mutex_lock(&mutex);

        while (task_queue.empty() && !terminate_flag) {
            pthread_cond_wait(&has_task, &mutex);
        }

        if (terminate_flag && task_queue.empty()) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        task* j = task_queue.front();
        task_queue.pop();

        pthread_mutex_unlock(&mutex);

        j->fn();
        delete j;
    }
}

void parallel_scheduler::run(void (*func)(int), int value) {
    task* j = new task;
    j->fn = [func, value]() {
        func(value);
    };

    pthread_mutex_lock(&mutex);
    task_queue.push(j);
    pthread_mutex_unlock(&mutex);

    pthread_cond_signal(&has_task);
}

