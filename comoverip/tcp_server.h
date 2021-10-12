#ifndef COMOVERIP_TCP_SERVER_H
#define COMOVERIP_TCP_SERVER_H

#include <i_reader.h>
#include <common/data.h>
#include <asio.hpp>

namespace comoverip
{

/// @brief Tcp сервер
class TcpServer
          : public IReader< TcpServer >
{
public:
     /// @brief Конструктор класса
     /// @param[in,out] ioContext
     /// @param[in] port
     TcpServer( const std::shared_ptr< asio::io_context >& ioContext, asio::ip::port_type port );

     void Receive( const std::shared_ptr< BaseMessage >& message ) override;

     ~TcpServer() override;

     /// @brief Запуск сервера
     void Start() override;

     /// @brief Остановка сервера
     void Stop() override;

private:
     /// @brief Старт асинхронной задачи приема подключения
     void PushAcceptTask();

     /// @brief Старт асинхронного чтения
     void PushReadTask();

     /// @brief Запись данных текущему клиенту
     /// @param[in] data
     void Write( const DataPtr& data );

     /// @brief Реализация остановки сервера
     void StopImpl();

public:
     asio::ip::tcp::acceptor acceptor_;
     asio::ip::tcp::socket socket_;
     std::atomic< bool > isStarted_;
};

}

#endif //COMOVERIP_TCP_SERVER_H
