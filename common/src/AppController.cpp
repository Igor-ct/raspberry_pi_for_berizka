#include "AppController.hpp"
#include "MessageParser.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <exception>
#include <limits>

AppController::AppController(MqttService& mqtt, UartInterface& uart)
    : mqtt_(mqtt), uart_(uart) {}

void AppController::run(std::atomic<bool>& g_running) {
    while (g_running) {
        std::cout << "\n--- MAIN Controller ---\n"
                  << "1. Send via MQTT\n"
                  << "2. Send via UART\n"
                  << "3. Exit\n> " << std::flush;

        int channelChoice = 0;
        if (!(std::cin >> channelChoice)) {
            std::cerr << "[ERROR] Invalid choice. Enter 1, 2, or 3.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (channelChoice == 3) break;
        if (channelChoice != 1 && channelChoice != 2) {
            std::cerr << "[ERROR] Invalid choice. Enter 1 or 2.\n";
            continue;
        }

        int r = -1, g = -1, b = -1;
        std::cout << "Enter R G B values (0-255) separated by spaces: " << std::flush;
        if (!(std::cin >> r >> g >> b)) {
            std::cerr << "[ERROR] Invalid RGB input. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        r = std::clamp(r, 0, 255);
        g = std::clamp(g, 0, 255);
        b = std::clamp(b, 0, 255);

        std::string payload = MessageParser::buildColorCommand(r, g, b);

        if (channelChoice == 1) {
            std::cout << "[Tx] Publishing MQTT: " << payload << std::endl;
            try {
                mqtt_.publishCommand(payload);
            } catch (const std::exception& e) {
                std::cerr << "[MQTT] Publish failed: " << e.what() << std::endl;
            }
        } else {
            if (uart_.isOpen()) {
                try {
                    uart_.write(payload);
                    std::cout << "[Tx] UART sent: " << payload << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "[UART] Write failed: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "[UART] Port is not open!\n";
            }
        }
    }

    std::cout << "[AppController] Exiting run loop.\n";
}