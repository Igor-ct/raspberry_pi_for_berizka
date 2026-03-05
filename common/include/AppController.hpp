#pragma once
#include "MqttService.hpp"
#include "UartInterface.hpp"
#include <atomic>

class AppController {
private:
    MqttService& mqtt_;
    UartInterface& uart_;

public:
    AppController(MqttService& mqtt, UartInterface& uart);
    
    void run(std::atomic<bool>& g_running);
};