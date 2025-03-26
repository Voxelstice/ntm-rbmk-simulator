#include "raylib.h"
#include <stdio.h>
#include <time.h>
// TODO: Make this compatible with Linux and MacOS. They dont appear to have such library
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

std::ofstream logStream;

// Custom logging function
std::string CustomLogString(int msgType, const char* text, va_list args) {
    char timeStr[64] = { 0 };
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);

    strftime(timeStr, sizeof(timeStr), "%d-%m-%Y %H:%M:%S", tm_info);
    std::string result = "[" + std::string(timeStr) + "] ";

    switch (msgType) {
        case LOG_INFO: result += "[INFO] : "; break;
        case LOG_ERROR: result += "[ERROR]: "; break;
        case LOG_WARNING: result += "[WARN] : "; break;
        case LOG_DEBUG: result += "[DEBUG]: "; break;
        default: break;
    }

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), text, args);
    result += buffer;
    result += "\n";

    return result;
}

void CustomLog(int msgType, const char *text, va_list args) {
    std::string logString = CustomLogString(msgType, text, args);
    printf("%s", logString.c_str());
    logStream << logString.c_str();
}

void Logger_init() {
    SetTraceLogCallback(CustomLog);

    // Create the logs directory
    std::filesystem::create_directory("./logs");

    // Begin the file stream
    char timeStr[64] = { 0 };
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    strftime(timeStr, sizeof(timeStr), "logs/%d-%m-%Y_%H-%M-%S.log", tm_info);
    logStream = std::ofstream(timeStr);
    printf("You might want to look at the actual log file if the game happens to crash\n");
}

void Logger_writeToLog(const char* text) {
    logStream << text;
}

void Logger_close() {
    Logger_writeToLog("Logger flushed and closed");
    logStream.flush();
    logStream.close();
    printf("You might want to look at the actual log file if the game happens to crash\n");
}