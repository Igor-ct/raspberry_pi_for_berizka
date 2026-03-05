#pragma once
#include <string>

class MessageParser {
public:
    static void parseAndLog(const std::string& raw_data);
    static std::string buildColorCommand(int r, int g, int b);
};