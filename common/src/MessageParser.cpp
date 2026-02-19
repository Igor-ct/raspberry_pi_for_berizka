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

    if (parsingSuccessful) {
        std::cout << "[ESP32 JSON Received]:\n" 
                  << root.toStyledString() << std::endl;
    } else {
        std::cerr << "[Parsing error]: Received invalid JSON -> " << raw_data << std::endl;
    }
}