#ifndef COMOVERIP_RTU_CLIENT_H
#define COMOVERIP_RTU_CLIENT_H

#include <string>
#include <asio.hpp>
#include <exchange/actor.h>
#include <exchange/exchange.h>
#include <exchange/message.h>
#include <common/data.h>

namespace comoverip
{

/// @brief Клиент последовательного порта
class RtuClient
          : public Actor< RtuClient >
{
public:
     /// @brief Конструктор класса. Открывает последовательный порт
     /// @param[in] ioContext
     /// @param[in] portName
     /// @param[in] baudRate
     /// @param[in] characterSize
     /// @param[in] stopBits
     /// @param[in] parity
     RtuClient( const std::shared_ptr< asio::io_context >& ioContext,
                std::string portName,
                asio::serial_port::baud_rate baudRate,
                asio::serial_port::character_size characterSize = asio::serial_port::character_size(),
                asio::serial_port::stop_bits stopBits = asio::serial_port::stop_bits(),
                asio::serial_port::parity parity = asio::serial_port::parity() );

     ~RtuClient() override;

     void Receive( const std::shared_ptr< BaseMessage >& message ) override;

     /// @brief Установить id актора для отправки сообщений
     /// @param[in] id
     void SetDispatcherId( ActorId id );

     /// @brief Старт работы, начать чтение из порта
     void Start();

     /// @brief Остановка работы, завершить все асинхронные задачи и закрыть порт
     void Stop();

private:
     /// @brief Создать асинхронную задачу на чтение
     void PushReadTask();

     /// @brief Записать асинхронно данные в порт
     /// @param[in] data
     void Write( const DataPtr& data );

private:
     std::string portName_;
     asio::serial_port serialPort_;
     ActorId dispatcherId_;
     bool isStarted_;
};

}

#endif
