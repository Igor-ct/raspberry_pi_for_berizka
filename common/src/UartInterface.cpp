#include "UartInterface.hpp"
#include <iostream>

UartInterface::UartInterface(const std::string& port, uint32_t baudrate) {
    try {
        serial_port.setPort(port);
        serial_port.setBaudrate(baudrate);
        serial::Timeout timeout = serial::Timeout::simpleTimeout(100);
        serial_port.setTimeout(timeout);
        serial_port.open();
    } catch (serial::IOException& e) {
        std::cerr << "Error opening port: " << port << std::endl;
        throw;
    }
}

UartInterface::~UartInterface() {
    if (serial_port.isOpen()) {
        serial_port.close();
    }
}

bool UartInterface::isOpen() const {
    return serial_port.isOpen();
}

std::string UartInterface::readRawLine() {
    if (serial_port.isOpen() && serial_port.available()) {
        return serial_port.readline();
    }
    return "";
}

void UartInterface::write(const std::string& data) {
    if (!serial_port.isOpen()) {
        std::cerr << "[UART] Write failed: port not open.\n";
        return;
    }

    try {
        serial_port.write(data);
    } catch (const serial::IOException& e) {
        std::cerr << "[UART] Write failed: " << e.what() << std::endl;
    }
}