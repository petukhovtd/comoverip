#include <gtest/gtest.h>
#include <configurator.h>
#include <asio.hpp>

using namespace comoverip;

TEST( ConfiguratorTest, CmdParsing )
{
     {
          char* argv[] = {
                    ( char* ) "name",
                    ( char* ) "-d",
                    ( char* ) "3",
                    ( char* ) "-h",
                    ( char* ) "rtu,dev,b38400",
                    ( char* ) "tcp-l,1234",
                    NULL
          };
          const Configurator configurator( 6, argv );
          EXPECT_EQ( Logger::LogLevel::LogNotice, configurator.GetLogLevel() );
          EXPECT_TRUE( configurator.NeedPrintHelp() );
          EXPECT_TRUE( ISource::Type::RtuClient == configurator.GetLhsArgs()->GetType() );
          EXPECT_TRUE( ISource::Type::TcpServer == configurator.GetRhsArgs()->GetType() );
     }
     {
          char* argv[] = {
                    ( char* ) "name",
                    ( char* ) "-d",
                    ( char* ) "2",
                    ( char* ) "tcp,127.0.0.1:38400",
                    ( char* ) "udp,127.0.0.1:1234",
                    NULL
          };
          const Configurator configurator( 5, argv );
          EXPECT_EQ( Logger::LogLevel::LogWarning, configurator.GetLogLevel() );
          EXPECT_FALSE( configurator.NeedPrintHelp() );
          EXPECT_TRUE( ISource::Type::TcpClient == configurator.GetLhsArgs()->GetType() );
          EXPECT_TRUE( ISource::Type::UdpClient == configurator.GetRhsArgs()->GetType() );
     }
     {
          char* argv[] = {
                    ( char* ) "name",
                    ( char* ) "tcp,127.0.0.1:38400",
                    ( char* ) "udp,127.0.0.1:1234",
                    NULL
          };
          const Configurator configurator( 3, argv );
          EXPECT_EQ( Logger::LogLevel::LogNotice, configurator.GetLogLevel() );
          EXPECT_TRUE( ISource::Type::TcpClient == configurator.GetLhsArgs()->GetType() );
          EXPECT_TRUE( ISource::Type::UdpClient == configurator.GetRhsArgs()->GetType() );
     }
     {
          char* argv[] = {
                    ( char* ) "name",
                    ( char* ) "tcp,127.0.0.1:38400",
                    NULL
          };
          Configurator configurator( 2, argv );
          EXPECT_THROW( configurator.CheckSourcesSize(), std::invalid_argument );
     }
     {
          char* argv[] = {
                    ( char* ) "name",
                    ( char* ) "tcp,127.0.0.1:38400",
                    ( char* ) "tcp,127.0.0.1:38400",
                    ( char* ) "tcp,127.0.0.1:38400",
                    NULL
          };
          Configurator configurator( 4, argv );
          EXPECT_THROW( configurator.CheckSourcesSize(), std::invalid_argument );
     }
}
