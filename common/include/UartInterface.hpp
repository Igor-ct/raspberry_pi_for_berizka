#pragma once
#include <string>
#include <serial/serial.h>


class UartInterface {
private:
    serial::Serial serial_port;

public:
    UartInterface(const std::string& port, uint32_t baudrate);
    ~UartInterface();

    bool isOpen() const;
    std::string readRawLine();
};