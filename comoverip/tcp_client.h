#ifndef COMOVERIP_TCP_CLIENT_H
#define COMOVERIP_TCP_CLIENT_H

#include <i_reader.h>
#include <common/data.h>
#include <asio.hpp>

namespace comoverip
{

/// @brief Tcp клиент
class TcpClient: public IReader< TcpClient >
{
public:
     /// @brief Подключение к серверу
     /// @param[in] ioContext
     /// @param[in] addr
     /// @param[in] port
     TcpClient( const std::shared_ptr< asio::io_context >& ioContext, const asio::ip::address& addr,
                asio::ip::port_type port );

     ~TcpClient() override;

     void Receive( const std::shared_ptr< BaseMessage >& message ) override;

     /// @brief Старт работы, начать принимать сообщения
     void Start() override;

     /// @brief Остановка работы, завершить все асинхронные задачи и закрыть сокеты
     void Stop() override;

private:
     /// @brief Создать асинхронную задачу на принятие сообщения
     void PushReadTask();

     /// @brief Отправить асинхронно данные
     /// @param[in] data
     void Write( const DataPtr& data );

     /// @brief Освобождение ресурсов
     void StopImpl();

private:
     asio::ip::tcp::socket tcpSocket_;
     std::atomic< bool > isStarted_;
};

}

#endif
