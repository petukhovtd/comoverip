#ifndef COMOVERIP_UDP_CLIENT_H
#define COMOVERIP_UDP_CLIENT_H

#include <i_reader.h>
#include <common/data.h>
#include <asio.hpp>

namespace comoverip
{

/// @brief Клиент udp
class UdpClient: public IReader< UdpClient >
{
public:
    /// @brief Конструктор класса. Открывает сокет приема и передачи
    /// @param[in] ioContext
    /// @param[in] addr
    /// @param[in] port
    UdpClient( const std::shared_ptr< asio::io_context >& ioContext, const asio::ip::address& addr,
    	   asio::ip::port_type port );

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
