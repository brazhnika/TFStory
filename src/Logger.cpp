
#include <stdarg.h>
#include <iostream>
#include "Logger.h"
#include "spdlog/sinks/rotating_file_sink.h"


Logger& Logger::GetLogger()
{
    static Logger s_logger{};
    return s_logger;
}

// void Logger::Debug(const char * f, ... )
// {
//     if(!m_logger) return;

//     va_list args;
//     va_start(args, f);
//     m_logger->debug(f, args);
//     va_end(args);
// }

// void Logger::Info(const char * f, ... )
// {
//     if(!m_logger) return;

//     va_list args;
//     va_start(args, f);
//     m_logger->info(f, args);
//     va_end(args);
// }

// void Logger::Warn(const char * f, ... )
// {
//     va_list args;
//     va_start(args, f);

//     if(m_logger)
//        m_logger->warn(f, args);
//     else
//         printf(f, args);
       
//     va_end(args);
// }

// void Logger::Error(const char * f, ... )
// {
//     va_list args;
//     va_start(args, f);

//     if(m_logger)
//        m_logger->error(f, args);
//     else
//         printf(f, args);

//     va_end(args);
// }

// void Logger::Debug(const std::string& str)
// {
//     if(m_logger)
//        m_logger->debug(str);
//     else
//         printf("%s", str.c_str());
// }

// void Logger::Info(const std::string& str)
// {
//     if(m_logger)
//        m_logger->info(str);
//     else
//         printf("%s", str.c_str());
// }

// void Logger::Warn(const std::string& str)
// {
//     if(m_logger)
//        m_logger->warn(str);
//     else
//         printf("%s", str.c_str());
// }

// void Logger::Error(const std::string& str)
// {
//     if(m_logger)
//        m_logger->error(str);
//     else
//         printf("%s", str.c_str());
// }


Logger::Logger()
{
    try
    {       
       m_logger = spdlog::rotating_logger_mt("Logger", "logs/Default.log", 1048576 * 5, 3);
       m_logger->set_level(spdlog::level::trace);
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cout << "Log init failed: " << ex.what() << std::endl;
        m_logger = nullptr;
    }
}
