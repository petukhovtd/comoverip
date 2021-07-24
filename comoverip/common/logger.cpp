#include <common/logger.h>
#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <chrono>
#include <array>

namespace comoverip
{


void Logger::Init( Logger::LogLevel logLevel )
{
     GetInstance().logLevel_ = logLevel;
}

void Logger::Log( Logger::LogLevel logLevel, std::string_view func, const char* format, ... )
{
     Logger& log = GetInstance();
     if( log.logLevel_ < logLevel )
     {
          return;
     }

     std::string_view loglevelName = GetLogLevelName( logLevel );

     va_list args;
     va_start( args, format );
     va_list copyArgs;
     va_copy( copyArgs, args );
     int messageSize = vsnprintf( nullptr, 0, format, copyArgs );
     va_end( copyArgs );
     if( messageSize <= 0 )
     {
          va_end( args );
          return;
     }

     std::string buffer;
     size_t it = 0;
     // {timestamp}_[{level}]_{__FUNCTION__}:_{message}\n
     // _[ ]_ :_ \n= 7ch
     buffer.resize( log.timeStampLen_ + loglevelName.size() + messageSize + func.size() + 7 );
     it += SPrintTimeStamp( &buffer[ 0 ], log.timeStampLen_ );
     buffer[ it++ ] = ' ';

     buffer[ it++ ] = '[';
     std::copy( loglevelName.begin(), loglevelName.end(), buffer.begin() + it );
     it += loglevelName.size();
     buffer[ it++ ] = ']';
     buffer[ it++ ] = ' ';

     std::copy( func.begin(), func.end(), buffer.begin() + it );
     it += func.size();
     buffer[ it++ ] = ':';
     buffer[ it++ ] = ' ';

     it += vsnprintf( &buffer[ it ], messageSize + 1, format, args );
     va_end( args );

     buffer[ it ] = '\n';

     std::clog << buffer << std::flush;
}

Logger::Logger()
          : logLevel_( LogDebug )
          , timeStampLen_( SPrintTimeStamp( nullptr, 0 ) + 1 )
{}

Logger& Logger::GetInstance()
{
     static Logger instance;
     return instance;
}

std::string_view Logger::GetLogLevelName( Logger::LogLevel logLevel )
{
     static const std::array< std::string, ( static_cast< size_t >( LogLevelSize ) ) > loglevelNames =
               {
                         "Crit", "Error", "Warning", "Notice", "Info", "Debug"
               };
     return loglevelNames[ logLevel ];
}

int Logger::SPrintTimeStamp( char* buffer, size_t maxLen )
{
     std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::now();
     std::time_t tp = std::chrono::system_clock::to_time_t( timePoint );
     uint64_t partSecond = ( timePoint.time_since_epoch().count() % 1000000000 ) / 1000;
     tm* tm = std::localtime( &tp );
     return snprintf( buffer, maxLen, "%04d-%02d-%02d %02d:%02d:%02d.%06lu",
                      ( tm->tm_year + 1900 ), tm->tm_mon, tm->tm_mday,
                      tm->tm_hour, tm->tm_min, tm->tm_sec, partSecond );
}

}
