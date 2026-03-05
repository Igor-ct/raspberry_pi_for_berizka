#include "MessageParser.hpp"
#include <iostream>
#include <memory>
#include <json/json.h>

void MessageParser::parseAndLog(const std::string& raw_data) {
    if (raw_data.empty()) return;

    Json::Value root;
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    std::string errs;

    bool parsingSuccessful = reader->parse(
        raw_data.c_str(), 
        raw_data.c_str() + raw_data.size(), 
        &root, 
        &errs
    );

    if (raw_data == "offline") {
        std::cout << "\n[ESP32 Status] Device is offline (LWT).\n> " << std::flush;
        return;
    }

    if (parsingSuccessful) {
        if (root.isMember("device_id") && root["device_id"].isString() &&
            root.isMember("state")     && root["state"].isString() &&
            root.isMember("uptime")    && root["uptime"].isInt()) {
            
            std::cout << "\n[ESP32 Status] Device: " << root["device_id"].asString()
                      << " | State: " << root["state"].asString()
                      << " | Uptime: " << root["uptime"].asInt() << "s\n> " << std::flush;
        } else {
            std::cout << "[ESP32 JSON Received]:\n" << root.toStyledString() << "\n> " << std::flush;
        }
    } else {
        std::cerr << "[Parsing error]: Received invalid JSON -> " << raw_data << std::endl;
    }
}

std::string MessageParser::buildColorCommand(int r, int g, int b) {
    Json::Value root;
    root["command"] = "SET_COLOR";
    
    Json::Value rgbArray(Json::arrayValue);
    rgbArray.append(r);
    rgbArray.append(g);
    rgbArray.append(b);
    root["rgb"] = rgbArray;

    Json::StreamWriterBuilder wbuilder;
    wbuilder["indentation"] = ""; 
    return Json::writeString(wbuilder, root);
}