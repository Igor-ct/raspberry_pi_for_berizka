#include "ConfigManager.hpp"
#include <fstream>
#include <json/json.h> 

SystemConfig ConfigManager::loadConfig(const std::string& filepath) {
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

    SystemConfig config;

    if (root.isMember("uart")) {
        config.uart.port     = root["uart"].get("port", "/dev/ttyUSB0").asString(); 
        config.uart.baudrate = root["uart"].get("baud_rate", 115200).asUInt();
    }

    if (root.isMember("mqtt")) {
        const auto& mqttNode = root["mqtt"];

        if (!mqttNode.isMember("broker") || !mqttNode["broker"].isString()) {
           throw std::runtime_error("PLEASE SET MQTT BROKER in config.json");
        }  
        if (!mqttNode.isMember("client_id") || !mqttNode["client_id"].isString()) {
            throw std::runtime_error("PLEASE SET MQTT CLIENT_ID in config.json");
        }
        if (!mqttNode.isMember("topic_status") || !mqttNode["topic_status"].isString()) {
            throw std::runtime_error("PLEASE SET MQTT TOPIC_STATUS in config.json");
        }  
        if (!mqttNode.isMember("topic_cmd") || !mqttNode["topic_cmd"].isString()) {
            throw std::runtime_error("PLEASE SET MQTT TOPIC_CMD in config.json");
        }

        config.mqtt.broker       = mqttNode["broker"].asString();
        config.mqtt.client_id    = mqttNode["client_id"].asString();
        config.mqtt.topic_status = mqttNode["topic_status"].asString();
        config.mqtt.topic_cmd    = mqttNode["topic_cmd"].asString();
    }        
    else {
        throw std::runtime_error("PLEASE SET MQTT SECTION in config.json");
    }


    return config;
}