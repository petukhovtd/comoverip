#include <gtest/gtest.h>
#include <source_args_parser.h>

using namespace comoverip;

TEST( SourceArgParserTest, RtuClientBase )
{
     {
          RtuClient::Args args = ParsingRtuClientArgs( "device,b38400" );
          EXPECT_EQ( "device", args.portName );
          EXPECT_EQ( 38400, args.baudRate.value() );
          EXPECT_EQ( 8, args.characterSize.value() );
          EXPECT_EQ( asio::serial_port::parity::none, args.parity.value() );
          EXPECT_EQ( asio::serial_port::stop_bits::one, args.stopBits.value() );
     }
     EXPECT_NO_THROW( ParsingRtuClientArgs( "device,b9600" ) );
     EXPECT_NO_THROW( ParsingRtuClientArgs( "device,b1" ) );
     EXPECT_NO_THROW( ParsingRtuClientArgs( "device,b0" ) );
     EXPECT_NO_THROW( ParsingRtuClientArgs( "device,b123456" ) );
     EXPECT_NO_THROW( ParsingRtuClientArgs( "device,b123456" ) );

     EXPECT_THROW( ParsingRtuClientArgs( "" ), std::invalid_argument );
     EXPECT_THROW( ParsingRtuClientArgs( "," ), std::invalid_argument );
     EXPECT_THROW( ParsingRtuClientArgs( "device" ), std::invalid_argument );
     EXPECT_THROW( ParsingRtuClientArgs( "device," ), std::invalid_argument );
     EXPECT_THROW( ParsingRtuClientArgs( "device,38400" ), std::invalid_argument );
     EXPECT_THROW( ParsingRtuClientArgs( ",b38400" ), std::invalid_argument );
}

TEST( SourceArgParserTest, RtuClientOptional )
{
     {
          RtuClient::Args args = ParsingRtuClientArgs( "device,b38400,8n1" );
          EXPECT_EQ( "device", args.portName );
          EXPECT_EQ( 38400, args.baudRate.value() );
          EXPECT_EQ( 8, args.characterSize.value() );
          EXPECT_EQ( asio::serial_port::parity::none, args.parity.value() );
          EXPECT_EQ( asio::serial_port::stop_bits::one, args.stopBits.value() );
     }
     {
          RtuClient::Args args = ParsingRtuClientArgs( "device,b38400,6o1.5" );
          EXPECT_EQ( "device", args.portName );
          EXPECT_EQ( 38400, args.baudRate.value() );
          EXPECT_EQ( 6, args.characterSize.value() );
          EXPECT_EQ( asio::serial_port::parity::odd, args.parity.value() );
          EXPECT_EQ( asio::serial_port::stop_bits::onepointfive, args.stopBits.value() );
     }

     EXPECT_THROW( ParsingRtuClientArgs( "device,b38400,4n1" ), std::out_of_range );
     EXPECT_EQ( 5, ParsingRtuClientArgs( "device,b38400,5n1" ).characterSize.value() );
     EXPECT_EQ( 6, ParsingRtuClientArgs( "device,b38400,6n1" ).characterSize.value() );
     EXPECT_EQ( 7, ParsingRtuClientArgs( "device,b38400,7n1" ).characterSize.value() );
     EXPECT_EQ( 8, ParsingRtuClientArgs( "device,b38400,8n1" ).characterSize.value() );
     EXPECT_THROW( ParsingRtuClientArgs( "device,b38400,4n1" ), std::out_of_range );
     EXPECT_THROW( ParsingRtuClientArgs( "device,b38400,n1" ), std::invalid_argument );
     EXPECT_THROW( ParsingRtuClientArgs( "device,b38400,en1" ), std::invalid_argument );

     EXPECT_EQ( asio::serial_port::parity::none, ParsingRtuClientArgs( "device,b38400,8n1" ).parity.value() );
     EXPECT_EQ( asio::serial_port::parity::odd, ParsingRtuClientArgs( "device,b38400,8o1" ).parity.value() );
     EXPECT_EQ( asio::serial_port::parity::even, ParsingRtuClientArgs( "device,b38400,8e1" ).parity.value() );
     EXPECT_THROW( ParsingRtuClientArgs( "device,b38400,8k1" ), std::invalid_argument );
     EXPECT_THROW( ParsingRtuClientArgs( "device,b38400,811" ), std::invalid_argument );

     EXPECT_EQ( asio::serial_port::stop_bits::one, ParsingRtuClientArgs( "device,b38400,8n1" ).stopBits.value() );
     EXPECT_EQ( asio::serial_port::stop_bits::onepointfive,
                ParsingRtuClientArgs( "device,b38400,8n1.5" ).stopBits.value() );
     EXPECT_EQ( asio::serial_port::stop_bits::two, ParsingRtuClientArgs( "device,b38400,8n2" ).stopBits.value() );
     EXPECT_THROW( ParsingRtuClientArgs( "device,b38400,8n0" ), std::invalid_argument );
     EXPECT_THROW( ParsingRtuClientArgs( "device,b38400,8n" ), std::invalid_argument );
     EXPECT_THROW( ParsingRtuClientArgs( "device,b38400,8n3" ), std::invalid_argument );
     EXPECT_THROW( ParsingRtuClientArgs( "device,b38400,8nd" ), std::invalid_argument );
}

