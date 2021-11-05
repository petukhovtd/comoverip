#include <configurator.h>
#include <rtu_client.h>
#include <tcp_server.h>
#include <tcp_client.h>
#include <udp_client.h>

#include <asio.hpp>

#include <iostream>
#include <memory>

using IoContextPtr = std::shared_ptr< asio::io_context >;

namespace
{
using namespace comoverip;

SourcePtr MakeSource( const IoContextPtr& ioContext, const SourceArgsPtr& args )
{
     switch( args->GetType() )
     {
          case ISource::Type::RtuClient:
          {
               auto* argsPtr = dynamic_cast< RtuClient::Args* >( args.get() );
               if( argsPtr )
               {
                    return RtuClient::Create( ioContext, *argsPtr );
               }
          }
               break;
          case ISource::Type::TcpClient:
          {
               auto* argsPtr = dynamic_cast< TcpClient::Args* >( args.get() );
               if( argsPtr )
               {
                    return TcpClient::Create( ioContext, *argsPtr );
               }
          }
               break;
          case ISource::Type::TcpServer:
          {
               auto* argsPtr = dynamic_cast< TcpServer::Args* >( args.get() );
               if( argsPtr )
               {
                    return TcpServer::Create( ioContext, *argsPtr );
               }
          }
               break;
          case ISource::Type::UdpClient:
          {
               auto* argsPtr = dynamic_cast< UdpClient::Args* >( args.get() );
               if( argsPtr )
               {
                    return UdpClient::Create( ioContext, *argsPtr );
               }
          }
               break;
          default:
               break;
     }
     throw std::invalid_argument( "invalid source type" );
}

}

int main( int argc, char* argv[] )
{
     using namespace comoverip;

     try
     {
          const Configurator configurator( argc, argv );
          if( configurator.NeedPrintHelp() )
          {
               Configurator::PrintHelp( std::cout );
               return EXIT_SUCCESS;
          }
          configurator.CheckSourcesSize();

          Logger::Init( configurator.GetLogLevel() );

          IoContextPtr ioContext = std::make_shared< asio::io_context >();
          auto backgroundWork = std::make_shared< asio::io_service::work >( *ioContext );

          std::thread workThread = std::thread( [ ioContext ]()
                                                {
                                                     COIP_LOG_NOTICE( "ioContext run" )
                                                     ioContext->run();
                                                     COIP_LOG_NOTICE( "ioContext stop" )
                                                } );

          auto lhs = MakeSource( ioContext, configurator.GetLhsArgs() );
          auto rhs = MakeSource( ioContext, configurator.GetRhsArgs() );

          ActorId lhsId = Exchange::Insert( lhs );
          ActorId rhsId = Exchange::Insert( rhs );

          lhs->SetWriterId( rhsId );
          rhs->SetWriterId( lhsId );

          lhs->Start();
          rhs->Start();

          std::cin.get();

          lhs->Stop();
          rhs->Stop();

          backgroundWork.reset();
          if( workThread.joinable() )
          {
               ioContext->stop();
               workThread.joinable();
          }
     }
     catch( const std::exception& e )
     {
          std::cerr << e.what() << std::endl;
          return EXIT_FAILURE;
     }

     return EXIT_SUCCESS;
}
