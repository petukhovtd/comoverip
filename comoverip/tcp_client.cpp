#include <tcp_client.h>
#include <message_data.h>
#include <common/logger.h>

using namespace asio;

namespace comoverip
{

TcpClient::TcpClient( const std::shared_ptr< asio::io_context >& ioContext, const asio::ip::address& addr,
                      asio::ip::port_type port )
: tcpSocket_( *ioContext )
, isStarted_( false )
{
     ip::tcp::endpoint ep( addr, port );
     tcpSocket_.connect( ep );
}

TcpClient::~TcpClient()
{
     StopImpl();
}

void TcpClient::Receive( const std::shared_ptr< BaseMessage >& message )
{
     std::shared_ptr< MessageData > targetMessage = std::dynamic_pointer_cast< MessageData >( message );
     if( targetMessage )
     {
          Write( targetMessage->GetData() );
     }
}

void TcpClient::Start()
{
     if( !isStarted_ )
     {
          isStarted_ = true;
          PushReadTask();
     }
}

void TcpClient::Stop()
{
     StopImpl();
}

void TcpClient::PushReadTask()
{
     auto data = comoverip::BuildDataBuffer();
     Weak weak = GetWeak();
     tcpSocket_.async_receive( buffer( *data ), [ data, weak ]( const error_code& ec, size_t size )
     {
          if( ec )
          {
               COIP_LOG_ERR( "TcpClient read failed: %s", ec.message().data() )
               return;
          }
          Ptr self = weak.lock();
          if( !self )
          {
               COIP_LOG_ERR( "TcpClient actor was deleted" )
               return;
          }
          data->resize( size );
          COIP_LOG_DEBUG( "TcpClient read %zu bytes", size )
          Exchange::Send( self->writerId_, MessageData::Create( data ) );
          self->PushReadTask();
     });
}

void TcpClient::Write( const DataPtr& data )
{
     error_code errorCode;
     auto size = tcpSocket_.send( asio::buffer(*data ), 0, errorCode );
     if( errorCode )
     {
          COIP_LOG_ERR( "TcpClient write failed: %s", errorCode.message().data() )
          return;
     }
     COIP_LOG_DEBUG( "TcpClient write %zu bytes", size )
}

void TcpClient::StopImpl()
{
     error_code ec;
     if( isStarted_ )
     {
          tcpSocket_.cancel( ec );
     }
     if( tcpSocket_.is_open() )
     {
          tcpSocket_.close( ec );
     }
}

}
