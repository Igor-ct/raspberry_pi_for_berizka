# Raspberry Pi CMake Study Project 

A tiny C++ project made for studying:
- basic project structure
- a simple “dummy” class
- building with **CMake**
- **[NEW]** extending the app to communicate with devices (like ESP32) via UART
- **[NEW]** integrating external libraries like jsoncpp and libserial
- **[NEW]** parsing JSON configuration files to manage hardware settings

The program prints:
```
hello world!
```

## **[NEW]** Practical Assignment: UART Communication Extension

Extend the application to communicate with other devices (e.g., ESP32) via a single-threaded blocking UART interface.
- Hardware Setup: Connect Rx/Tx pins on a UART-USB converter to your laptop. Verify the device mapping (e.g., /dev/ttyUSB0) and test echo functionality using standard tools like minicom or putty.
- Dependencies: Install jsoncpp and libserial-dev into the system and update CMakeLists.txt to link them.
- JSON Configuration: Use ConfigManager to parse config.json at startup to retrieve the serial port filename and baud rate.
- UART Logic: Implement UartInterface to read raw bytes using the serial library. Use MessageParser to handle incoming JSON data from the ESP32 and log it to the terminal.
-  By separating the byte-stream handling in UartInterface from the data logic in MessageParser, you ensure that the application remains modular and avoids the "Swiss-army knife" class anti-pattern.

## Project Structure

```
├── common/
│ ├── include/
│ │ ├──logger.hpp
│ │ ├──ConfigManager.hpp
│ │ ├──MessageParser.hpp
│ │ └──UartInterface.hpp
│ ├── src/
│ │ ├──logger.cpp
│ │ ├──ConfigManager.cpp
│ │ ├──MessageParser.cpp
│ │ └──UartInterface.cpp
│ └── CMakeLists.txt
│
├── main.cpp
├── CMakeLists.txt
├── .gitignore
├── config.json
├── LICENSE
└── README.md
```
