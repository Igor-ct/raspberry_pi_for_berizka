#pragma once
#include <string>
#include <stdint.h>

struct UartConfig {
    std::string port;
    uint32_t baudrate;
};

struct MqttConfig {
    std::string broker;
    std::string client_id;
    std::string topic_status;
    std::string topic_cmd;
};

struct SystemConfig {
    UartConfig uart;
    MqttConfig mqtt;
};

class ConfigManager {
public:
    static SystemConfig loadConfig(const std::string& filepath);
};