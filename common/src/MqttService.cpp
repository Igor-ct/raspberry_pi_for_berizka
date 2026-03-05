#include "MqttService.hpp"
#include "MessageParser.hpp"
#include <iostream>

MqttService::MqttService(const std::string& address, const std::string& clientId, 
                         const std::string& topicStatus, const std::string& topicCmd)
    : client_(address, clientId), topic_status_(topicStatus), topic_cmd_(topicCmd) {

    client_.set_callback(*this);

    client_.set_connected_handler([this](const std::string&) {
        std::cout << "[MQTT] Connected callback triggered. Subscribing...\n";
        try {
            client_.subscribe(topic_status_, 1); 
        } catch (const mqtt::exception& e) {
            std::cerr << "[MQTT] Subscribe failed: " << e.what() << std::endl;
        }
    });

    client_.set_connection_lost_handler([this](const std::string& cause) {
        std::cerr << "[MQTT] Connection lost: " << cause << std::endl;
    });
}

void MqttService::connect() {
    mqtt::connect_options connOpts;
    connOpts.set_clean_session(true);
    connOpts.set_automatic_reconnect(true);
    connOpts.set_keep_alive_interval(20);

    mqtt::message willmsg(topic_status_, "offline", 1, true);
    connOpts.set_will(willmsg);

    std::cout << "[MQTT] Starting async connect...\n";

    try {
        client_.connect(connOpts);  
        std::cout << "[MQTT] Connect issued (async)\n";
    } catch (const mqtt::exception& e) {
        std::cerr << "[MQTT] Connect failed immediately: " << e.what() << std::endl;
    }
}

void MqttService::disconnect() {
    try {
        client_.disconnect()->wait();
    } catch (const mqtt::exception& e) {
        std::cerr << "[MQTT] Disconnect failed: " << e.what() << std::endl;
    }
}

void MqttService::publishCommand(const std::string& payload) {
    if (!client_.is_connected()) {
        std::cerr << "[MQTT] Not connected. Publish skipped.\n";
        return;
    }
    try {
        client_.publish(topic_cmd_, payload, 1, false);
    } catch (const mqtt::exception& e) {
        std::cerr << "[MQTT] Publish failed: " << e.what() << std::endl;
    }
}

void MqttService::connection_lost(const std::string& cause) {
    std::cerr << "[MQTT] Connection lost: " << cause << std::endl;
}

void MqttService::message_arrived(mqtt::const_message_ptr msg) {
    std::cout << "[MQTT RX] " << msg->get_topic() 
              << " -> " << msg->get_payload_str() << std::endl;
    MessageParser::parseAndLog(msg->get_payload_str());
}

void MqttService::delivery_complete(mqtt::delivery_token_ptr) {}