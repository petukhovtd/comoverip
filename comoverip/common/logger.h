#ifndef COMOVERIP_LOGGER_H
#define COMOVERIP_LOGGER_H

#include <string>
#include <syslog.h>
#include <vector>
#include <ostream>
#include <mutex>

namespace comoverip
{

/// @brief Логирование в syslog или std::cout
/// для std::cout потокобезопасен
class Logger
{
public:
     /// @brief Уровень логирования
     enum LogLevel: int
     {
          LogCrit = LOG_CRIT,
          LogErr = LOG_ERR,
          LogWarning = LOG_WARNING,
          LogNotice = LOG_NOTICE,
          LogInfo = LOG_INFO,
          LogDebug = LOG_DEBUG,
     };

     /// @brief Деструктор класса
     ~Logger();

     /// @brief Сообщение логирования уровня Crit
     /// @param format
     /// @param ...
     static void Crit( const char* format ... );

     /// @brief Сообщение логирования уровня Err
     /// @param format
     /// @param ...
     static void Err( const char* format ... );

     /// @brief Сообщение логирования уровня Warning
     /// @param format
     /// @param ...
     static void Warning( const char* format ... );

     /// @brief Сообщение логирования уровня Notice
     /// @param format
     /// @param ...
     static void Notice( const char* format ... );

     /// @brief Сообщение логирования уровня Info
     /// @param format
     /// @param ...
     static void Info( const char* format ... );

     /// @brief Сообщение логирования уровня Debug
     /// @param format
     /// @param ...
     static void Debug( const char* format ... );

     /// @brief Инициализация syslog
     /// @param appName имя приложения
     static void InitSyslog( std::string const& appName );

     /// @brief Установить уровень логирования
     /// @param logLevel
     static void SetLogLevel( LogLevel logLevel );

     /// @brief Добавлять название уровня логирования
     /// только для консоли
     static void EnablePrefix();

     /// @brief Добавлять метку времени
     /// только для консоли
     static void EnableTimeStamp();

private:
     Logger();

     static Logger& GetInstance();

     void Log( LogLevel logLevel, const char* format, va_list args );

     static std::string GetPrefix( LogLevel targetLevel );

     static std::string GetTimeStamp();

private:
     LogLevel logLevel_;
     bool toSyslog_;
     bool levelPrefix_;
     bool timeStamp_;
     std::mutex mutex_;
};

}

#endif
