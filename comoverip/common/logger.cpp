#include <common/logger.h>
#include <memory>
#include <cstdarg>
#include <map>
#include <chrono>
#include <iomanip>
#include <iostream>

namespace comoverip
{

void Logger::Crit( const char* format, ... )
{
     Logger& logger = GetInstance();
     va_list args;
     va_start( args, format );
     logger.Log( LogCrit, format, args );
     va_end( args );
}

void Logger::Err( const char* format, ... )
{
     Logger& logger = GetInstance();
     va_list args;
     va_start( args, format );
     logger.Log( LogErr, format, args );
     va_end( args );
}

void Logger::Warning( const char* format, ... )
{
     Logger& logger = GetInstance();
     va_list args;
     va_start( args, format );
     logger.Log( LogWarning, format, args );
     va_end( args );
}

void Logger::Notice( const char* format, ... )
{
     Logger& logger = GetInstance();
     va_list args;
     va_start( args, format );
     logger.Log( LogNotice, format, args );
     va_end( args );
}

void Logger::Info( const char* format, ... )
{
     Logger& logger = GetInstance();
     va_list args;
     va_start( args, format );
     logger.Log( LogInfo, format, args );
     va_end( args );
}

void Logger::Debug( const char* format, ... )
{
     Logger& logger = GetInstance();
     va_list args;
     va_start( args, format );
     logger.Log( LogDebug, format, args );
     va_end( args );
}

void Logger::InitSyslog( const std::string& appName )
{
     Logger& logger = GetInstance();
     logger.toSyslog_ = true;
     openlog( appName.c_str(), LOG_NDELAY | LOG_PID, LOG_USER );
     setlogmask( LOG_UPTO( logger.logLevel_ ));
}

void Logger::SetLogLevel( Logger::LogLevel logLevel )
{
     Logger& logger = GetInstance();
     logger.logLevel_ = logLevel;
     if( logger.toSyslog_ )
     {
          setlogmask( LOG_UPTO( logger.logLevel_ ));
     }
}

void Logger::EnablePrefix()
{
     Logger& logger = GetInstance();
     logger.levelPrefix_ = true;
}

void Logger::EnableTimeStamp()
{
     Logger& logger = GetInstance();
     logger.timeStamp_ = true;
}

Logger::Logger()
: logLevel_( LogDebug )
, toSyslog_( false )
, levelPrefix_( false )
, timeStamp_( false )
{}


Logger::~Logger()
{
     if( toSyslog_ )
     {
          closelog();
     }
}

Logger& Logger::GetInstance()
{
     static std::unique_ptr< Logger > instance( new Logger );
     return *instance;
}

void Logger::Log( LogLevel logLevel, const char* format, va_list args )
{
     Logger& logger = GetInstance();
     if( !logger.toSyslog_ )
     {
          if( logLevel > logger.logLevel_ )
          {
               return;
          }
          std::stringstream ss;
          if( logger.timeStamp_ )
          {
               ss << '[' << GetTimeStamp() << "] ";
          }
          if( logger.levelPrefix_ )
          {
               ss << '[' << GetPrefix( logLevel ) << "] ";
          }
          char* message = NULL;
          if( -1 != vasprintf( &message, format, args ) )
          {
               ss << message;
               std::lock_guard< std::mutex > lock( logger.mutex_ );
               std::cout << ss.str() << std::endl << std::flush;
          }
          free( message );
     }
     else
     {
          vsyslog( logLevel, format, args );
     }
}

std::string Logger::GetPrefix( LogLevel targetLevel )
{
     static std::map< LogLevel, std::string > prefixMap =
               {
                         { LogCrit,    "Critical" },
                         { LogErr,     "Error" },
                         { LogWarning, "Warning" },
                         { LogNotice,  "Notice" },
                         { LogInfo,    "Info" },
                         { LogDebug,   "Debug" },
               };

     if( prefixMap.count( targetLevel ) == 0 )
     {
          return std::string();
     }

     return prefixMap[ targetLevel ];
}

std::string Logger::GetTimeStamp()
{
     std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::now();
     std::time_t now_c = std::chrono::system_clock::to_time_t( timePoint );

     uint64_t timeSinesEpoch = timePoint.time_since_epoch().count();
     uint64_t millisNanos = timeSinesEpoch % 1000000;

     std::stringstream ss;
     ss << std::put_time( std::localtime( &now_c ), "%F %T" ) << '.' << std::setw( 6 ) << std::setfill( '0' )
        << millisNanos;
     return ss.str();
}




}
