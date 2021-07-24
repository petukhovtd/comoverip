#include <message_data.h>

namespace comoverip
{

MessageData::MessageData( DataPtr data )
          : data_( std::move( data ) )
{}

DataPtr MessageData::GetData() const
{
     return data_;
}

}