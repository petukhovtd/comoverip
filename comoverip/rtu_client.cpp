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
     StopImpl();
}

void RtuClient::Receive( const std::shared_ptr< BaseMessage >& message )
{
     std::shared_ptr< MessageData > targetMessage = std::dynamic_pointer_cast< MessageData >( message );
     if( targetMessage )
     {
          Write( targetMessage->GetData() );
     }
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
     StopImpl();
}

void RtuClient::PushReadTask()
{
     DataPtr data = BuildDataBuffer();
     Weak weak = GetWeak();
     serialPort_.async_read_some( buffer( *data ),[ weak, data ]( const error_code& ec, const size_t size )
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
                      Exchange::Send( self->writerId_, MessageData::Create( data ) );
                      self->PushReadTask();
                 } );
}

void RtuClient::Write( const DataPtr& data )
{
     Weak weak = GetWeak();
     Ptr self = weak.lock();
     error_code errorCode;
     auto size = serialPort_.write_some( buffer( *data ), errorCode );
     if( errorCode )
     {
          COIP_LOG_ERR( "RtuClient write failed: %s", errorCode.message().data() )
          return;
     }
     COIP_LOG_DEBUG( "RtuClient write %zu bytes", size )
}

void RtuClient::StopImpl()
{
     error_code ec;
     if( isStarted_ )
     {
          serialPort_.cancel( ec );
     }
     if( serialPort_.is_open() )
     {
          serialPort_.close( ec );
     }
}

}
