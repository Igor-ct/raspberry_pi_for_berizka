# Raspberry Pi CMake Study Project 

A tiny C++ project made for studying:
- basic project structure
- a simple “dummy” class
- building with **CMake**
- extending the app to communicate with devices (like ESP32) via UART
- integrating external libraries like jsoncpp and libserial
- parsing JSON configuration files to manage hardware settings
- **[NEW]** Multi-threaded Producer-Consumer Architecture: Decoupling high-frequency UART data reception from JSON processing logic using dedicated threads.
- **[NEW]** Thread Synchronization & Safety: Implementing a custom ThreadSafeQueue with std::mutex and std::condition_variable to manage data flow and prevent race conditions.
- **[NEW]** Template Logic Separation: Organizing complex generic structures by splitting template declarations into .hpp and implementations into .tpp files.

## Practical Assignment: UART Communication Extension

Extend the application to communicate with other devices (e.g., ESP32) via a single-threaded blocking UART interface.
- Hardware Setup: Connect Rx/Tx pins on a UART-USB converter to your laptop. Verify the device mapping (e.g., /dev/ttyUSB0) and test echo functionality using standard tools like minicom or putty.
- Dependencies: Install jsoncpp and libserial-dev into the system and update CMakeLists.txt to link them.
- JSON Configuration: Use ConfigManager to parse config.json at startup to retrieve the serial port filename and baud rate.
- UART Logic: Implement UartInterface to read raw bytes using the serial library. Use MessageParser to handle incoming JSON data from the ESP32 and log it to the terminal.
-  By separating the byte-stream handling in UartInterface from the data logic in MessageParser, you ensure that the application remains modular and avoids the "Swiss-army knife" class anti-pattern.

## **[NEW]** Multi-threaded Producer-Consumer Architecture

To handle high-frequency data from the ESP32 without blocking the main logic, the application has been upgraded to a multi-threaded architecture using the Producer-Consumer pattern.
- Concurrency Model: The app utilizes two dedicated background threads:
    - The Producer: Continuously polls the UartInterface for incoming data and pushes raw strings into a thread-safe queue.
    - The Consumer: Waits for data to appear in the queue, then invokes the MessageParser to process and log the JSON.
- Thread Safety: A custom ThreadSafeQueue was implemented, using std::mutex and std::condition_variable to prevent race conditions and ensure efficient thread synchronization (avoiding high CPU usage from busy-waiting).
- The "Poison Pill" Pattern: For graceful shutdowns, the system can inject a special signal into the queue to tell the Consumer thread to finish its work and exit cleanly.

## Project Structure

```
├── common/
│ ├── include/
│ │ ├──logger.hpp
│ │ ├──ConfigManager.hpp
│ │ ├──ThreadSafeQueue.hpp 
│ │ ├──ThreadSafeQueue.tpp       
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
