#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
    std::queue<T> queue_;
    mutable std::mutex mtx_;
    std::condition_variable_any cv_;
    bool closed_ = false;

public:
    void push(const T& item);

    bool pop(T& item, std::stop_token stoken);;

    bool empty() const;
    void close();
};

#include "ThreadSafeQueue.tpp"