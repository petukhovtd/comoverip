#ifndef COMOVERIP_UDP_CLIENT_H
#define COMOVERIP_UDP_CLIENT_H

#include <exchange/actor.h>
#include <exchange/exchange.h>
#include <common/data.h>
#include <asio.hpp>

namespace comoverip
{

/// @brief Клиент udp
class UdpClient: public Actor< UdpClient >
{
public:
    /// @brief Конструктор класса. Открывает сокет приема и передачи
    /// @param[in] ioContext
    /// @param[in] addr
    /// @param[in] port
    UdpClient( const std::shared_ptr< asio::io_context >& ioContext, const asio::ip::address& addr,
    	   asio::ip::port_type port );

    ~UdpClient() override = default;

    void Receive( const std::shared_ptr< BaseMessage >& message) override;

    /// @brief Установить id актора для отправки сообщений
    /// @param[in] id
    void SetDispatcherId( ActorId id );

    /// @brief Старт работы, начать принимать сообщения
    void Start();

    /// @brief Остановка работы, завершить все асинхронные задачи и закрыть сокеты
    void Stop();

private:
    /// @brief Создать асинхронную задачу на принятие сообщения
    void PushReadTask();

    /// @brief Отправить асинхронно данные
    /// @param[in] data
    void Write( const DataPtr& data );

private:
    asio::ip::udp::endpoint targetEp_;
    asio::ip::udp::socket receiveSocket_;
    asio::ip::udp::socket sendSocket_;
    ActorId dispatcherId_;
    bool isStarted_;
};

}

#endif
