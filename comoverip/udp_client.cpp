#include <udp_client.h>
#include <message_data.h>
#include <common/logger.h>

using namespace asio;

namespace comoverip
{

UdpClient::UdpClient( const std::shared_ptr< io_context >& ioContext, const ip::address& addr, ip::port_type port )
	  : targetEp_( addr, port )
	  , receiveSocket_( *ioContext, targetEp_ )
	  , sendSocket_( *ioContext, ip::udp::endpoint() )
	  , dispatcherId_( defaultId )
	  , isStarted_( false )
{}

void UdpClient::Receive( const std::shared_ptr< BaseMessage >& message )
{
	std::shared_ptr< MessageData > targetMessage = std::dynamic_pointer_cast< MessageData >( message );
	if( targetMessage )
	{
		Write( targetMessage->GetData() );
	}
}

void UdpClient::SetDispatcherId( ActorId id )
{
	dispatcherId_ = id;
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
	if( isStarted_ )
	{
		sendSocket_.cancel();
		receiveSocket_.cancel();
	}
	if( sendSocket_.is_open() )
	{
		sendSocket_.close();
	}
	if( receiveSocket_.is_open() )
	{
		receiveSocket_.close();
	}
}

void UdpClient::PushReadTask()
{
	auto receiveEp = std::make_shared< ip::udp::endpoint >();
	auto data = comoverip::BuildDataBuffer();
	Weak weak = GetWeak();
	receiveSocket_.async_receive_from( asio::buffer( *data ), *receiveEp, [ data, receiveEp, weak ]( const error_code& ec, size_t size )
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
		COIP_LOG_DEBUG( "UdpClient read %zu bytes", size )
		Exchange::Send( self->dispatcherId_, MessageData::Create( data ) );
		self->PushReadTask();
	});
}

void UdpClient::Write( const DataPtr& data )
{
	Weak weak = GetWeak();
	sendSocket_.async_send_to( asio::buffer(*data ), targetEp_, [ data, weak ]( const error_code& ec, size_t size )
	{
		if( ec )
		{
			COIP_LOG_ERR( "UdpClient write failed: %s", ec.message().data() )
			return;
		}
		COIP_LOG_DEBUG( "UdpClient write %zu bytes", size )
	});
}

}
