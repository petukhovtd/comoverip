#ifndef COMOVERIP_UDP_CLIENT_H
#define COMOVERIP_UDP_CLIENT_H

#include <i_source.h>
#include <common/data.h>
#include <asio.hpp>

namespace comoverip
{

/// @brief Клиент udp
class UdpClient: public ISource, public Actor< UdpClient >
{
public:
     struct Args: public ISource::Args
     {
          asio::ip::address addr;
          asio::ip::port_type port;

          Args( asio::ip::address a, asio::ip::port_type p )
          : ISource::Args( ISource::Type::UdpClient )
          , addr( std::move( a ) )
          , port( p )
          {}
     };

    /// @brief Конструктор класса. Открывает сокет приема и передачи
    /// @param[in] ioContext
    /// @param[in] args
    UdpClient( const std::shared_ptr< asio::io_context >& ioContext, const Args& args );

    ~UdpClient() override;

    void Receive( const std::shared_ptr< BaseMessage >& message) override;

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
    asio::ip::udp::endpoint targetEp_;
    asio::ip::udp::socket udpSocket_;
    std::atomic< bool > isStarted_;
};

}

#endif
