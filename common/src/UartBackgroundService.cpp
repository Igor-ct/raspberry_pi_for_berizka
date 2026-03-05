#include "UartBackgroundService.hpp"
#include "MessageParser.hpp"
#include <iostream>
#include <exception>

UartBackgroundService::UartBackgroundService(UartInterface& uart)
    : uart_(uart) {}

void UartBackgroundService::start() {
    if (!uart_.isOpen()) {
        std::cerr << "[UART] Port not open, background service will not start.\n";
        return;
    }

    receiver_thread_ = std::jthread([this](std::stop_token stoken) {
        try {
            while (!stoken.stop_requested()) {
                std::string rawLine = uart_.readRawLine();
                if (!rawLine.empty()) {
                    messageQueue_.push(rawLine);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "[UART Receiver] Exception: " << e.what() << std::endl;
        }
        messageQueue_.close();
    });

    processor_thread_ = std::jthread([this](std::stop_token stoken) {
        std::string msg;
        while (messageQueue_.pop(msg, stoken)) {
            try {
                MessageParser::parseAndLog(msg);
            } catch (const std::exception& e) {
                std::cerr << "[UART Processor] Parsing error: " << e.what() << std::endl;
            }
        }
        std::cout << "[UART Processor] Shutting down cleanly.\n";
    });

    std::cout << "[UartBackgroundService] Background threads started successfully.\n";
}

void UartBackgroundService::stop() {
    receiver_thread_.request_stop();
    processor_thread_.request_stop();

    messageQueue_.close();

}