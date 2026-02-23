
template<typename T>
void ThreadSafeQueue<T>::push(const T& item)
{
    std::lock_guard<std::mutex> lock(mtx_);
    queue_.push(item);
    cv_.notify_one();
}

template<typename T>
bool ThreadSafeQueue<T>::pop(T& item)
{
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [this] { return !queue_.empty(); });
    
    item = std::move(queue_.front());
    queue_.pop();
    return true;
}

template<typename T>
bool ThreadSafeQueue<T>::empty() const 
{
    std::lock_guard<std::mutex> lock(mtx_);
    return queue_.empty();
}