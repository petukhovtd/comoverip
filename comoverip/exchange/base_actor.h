#ifndef COMOVERIP_BASEACTOR_H
#define COMOVERIP_BASEACTOR_H

#include <exchange/base_message.h>

namespace comoverip
{

class BaseActor
{
public:
     /// @brief Получение сообщения
     /// @param message
     virtual void Receive( std::shared_ptr< BaseMessage > message ) = 0;

     virtual ~BaseActor() = default;
};

}

#endif
