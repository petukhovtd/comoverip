#include <rtu_client.h>
#include <common/logger.h>
#include <exchange/exchange.h>
#include <message_data.h>

using namespace asio;

namespace comoverip
{

RtuClient::RtuClient( const std::shared_ptr< io_context >& ioContext,
                      std::string portName,
                      serial_port::baud_rate baudRate,
                      serial_port::character_size characterSize,
                      serial_port::stop_bits stopBits,
                      serial_port::parity parity )
          : portName_( std::move( portName ) )
          , serialPort_( *ioContext )
          , dispatcherId_( defaultId )
          , isStarted_( false )
{
     serialPort_.open( portName_ );
     serialPort_.set_option( baudRate );
     serialPort_.set_option( characterSize );
     serialPort_.set_option( stopBits );
     serialPort_.set_option( parity );
}


RtuClient::~RtuClient()
{
     Stop();
}

void RtuClient::Receive( const std::shared_ptr< BaseMessage >& message )
{
     std::shared_ptr< MessageData > targetMessage = std::dynamic_pointer_cast< MessageData >( message );
     if( targetMessage )
     {
          Write( targetMessage->GetData() );
     }
}


void RtuClient::SetDispatcherId( ActorId id )
{
     dispatcherId_ = id;
}

void RtuClient::Start()
{
     if( !isStarted_ )
     {
          PushReadTask();
          isStarted_ = true;
     }
}

void RtuClient::Stop()
{
     if( isStarted_ )
     {
          serialPort_.cancel();
     }
     if( serialPort_.is_open() )
     {
          serialPort_.close();
     }
}

void RtuClient::PushReadTask()
{
     DataPtr data = BuildDataBuffer();
     Weak weak = GetWeak();
     async_read( serialPort_, buffer( *data ), transfer_at_least( 1 ),
                 [ weak, data ]( const error_code& ec, size_t size )
                 {
                      if( ec )
                      {
                           COIP_LOG_ERR( "RtuClient read failed: %s", ec.message().data() )
                           return;
                      }
                      Ptr self = weak.lock();
                      if( !self )
                      {
                           COIP_LOG_ERR( "RtuClient actor was deleted" )
                           return;
                      }
                      data->resize( size );
                      COIP_LOG_DEBUG( "RtuClient read %zu bytes", size )
                      Exchange::Send( self->dispatcherId_, MessageData::Create( data ) );
                      self->PushReadTask();
                 } );
}

void RtuClient::Write( const DataPtr& data )
{
     Weak weak = GetWeak();
     async_write( serialPort_, buffer( *data ),
                  [ weak, data ]( const error_code& ec, size_t size )
                  {
                       if( ec )
                       {
                            COIP_LOG_ERR( "RtuClient write failed: %s", ec.message().data() )
                            return;
                       }
                       COIP_LOG_DEBUG( "RtuClient write %zu bytes", size )
                  } );
}

}
