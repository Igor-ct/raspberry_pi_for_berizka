#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
    std::queue<T> queue_;
    mutable std::mutex mtx_;
    std::condition_variable cv_;

public:
    void push(const T& item);

    bool pop(T& item);

    bool empty() const;
};

#include "ThreadSafeQueue.tpp"