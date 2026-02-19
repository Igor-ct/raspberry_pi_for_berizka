#include <iostream>
#include <string>
#include "ConfigManager.hpp"
#include "UartInterface.hpp"
#include "MessageParser.hpp"
#include "logger.hpp"

int main(int argc, char** argv) {

    dummy a;
    a.helloworld();

    std::string configPath = "config.json";

    if (argc > 1) {
        configPath = argv[1];
    }

    try {
        std::cout << "Loading configuration from: " << configPath << std::endl;
        UartConfig config = ConfigManager::loadConfig(configPath);
        std::cout << "Settings: Port=" << config.port << ", Baudrate=" << config.baudrate << std::endl;

        UartInterface uart(config.port, config.baudrate);
        
        if (uart.isOpen()) {
            std::cout << "Successfully connected. Waiting for data from ESP32..." << std::endl;
        }

        while (true) {
            std::string rawLine = uart.readRawLine();
            
            if (!rawLine.empty()) {
                MessageParser::parseAndLog(rawLine);
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}