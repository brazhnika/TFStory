#pragma once

#include <memory>

#include "spdlog/spdlog.h"


class Logger
{
public:

   template<typename... Args>
   void Debug(Args...args)
   {
      if(!m_logger) return;
      m_logger->debug(args...);
   }

   template<typename... Args>
   void Info(Args... args)
   {
      if(!m_logger) return;
       m_logger->info(args...);
   }

   template<typename... Args>
   void Warn(Args... args)
   {
      if(!m_logger) return;
       m_logger->warn(args...);
   }

   template<typename... Args>
   void Error(Args... args)
   {
      if(!m_logger) return;
       m_logger->error(args...);
   }

   static Logger& GetLogger();

   void Flush() {m_logger->flush();}

private:
   Logger();
   static std::shared_ptr<spdlog::logger>& getLogger();
   std::shared_ptr<spdlog::logger> m_logger; 
};