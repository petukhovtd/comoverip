#ifndef COMOVERIP_MESSAGE_DATA_H
#define COMOVERIP_MESSAGE_DATA_H

#include <exchange/message.h>
#include <common/data.h>

namespace comoverip
{

/// @brief Сообщение с буфером данных
class MessageData: public Message< MessageData >
{
public:
     /// @brief Конструктор класса
     /// @param data
     explicit MessageData( DataPtr data );

     /// @brief Получить буфер данных
     /// @return
     DataPtr GetData() const;

private:
     DataPtr data_;
};

}

#endif
