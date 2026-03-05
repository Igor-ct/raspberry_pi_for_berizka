#pragma once
#include <string>
#include <mqtt/async_client.h>

class MqttService : public virtual mqtt::callback {
private:
    mqtt::async_client client_;
    std::string topic_status_;
    std::string topic_cmd_;

    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr msg) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

public:
    MqttService(const std::string& address, const std::string& clientId, 
                const std::string& topicStatus, const std::string& topicCmd);
    void connect();
    void disconnect();
    void publishCommand(const std::string& payload);
};