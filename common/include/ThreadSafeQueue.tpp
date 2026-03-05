
template<typename T>
void ThreadSafeQueue<T>::push(const T& item)
{
    std::lock_guard<std::mutex> lock(mtx_);
    queue_.push(item);
    cv_.notify_one();
}

template<typename T>
bool ThreadSafeQueue<T>::pop(T& item, std::stop_token stoken)
{
    std::unique_lock<std::mutex> lock(mtx_);
    
    bool wait_successful = cv_.wait(lock, stoken, [this] { return !queue_.empty(); });
    
    if (!wait_successful) {
        return false; 
    }
    
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

template<typename T>
void ThreadSafeQueue<T>::close() {
        std::lock_guard<std::mutex> lock(mtx_);
        closed_ = true;
        cv_.notify_all();
    }