#include <configurator.h>
#include <string_parser.h>
#include <source_args_parser.h>

#include <iostream>

namespace comoverip
{

Configurator::Configurator( int argc, char* argv[] )
          : logLevel_( Logger::LogNotice )
          , sourceArgs_()
          , printHelp_( false )
{
     sourceArgs_.reserve( SourcesSize );
     ParsingArgs( argc, argv );
}


void Configurator::CheckSourcesSize() const
{
     if( sourceArgs_.size() != SourcesSize )
     {
          throw std::invalid_argument( "invalid sources size" );
     }
}

void Configurator::PrintHelp( std::ostream& os )
{
     os
               << "comoverip [options] [source1] [source2]\n"
               << "\n"
               << "[options]\n"
               << "\t-h\tprint help\n"
               << "\t-d\tdebug level 0-5\n"
               << "\n"
               << "[source1] and [source2]\n"
               << "rtu,<dev>,b<baudrate>,<character_size(5-8)><parity(n(one),o(dd),e(ven))><stop_bits(1,1.5,2)>\n"
               << "tcp-l,<port>\n"
               << "tcp,<ip>:<port>\n"
               << "udp,<ip>:<port>\n"
               << std::endl;
}

Logger::LogLevel Configurator::GetLogLevel() const
{
     return logLevel_;
}

SourceArgsPtr Configurator::GetLhsArgs() const
{
     return sourceArgs_[ Lhs ];
}

SourceArgsPtr Configurator::GetRhsArgs() const
{
     return sourceArgs_[ Rhs ];
}

void Configurator::ParsingArgs( int argc, char* argv[] )
{
     for( int i = 1; i < argc; ++i )
     {
          if( argv[ i ][ 0 ] == '-' )
          {
               if( ParsingOptions( argv[ i ], ( i != ( argc - 1 ) )? argv[ i + 1 ]: "" ) )
               {
                    ++i;
               }
          }
          else
          {
               ParsingSource( argv[ i ] );
          }
     }
}

bool Configurator::ParsingOptions( std::string_view current, std::string_view next )
{
     bool usedNext = false;
     if( current == "-h" || current == "--help" )
     {
          printHelp_ = true;
     }
     else if( current == "-d" || current == "--debug" )
     {
          std::istringstream is( next.data() );
          int logLevel;
          is >> logLevel;
          if( is && logLevel < Logger::LogLevelSize && logLevel >= Logger::LogCrit )
          {
               logLevel_ = static_cast<Logger::LogLevel>( logLevel );
               usedNext = true;
          }
          else
          {
               throw std::invalid_argument( "invalid debug level: " + std::string( next ) );
          }
     }
     return usedNext;
}

void Configurator::ParsingSource( std::string_view s )
{
     std::string_view type = ReadToken( s, "," );
     sourceArgs_.push_back( MakeSourceArgs( type, s ) );
}

bool Configurator::NeedPrintHelp() const
{
     return printHelp_;
}

}
