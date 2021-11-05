#ifndef COMOVERIP_RTU_CLIENT_H
#define COMOVERIP_RTU_CLIENT_H

#include <string>
#include <asio.hpp>
#include <i_source.h>
#include <exchange/message.h>
#include <common/data.h>

namespace comoverip
{

/// @brief Клиент последовательного порта
class RtuClient
          : public ISource, public Actor< RtuClient >
{
public:
     struct Args: ISource::Args
     {
          std::string portName;
          asio::serial_port::baud_rate baudRate;
          asio::serial_port::character_size characterSize = asio::serial_port::character_size();
          asio::serial_port::stop_bits stopBits = asio::serial_port::stop_bits();
          asio::serial_port::parity parity = asio::serial_port::parity();

          Args( std::string port, asio::serial_port::baud_rate baud )
          : ISource::Args( ISource::Type::RtuClient )
          , portName( std::move( port ) )
          , baudRate( baud )
          {}
     };

     /// @brief Конструктор класса. Открывает последовательный порт
     /// @param[in] ioContext
     /// @param[in] args
     RtuClient( const std::shared_ptr< asio::io_context >& ioContext, const Args& args );

     ~RtuClient() override;

     void Receive( const std::shared_ptr< BaseMessage >& message ) override;

     /// @brief Старт работы, начать чтение из порта
     void Start() override;

     /// @brief Остановка работы, завершить все асинхронные задачи и закрыть порт
     void Stop() override;

private:
     /// @brief Создать асинхронную задачу на чтение
     void PushReadTask();

     /// @brief Записать синхронно данные в порт
     /// @param[in] data
     void Write( const DataPtr& data );

     /// @brief Реализация остановки работы
     void StopImpl();

private:
     std::string portName_;
     asio::serial_port serialPort_;
     std::atomic< bool > isStarted_;
};

}

#endif
