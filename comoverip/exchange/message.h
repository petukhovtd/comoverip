#ifndef COMOVERIP_MESSAGE_H
#define COMOVERIP_MESSAGE_H

#include <exchange/base_message.h>

namespace comoverip
{

/// @brief ¬спомогательный класс дл€ создани€ указател€ на Message
/// @tparam M
template< typename M >
class Message: public BaseMessage
{
public:
     using Ptr = std::shared_ptr< M >;

     Message() = default;

     Message( const Message& ) = delete;

     Message( Message&& ) = delete;

     Message& operator=( const Message& ) = delete;

     Message& operator=( Message&& ) = delete;

     /// @brief —оздание экземпл€ра класса
     /// @param t параметры конструктора
     /// @return экземпл€р класса
     template< typename... T >
     static Ptr Create( T... t )
     {
          return std::make_shared< M >( t... );
     }
};

}

#endif
