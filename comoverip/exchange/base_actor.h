#ifndef COMOVERIP_BASEACTOR_H
#define COMOVERIP_BASEACTOR_H

#include <memory>
#include <exchange/base_message.h>

namespace comoverip
{

/// @brief Базовый интерфейс актора принимающего сообщения
class BaseActor
{
public:
     /// @brief Принять сообщение
     /// @param message
     virtual void Receive( const std::shared_ptr< BaseMessage >& message ) = 0;

     virtual ~BaseActor() = default;
};

}

#endif
