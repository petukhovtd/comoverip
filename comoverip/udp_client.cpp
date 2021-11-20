#include <udp_client.h>
#include <message_data.h>
#include <common/logger.h>

using namespace asio;

namespace comoverip
{

UdpClient::UdpClient( const std::shared_ptr< io_context >& ioContext, const Args& args )
	  : targetEp_( args.addr, args.port )
	  , udpSocket_( *ioContext, ip::udp::endpoint( ip::address_v4::any(), args.port ) )
	  , isStarted_( false )
{}

UdpClient::~UdpClient()
{
     StopImpl();
}

void UdpClient::Receive( const std::shared_ptr< BaseMessage >& message )
{
	std::shared_ptr< MessageData > targetMessage = std::dynamic_pointer_cast< MessageData >( message );
	if( targetMessage )
	{
		Write( targetMessage->GetData() );
	}
}

void UdpClient::Start()
{
	if( !isStarted_ )
	{
		PushReadTask();
		isStarted_ = true;
	}
}

void UdpClient::Stop()
{
     StopImpl();
}

void UdpClient::PushReadTask()
{
	auto receiveEp = std::make_shared< ip::udp::endpoint >();
	auto data = comoverip::BuildDataBuffer();
	Weak weak = GetWeak();
	udpSocket_.async_receive_from( asio::buffer( *data ), *receiveEp, [ data, receiveEp, weak ]( const error_code& ec, size_t size )
	{
		if( ec )
		{
			COIP_LOG_ERR( "UdpClient read failed: %s", ec.message().data() )
			return;
		}
		Ptr self = weak.lock();
		if( !self )
		{
			COIP_LOG_ERR( "UdpClient actor was deleted" )
			return;
		}
		data->resize( size );
          COIP_LOG_DEBUG( "UdpClient read %llu bytes, from %s:%u", SIZE_T_TO_ULL( size ),
                          receiveEp->address().to_string().c_str(),
                          receiveEp->port() )
		Exchange::Send( self->writerId_, MessageData::Create( data ) );
		self->PushReadTask();
	});
}

void UdpClient::Write( const DataPtr& data )
{
	error_code errorCode;
	auto size = udpSocket_.send_to( asio::buffer( *data ), targetEp_, 0, errorCode );
     if( errorCode )
     {
          COIP_LOG_ERR( "UdpClient write failed: %s", errorCode.message().data() )
          return;
     }
     COIP_LOG_DEBUG( "UdpClient write %llu bytes", SIZE_T_TO_ULL( size ) )
}

void UdpClient::StopImpl()
{
     error_code ec;
     if( isStarted_ )
     {
          udpSocket_.cancel( ec );
     }
     if( udpSocket_.is_open() )
     {
          udpSocket_.close( ec );
     }
}

}
