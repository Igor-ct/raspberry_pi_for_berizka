#include "ConfigManager.hpp"
#include <fstream>
#include <json/json.h> 

UartConfig ConfigManager::loadConfig(const std::string& filepath) {
    std::ifstream configFile(filepath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Failed to open configuration file: " + filepath);
    }

    Json::Value root;
    Json::CharReaderBuilder builder;
    std::string errs;

    if (!Json::parseFromStream(builder, configFile, &root, &errs)) {
        throw std::runtime_error("Error parsing config.json: " + errs);
    }

    UartConfig config;

    config.port = root.get("filename", "/dev/ttyUSB0").asString();
    config.baudrate = root.get("baud_rate", 115200).asUInt();


    return config;
}