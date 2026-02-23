#include <iostream>
#include <string>
#include <thread> 
#include "ConfigManager.hpp"
#include "UartInterface.hpp"
#include "MessageParser.hpp"
#include "logger.hpp"
#include "ThreadSafeQueue.hpp" 

int main(int argc, char** argv) {

    std::string configPath = "config.json";

    if (argc > 1) {
        configPath = argv[1];
    }

    try {
        std::cout << "Loading configuration from: " << configPath << std::endl;
        UartConfig config = ConfigManager::loadConfig(configPath);
        std::cout << "Settings: Port=" << config.port << ", Baudrate=" << config.baudrate << std::endl;

        UartInterface uart(config.port, config.baudrate);
        
        if (!uart.isOpen()) {
            std::cerr << "Failed to open UART port. Exiting..." << std::endl;
            return 1;
        }
        
        std::cout << "Successfully connected. Starting background threads..." << std::endl;

        ThreadSafeQueue<std::string> messageQueue;

        std::thread receiver_thread([&uart, &messageQueue]() {
            while (true) {
                try {
                    std::string rawLine = uart.readRawLine();
                    
                    if (!rawLine.empty()) {
                        messageQueue.push(rawLine);
                    }
                } 
                catch (const serial::SerialException& e) {
                    std::cerr << "[Receiver] Warning: connection with port is lost!" << std::endl;
                    std::cerr << "[Receiver] Details: " << e.what() << std::endl;
                    
                    break;
                } 
                catch (const std::exception& e) {
                    std::cerr << "[Receiver] Unknown error: " << e.what() << std::endl;
                    break;
                }
            }
        });

        std::thread processor_thread([&messageQueue]() {
            while (true) {
                std::string msg;
                
                messageQueue.pop(msg); 

                MessageParser::parseAndLog(msg);
            }
        });

        receiver_thread.join();
        processor_thread.join();

    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}