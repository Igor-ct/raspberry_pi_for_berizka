#include <iostream>
#include <string>
#include <atomic>
#include <csignal>
#include <chrono>

#include "ConfigManager.hpp"
#include "UartInterface.hpp"
#include "UartBackgroundService.hpp"
#include "MqttService.hpp"
#include "AppController.hpp"

std::atomic<bool> g_running(true);

void signalHandler(int) {
    g_running = false;
}

int main(int argc, char** argv) {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    std::string configPath = (argc > 1) ? argv[1] : "config.json";

    try {
        SystemConfig config = ConfigManager::loadConfig(configPath);

        UartInterface uart(config.uart.port, config.uart.baudrate);
        if (!uart.isOpen()) {
            std::cerr << "[WARN] Failed to open UART. Proceeding with MQTT only..." << std::endl;
        }

        UartBackgroundService uartService(uart);
        uartService.start();

        MqttService mqtt(
            config.mqtt.broker,
            config.mqtt.client_id,
            config.mqtt.topic_status,
            config.mqtt.topic_cmd
        );
        mqtt.connect();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        AppController app(mqtt, uart);
        app.run(g_running);

        std::cout << "[SYSTEM] Initiating clean shutdown..." << std::endl;
        uartService.stop();
        mqtt.disconnect();

    } catch (const mqtt::exception& exc) {
        std::cerr << "[MQTT] Critical error: " << exc.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "[SYSTEM] Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}