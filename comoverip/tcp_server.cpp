#include <tcp_server.h>
#include <exchange/exchange.h>
#include <common/logger.h>
#include <message_data.h>

using namespace asio;

namespace comoverip
{

TcpServer::TcpServer( const std::shared_ptr< asio::io_context >& ioContext, const Args& args )
: acceptor_( *ioContext, ip::tcp::endpoint( ip::address_v4::any(), args.port ) )
, socket_( *ioContext )
, isStarted_( false )
{}

void TcpServer::Receive( const std::shared_ptr< BaseMessage >& message )
{
     std::shared_ptr< MessageData > targetMessage = std::dynamic_pointer_cast< MessageData >( message );
     if( targetMessage )
     {
          Write( targetMessage->GetData() );
     }
}

TcpServer::~TcpServer()
{
     StopImpl();
}

void TcpServer::Start()
{
     if( !isStarted_ )
     {
          isStarted_ = true;
          PushAcceptTask();
     }
}

void TcpServer::Stop()
{
     StopImpl();
}

void TcpServer::PushAcceptTask()
{
     Weak weak = GetWeak();
     acceptor_.async_accept( socket_, [ weak ]( const error_code& ec )
     {
          if( ec )
          {
               COIP_LOG_ERR( "TcpServer accept failed: %s", ec.message().data() )
               return;
          }
          Ptr self = weak.lock();
          if( !self )
          {
               COIP_LOG_ERR( "TcpServer actor was deleted" )
               return;
          }
          COIP_LOG_DEBUG( "TcpServer connect from %s:%d", self->socket_.remote_endpoint().address().to_string().c_str(),
                    self->socket_.remote_endpoint().port() )
          self->PushReadTask();
     } );
}

void TcpServer::PushReadTask()
{
     auto data = comoverip::BuildDataBuffer();
     Weak weak = GetWeak();
     socket_.async_receive( buffer( *data ),
          [ weak, data ]( const error_code& ec, const std::size_t size )
          {
               Ptr self = weak.lock();
               if( !self )
               {
                    COIP_LOG_ERR( "TcpServer actor was deleted" )
                    return;
               }
               if( !ec )
               {
                    COIP_LOG_DEBUG( "TcpServer read %llu bytes", size )
                    data->resize( size );
                    Exchange::Send( self->writerId_, MessageData::Create( data ) );
                    self->PushReadTask();
                    return;
               }
               if(( error::eof == ec ) || ( error::connection_reset == ec ))
               {
                    COIP_LOG_INFO( "TcpServer client disconnect, wait new connect" );

                    if( self->socket_.is_open() )
                    {
                         self->socket_.close();
                    }
                    self->PushAcceptTask();
                    return;
               }

               COIP_LOG_ERR( "TcpClient read failed: %s", ec.message().data() )
          } );
}

void TcpServer::Write( const DataPtr& data )
{
     if( !socket_.is_open() )
     {
          return;
     }

     error_code errorCode;
     auto size = socket_.send( asio::buffer(*data ), 0, errorCode );
     if( errorCode )
     {
          COIP_LOG_ERR( "TcpServer write failed: %s", errorCode.message().data() )
          return;
     }
     COIP_LOG_DEBUG( "TcpServer write %llu bytes", size )
}

void TcpServer::StopImpl()
{
     error_code ec;
     if( isStarted_ )
     {
          acceptor_.cancel( ec );
          socket_.cancel( ec );
     }
     if( acceptor_.is_open() )
     {
          acceptor_.close( ec );
     }
     if( socket_.is_open() )
     {
          socket_.close( ec );
     }
}

}
