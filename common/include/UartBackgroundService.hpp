#pragma once
#include <thread>
#include <stop_token>
#include <string>

#include "UartInterface.hpp"
#include "ThreadSafeQueue.hpp"

class UartBackgroundService {
private:
    UartInterface& uart_;
    ThreadSafeQueue<std::string> messageQueue_;
    std::jthread receiver_thread_;
    std::jthread processor_thread_;

public:
    explicit UartBackgroundService(UartInterface& uart);
    
    void start();
    void stop();
};