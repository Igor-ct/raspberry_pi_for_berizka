#pragma once
#include <string>
#include <stdexcept>
#include <cstdint>

struct UartConfig {
    std::string port;
    uint32_t baudrate;
};

class ConfigManager {
public:
    static UartConfig loadConfig(const std::string& filepath);
};