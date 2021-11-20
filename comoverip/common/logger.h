#ifndef COMOVERIP_LOGGER_H
#define COMOVERIP_LOGGER_H

#include <string>
#include <string_view>
#include <atomic>

namespace comoverip
{

/// @brief Класс Logger.
class Logger
{
public:
     /// @brief Уровень логирования
     enum LogLevel: int
     {
          LogCrit = 0,
          LogErr,
          LogWarning,
          LogNotice,
          LogInfo,
          LogDebug,
          LogLevelSize,
     };

     /// @brief Установка уровня логирования
     /// @param[in] logLevel
     static void Init( LogLevel logLevel );

     /// @brief Печать сообщения в лог
     /// @param[in] logLevel уровень сообщения
     /// @param[in] func имя функции
     /// @param[in] format формат сообщения
     /// @param ... аргументы формата
     static void Log( LogLevel logLevel, std::string_view func, const char* format, ... )
     __attribute__ ((__format__ (__printf__, 3, 4)));

private:
     Logger();

     static Logger& GetInstance();

     static std::string_view GetLogLevelName( LogLevel logLevel );

     static int SPrintTimeStamp( char* buffer, size_t maxLen );

private:
     std::atomic< LogLevel > logLevel_;
     size_t timeStampLen_;
};

}

#define COIP_LOG( logLevel, format, ... ) \
     comoverip::Logger::Log( logLevel, __FUNCTION__, format, ##__VA_ARGS__ );

#define COIP_LOG_CRIT( format, ... ) COIP_LOG( comoverip::Logger::LogCrit, format, ##__VA_ARGS__ )
#define COIP_LOG_ERR( format, ... ) COIP_LOG( comoverip::Logger::LogErr, format, ##__VA_ARGS__ )
#define COIP_LOG_WARNING( format, ... ) COIP_LOG( comoverip::Logger::LogWarning, format, ##__VA_ARGS__ )
#define COIP_LOG_NOTICE( format, ... ) COIP_LOG( comoverip::Logger::LogNotice, format, ##__VA_ARGS__ )
#define COIP_LOG_INFO( format, ... ) COIP_LOG( comoverip::Logger::LogInfo, format, ##__VA_ARGS__ )
#define COIP_LOG_DEBUG( format, ... ) COIP_LOG( comoverip::Logger::LogDebug, format, ##__VA_ARGS__ )

#define SIZE_T_TO_ULL( s ) static_cast< unsigned long long >( s )

#endif
