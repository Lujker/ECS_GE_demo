#pragma once
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <future>
#include <atomic>
#include <vector>
#include <mutex>
#include <thread>

#include "FunctionWrapper.h"
//!C++11-14
template<typename T>
class threadsafe_queue
{
	struct node
	{
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
	};
    mutable std::mutex head_mutex; 
    mutable std::mutex tail_mutex;
    std::unique_ptr<node> head;
    node* tail;
    std::condition_variable data_cond;

    node* get_tail()
    {
        std::lock_guard tail_lock(tail_mutex);
        return tail;
    }

    std::unique_ptr<node> pop_head()
    {
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }

    std::unique_lock<std::mutex> wait_for_data()
    {
        std::unique_lock h_lock(head_mutex);
        data_cond.wait(h_lock, [&] {return head.get() != get_tail(); });
        return std::move(h_lock);
    }

    std::unique_ptr<node> wait_pop_head()
    {
        std::unique_lock h_lock(wait_for_data());
        return pop_head();
    }

    std::unique_ptr<node> wait_pop_head(T& val)
    {
        std::unique_lock h_lock(wait_for_data());
        val = std::move(*head->data);
        return pop_head();
    }

    std::unique_ptr<node> try_pop_head()
    {
        std::lock_guard head_lock(head_mutex);
        if (head.get() == get_tail())
        {
            return std::unique_ptr<node>();
        }
        return pop_head();
    }

    std::unique_ptr<node> try_pop_head(T& value)
    {
        std::lock_guard head_lock(head_mutex);
        if (head.get() == get_tail())
        {
            return std::unique_ptr<node>();
        }
        value = std::move(*head->data);
        return pop_head();
    }

public:
    threadsafe_queue() :
		head(std::make_unique<node>()), tail(head.get())
		{}
    threadsafe_queue(const threadsafe_queue&) = delete;
    threadsafe_queue& operator=(const threadsafe_queue&)=delete;

    void push(T new_value);
    bool try_pop(T& value);
    std::shared_ptr<T> try_pop();

    void wait_and_pop(T& value);
    std::shared_ptr<T> wait_and_pop();

    bool empty()const;
};

class join_threads {
    std::vector<std::thread>& thread;
public:
    explicit join_threads(std::vector<std::thread>& threads_) :
        thread(threads_) {}
    ~join_threads() {
        for (unsigned long i = 0; i < thread.size(); ++i)
            if (thread[i].joinable())
                thread[i].join();
    }
};

class Thread_pool
{
    std::atomic_bool done;
    threadsafe_queue<function_wrapper> pool_work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;
    void worker_thread();

public:
    Thread_pool();
    ~Thread_pool();

    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f);

    void run_pending_task();
};

template<typename FunctionType>
inline std::future<typename std::result_of<FunctionType()>::type> Thread_pool::submit(FunctionType f)
{
    typedef  typename std::result_of<FunctionType()>::type result_type;
    std::packaged_task<result_type()> task(std::move(f));
    std::future<result_type> res(task.get_future());
    pool_work_queue.push(std::move(task));
    return res;
}

template<typename T>
inline void threadsafe_queue<T>::push(T new_value)
{
    std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));

    std::unique_ptr<node> p(std::make_unique<node>());
    {
        std::lock_guard tail_lock(tail_mutex);
        tail->data = new_data;
        node* const new_tail = p.get();
        tail->next = std::move(p);
        tail = new_tail;
    }
    data_cond.notify_one();
}

template<typename T>
inline bool threadsafe_queue<T>::try_pop(T& value)
{
    std::unique_ptr<node> const old_head = try_pop_head(value);
    return old_head != std::unique_ptr<node>();
}

template<typename T>
inline std::shared_ptr<T> threadsafe_queue<T>::try_pop()
{
    std::unique_ptr<node> const old_head = try_pop_head();
    return old_head ? old_head->data : std::shared_ptr<T>();
}

template<typename T>
inline void threadsafe_queue<T>::wait_and_pop(T& value)
{
    std::unique_ptr<node> const old_head = wait_pop_head(value);
}

template<typename T>
inline std::shared_ptr<T> threadsafe_queue<T>::wait_and_pop()
{
    std::unique_ptr<node> const old_head = wait_pop_head();
    return old_head->data;
}

template<typename T>
inline bool threadsafe_queue<T>::empty() const
{
    std::lock_guard head_lock(head_mutex);
    return (head.get() == get_tail());
}
#endif // THREAD_POOL_H
