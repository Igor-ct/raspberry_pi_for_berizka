#include <iostream>
#include <string>
#include <thread> 
#include <stop_token> 
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

        std::jthread receiver_thread([&uart, &messageQueue](std::stop_token stoken) {
            while (!stoken.stop_requested()) {
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

        std::jthread processor_thread([&messageQueue](std::stop_token stoken) {
            
            std::string msg;
            
            while (messageQueue.pop(msg, stoken)) {
                
                MessageParser::parseAndLog(msg);
                
            }
            
            std::cout << "[Processor] Shutting down cleanly." << std::endl;
        });

        receiver_thread.join();
        processor_thread.request_stop();

    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}