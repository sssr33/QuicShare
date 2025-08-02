#pragma once

#include <QtWidgets/QTextEdit>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/qt_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

class Log {
public:
    static void Init(const std::string logPath, QTextEdit* logWidget);
};

#define LOG_INFO(msg, ...) spdlog::info(msg, __VA_ARGS__);
#define LOG_WARN(msg, ...) spdlog::warn(msg, __VA_ARGS__);
#define LOG_DEBUG(msg, ...) spdlog::debug(msg, __VA_ARGS__);
#define LOG_ERROR(msg, ...) spdlog::error(msg, __VA_ARGS__);
#define LOG_CRITICAL(msg, ...) spdlog::critical(msg, __VA_ARGS__);
#define LOG_TRACE(msg, ...) spdlog::trace(msg, __VA_ARGS__);
