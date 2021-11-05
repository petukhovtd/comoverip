#include <source_args_parser.h>
#include <string_parser.h>

#include <sstream>

namespace comoverip
{

static const std::string_view delimiter = ",";

RtuClient::Args ParsingRtuClientArgs( std::string_view s )
{
     std::string_view portName = ReadToken( s, delimiter );
     if( portName.empty() )
     {
          throw std::invalid_argument( "don't config rtu port name" );
     }

     const asio::serial_port::baud_rate baudRate = [ &s ]()
     {
          std::string_view baudRate = ReadToken( s, delimiter );
          if( baudRate.empty() )
          {
               throw std::invalid_argument( "don't config rtu baud rate" );
          }
          if( baudRate.front() != 'b' )
          {
               throw std::invalid_argument( "don't found rtu baud rate" );
          }
          baudRate.remove_prefix( 1 );
          unsigned int baud;
          std::istringstream is( baudRate.data() );
          is >> baud;
          if( is )
          {
               return asio::serial_port::baud_rate( baud );
          }

          throw std::invalid_argument( "invalid baud rate: " + std::string( baudRate ) );
     }();

     RtuClient::Args args( std::string( portName ), baudRate );
     std::string_view options = ReadToken( s, delimiter );
     if( !options.empty() )
     {
          std::istringstream is( options.data() );
          if( !std::isdigit( is.peek() ) )
          {
               throw std::invalid_argument( "invalid character size: " + std::string( is.peek(), 1 ) );
          }
          const unsigned int characterSize = is.get() - 0x30;
          if( !std::isalpha( is.peek() ) )
          {
               throw std::invalid_argument( "invalid parity: " + std::string( is.peek(), 1 ) );
          }
          const char parityStr = is.get();
          const asio::serial_port::parity::type parity = [ &parityStr ]()
          {
               switch( parityStr )
               {
                    case 'n': return asio::serial_port::parity::none;
                    case 'o': return asio::serial_port::parity::odd;
                    case 'e': return asio::serial_port::parity::even;
                    default:
                         throw std::invalid_argument( "unknown parity: " + std::string( parityStr, 1 ) );
               }
          }();
          std::string stopBitsStr;
          is >> stopBitsStr;
          const asio::serial_port::stop_bits::type stopBits = [ &stopBitsStr ]()
          {
               if( stopBitsStr == "1" )
               {
                    return asio::serial_port::stop_bits::type::one;
               }
               if( stopBitsStr == "1.5" )
               {
                    return asio::serial_port::stop_bits::type::onepointfive;
               }
               if( stopBitsStr == "2" )
               {
                    return asio::serial_port::stop_bits::type::two;
               }
               throw std::invalid_argument( "invalid stop bits: " + stopBitsStr );
          }();

          args.characterSize = asio::serial_port::character_size( characterSize );
          args.parity = asio::serial_port::parity( parity );
          args.stopBits = asio::serial_port::stop_bits( stopBits );
     }
     return args;
}

TcpClient::Args ParsingTcpClientArgs( std::string_view s )
{
     const auto[ addr, port ] = ReadIpAddrAndPort( s );

     TcpClient::Args args( asio::ip::address_v4::from_string( std::string( addr ) ),
                           static_cast< asio::ip::port_type >( port ) );
     return args;
}

TcpServer::Args ParsingTcpServerArgs( std::string_view s )
{
     TcpServer::Args args( static_cast< asio::ip::port_type >( ReadIpPort( s ) ) );
     return args;
}

UdpClient::Args ParsingUdpClientArgs( std::string_view s )
{
     const auto[ addr, port ] = ReadIpAddrAndPort( s );

     UdpClient::Args args( asio::ip::address_v4::from_string( std::string( addr ) ),
                           static_cast< asio::ip::port_type >( port ) );
     return args;
}

std::pair< std::string_view, uint16_t > ReadIpAddrAndPort( std::string_view s )
{
     std::string_view address = ReadToken( s, ":" );
     if( address.empty() )
     {
          throw std::invalid_argument( "don't have ip address" );
     }

     return { address, ReadIpPort( s ) };
}

uint16_t ReadIpPort( std::string_view s )
{
     if( s.empty() )
     {
          throw std::invalid_argument( "don't have ip port" );
     }
     std::istringstream is( s.data() );
     uint16_t port;
     is >> port;
     if( !is || !is.eof() )
     {
          throw std::invalid_argument( "invalid ip port: " + std::to_string( port ) );
     }
     return port;
}

SourceArgsPtr MakeSourceArgs( const std::string_view& type, std::string_view args )
{
     if( type == "rtu" )
     {
          return std::make_shared< RtuClient::Args >( ParsingRtuClientArgs( args ) );
     }
     else if( type == "tcp-l" )
     {
          return std::make_shared< TcpServer::Args >( ParsingTcpServerArgs( args ) );
     }
     else if( type == "tcp" )
     {
          return std::make_shared< TcpClient::Args >( ParsingTcpClientArgs( args ) );
     }
     else if( type == "udp" )
     {
          return std::make_shared< UdpClient::Args >( ParsingUdpClientArgs( args ) );
     }
     throw std::invalid_argument( "unknown source type: " + std::string( type ) );
}

}
