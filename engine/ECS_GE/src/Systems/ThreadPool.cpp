#include "ThreadPool.h"

void Thread_pool::worker_thread()
{
    while (!done) 
        run_pending_task();
}

Thread_pool::Thread_pool(): done(false), joiner(threads)
{
    unsigned const threads_count = std::thread::hardware_concurrency();
    try {
        for (unsigned i = 0; i < threads_count; ++i) 
        {
            threads.emplace_back(std::thread(&Thread_pool::worker_thread, this));
        }
    }
    catch (...) {
        done = true;
        throw;
    }
}

Thread_pool::~Thread_pool()
{
    done = true;
}

void Thread_pool::run_pending_task()
{
    function_wrapper task;
    if (pool_work_queue.try_pop(task)) 
        task();
    else 
        std::this_thread::yield();
}

GlobalThreadPool& GlobalThreadPool::Instanse()
{
    static GlobalThreadPool thread_pool;
    return thread_pool;
}