TEST( SourceArgParserTest, TcpClientArgs )
{
     {
          TcpClient::Args args = ParsingTcpClientArgs( "127.0.0.1:1234" );
          EXPECT_EQ( "127.0.0.1", args.addr.to_string() );
          EXPECT_EQ( 1234, args.port );
     }
     {
          TcpClient::Args args = ParsingTcpClientArgs( "192.168.1.1:6547" );
          EXPECT_EQ( "192.168.1.1", args.addr.to_string() );
          EXPECT_EQ( 6547, args.port );
     }
     EXPECT_THROW( ParsingTcpClientArgs( "" ), std::invalid_argument );
     EXPECT_THROW( ParsingTcpClientArgs( ":" ), std::invalid_argument );
     EXPECT_THROW( ParsingTcpClientArgs( "127.0.0.1" ), std::invalid_argument );
     EXPECT_THROW( ParsingTcpClientArgs( "127.0.0.1:" ), std::invalid_argument );
     EXPECT_THROW( ParsingTcpClientArgs( ":1234" ), std::invalid_argument );
     EXPECT_THROW( ParsingTcpClientArgs( "127.0.0.1:aaa" ), std::invalid_argument );
     EXPECT_THROW( ParsingTcpClientArgs( "127.0.0.1:123a" ), std::invalid_argument );
     EXPECT_THROW( ParsingTcpClientArgs( "999.999.999.999:1234" ), std::system_error );
}

TEST( SourceArgParserTest, UdpClientArgs )
{
     {
          UdpClient::Args args = ParsingUdpClientArgs( "127.0.0.1:1234" );
          EXPECT_EQ( "127.0.0.1", args.addr.to_string() );
          EXPECT_EQ( 1234, args.port );
     }
     {
          TcpClient::Args args = ParsingTcpClientArgs( "192.168.1.1:6547" );
          EXPECT_EQ( "192.168.1.1", args.addr.to_string() );
          EXPECT_EQ( 6547, args.port );
     }
     EXPECT_THROW( ParsingUdpClientArgs( "" ), std::invalid_argument );
     EXPECT_THROW( ParsingUdpClientArgs( ":" ), std::invalid_argument );
     EXPECT_THROW( ParsingUdpClientArgs( "127.0.0.1" ), std::invalid_argument );
     EXPECT_THROW( ParsingUdpClientArgs( "127.0.0.1:" ), std::invalid_argument );
     EXPECT_THROW( ParsingUdpClientArgs( ":1234" ), std::invalid_argument );
     EXPECT_THROW( ParsingUdpClientArgs( "127.0.0.1:aaa" ), std::invalid_argument );
     EXPECT_THROW( ParsingUdpClientArgs( "127.0.0.1:123a" ), std::invalid_argument );
     EXPECT_THROW( ParsingUdpClientArgs( "999.999.999.999:1234" ), std::system_error );
}

TEST( SourceArgParserTest, TcpServerArgs )
{
     EXPECT_EQ( 1234, ParsingTcpServerArgs( "1234" ).port );
     EXPECT_EQ( 4444, ParsingTcpServerArgs( "4444" ).port );

     EXPECT_THROW( ParsingTcpServerArgs( "" ), std::invalid_argument );
     EXPECT_THROW( ParsingTcpServerArgs( ":" ), std::invalid_argument );
     EXPECT_THROW( ParsingTcpServerArgs( "avc" ), std::invalid_argument );
     EXPECT_THROW( ParsingTcpServerArgs( "127.0.0.1:" ), std::invalid_argument );
}

TEST( SourceArgParserTest, MakeArgs )
{
     EXPECT_EQ( ISource::Type::RtuClient, MakeSourceArgs( "rtu", "dev,b38400" )->GetType() );
     EXPECT_EQ( ISource::Type::TcpClient, MakeSourceArgs( "tcp", "127.0.0.1:1234" )->GetType() );
     EXPECT_EQ( ISource::Type::TcpServer, MakeSourceArgs( "tcp-l", "1234" )->GetType() );
     EXPECT_EQ( ISource::Type::UdpClient, MakeSourceArgs( "udp", "127.0.0.1:1234" )->GetType() );
}