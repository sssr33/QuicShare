#include "Log.h"

void Log::Init(const std::string logPath, QTextEdit* logWidget) {
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath);
    auto qtSink = std::make_shared<spdlog::sinks::qt_color_sink_mt>(logWidget, 99999, false, true);

    std::vector<spdlog::sink_ptr> sinks{ fileSink, qtSink };

    auto logger = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug);

    LOG_INFO("\n");
    LOG_INFO("======== NEW SESSION ========\n");
}
