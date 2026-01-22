#pragma once
#ifndef MYENGINE_UTILS_LOGGER_H
#define MYENGINE_UTILS_LOGGER_H
#include "../Libs.h"
#include "FileSystem.h"
namespace MyEngine {
    class Logger {
    public:
        enum LogLevel : uint8_t {
            Debug,
            Info,
            Warn,
            Error,
            Fatal
        };

        Logger() = delete;
        Logger(Logger &&) = delete;
        Logger(const Logger &) = delete;
        Logger &operator=(Logger &&) = delete;
        Logger &operator=(const Logger &) = delete;
        ~Logger() = delete;

        static void log(const std::string &message, LogLevel level = Debug) {
            if (level < _base_level) return;
            _running_time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            auto _real_time = (float) (_running_time - _started_time) / 1e9;
            std::string log;
            if (level >= Warn) {
                log = std::format("[{:.06f}] [{}] {}\n", _real_time, _logLevelToString(level), message);
                std::cerr << log;
            } else {
                log = std::format("[{:.06f}] [{}] {}\n", _real_time, _logLevelToString(level), message);
                std::cout << log;
            }
            if (_write_log) {
                _writeLogToFile(log);
            }
            _last_log_level = level;
            _last_log_info = message;
        }

        template<typename ...Args>
        static void log(LogLevel level, std::string_view format, Args... args) {
            if (level < _base_level) return;
            _running_time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            auto _real_time = (float) (_running_time - _started_time) / 1e9;
            std::string fmt = std::format("[{:.06f}] [{}] ", _real_time, _logLevelToString(level));
            std::string msg;
            try {
                std::vformat_to(std::back_inserter(msg), format, std::make_format_args(args...));
                std::cout << fmt << msg << "\n";
                if (_write_log) {
                    _writeLogToFile(fmt);
                }
                _last_log_level = level;
                _last_log_info = msg;
            } catch (const std::format_error &e) {
                _last_log_info = std::format("[{:.06f}] [{}] Logger: {}\n",
                                             _real_time, _logLevelToString(Error), e.what());
                std::cerr << _last_log_info;
                _last_log_level = Error;
            }
        }

        static void setBaseLogLevel(LogLevel level) {
            _base_level = level;
        }

        static std::string lastError() {
            return (_last_log_level >= Error ? _last_log_info : "");
        }

        static void setLogFileName(const std::string& file_name) {
            _output_file_name = file_name;
        }

        static std::string currentLogFileName() {
            return _output_file_name;
        }

        static void setWriteLogFileEnabled(bool enabled) {
            _write_log = enabled;
        }

        static bool writeLogFileEnabled() {
            return _write_log;
        }

    private:
        static std::string _logLevelToString(LogLevel level) {
            switch (level) {
                case Debug:
                    return "DEBUG";
                case Info:
                    return "INFO";
                case Warn:
                    return "WARN";
                case Error:
                    return "ERROR";
                case Fatal:
                    return "FATAL";
                default:
                    return "UNKNOWN";
            }
        }

        static void _writeLogToFile(const std::string& log) {
            FILE* file = fopen(_output_file_name.c_str(), "a+");
            if (!file) {
                _write_log = false;
                Logger::log(std::format("Logger: Can't open file '{}' to write log! ", _output_file_name), Warn);
            }
            fprintf(file, "%s", log.c_str());
            fflush(file);
            fclose(file);
        }

        inline static LogLevel _base_level{Logger::Info};
        inline static LogLevel _last_log_level{Logger::Debug};
        inline static std::string _last_log_info{};
        inline static uint64_t _started_time
            {static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count())};
        inline static uint64_t _running_time{};
        inline static std::string _output_file_name{"./App.log"};
        inline static bool _write_log{false};
    };
}
#endif //MYENGINE_UTILS_LOGGER_H
