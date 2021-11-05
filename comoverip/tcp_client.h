#ifndef COMOVERIP_TCP_CLIENT_H
#define COMOVERIP_TCP_CLIENT_H

#include <i_source.h>
#include <common/data.h>
#include <asio.hpp>
#include <utility>

namespace comoverip
{

/// @brief Tcp клиент
class TcpClient
          : public ISource
            , public Actor< TcpClient >
{
public:
     struct Args: public ISource::Args
     {
          asio::ip::address addr;
          asio::ip::port_type port;

          Args( asio::ip::address address, asio::ip::port_type p )
                    : ISource::Args( ISource::Type::TcpClient )
                    , addr( std::move( address ) )
                    , port( p )
          {}
     };

     /// @brief Подключение к серверу
     /// @param[in] ioContext
     /// @param[in] args
     TcpClient( const std::shared_ptr< asio::io_context >& ioContext, const Args& args );

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
