#ifndef COMOVERIP_EXCHANGE_H
#define COMOVERIP_EXCHANGE_H

#include <memory>
#include <map>
#include <mutex>
#include <exchange/base_actor.h>
#include <exchange/base_message.h>

namespace comoverip
{

class Exchange
{
public:
     /// @brief Добавить актора для обмена
     /// @param actor
     /// @return id актора
     static uint64_t Insert( std::shared_ptr< BaseActor > actor );

     /// @brief Отправить сообщение актору
     /// @param id актора
     /// @param message сообщение
     /// @return true сообщение отправлено актору
     /// @return false актор с таким id не найден, ссылка на актор невалидна
     static bool Send( uint64_t id, std::shared_ptr< BaseMessage > message );

     /// @brief Удалить катора из обмена
     /// @param id актора
     static void Remove( uint64_t id );

private:
     Exchange() = default;

     static Exchange& GetInstance();

     static uint64_t GetNextId();

     uint64_t InsertImpl( std::shared_ptr< BaseActor > actor );

     bool SendImpl( uint64_t id, std::shared_ptr< BaseMessage > message );

     void RemoveImpl( uint64_t id );

private:
     std::mutex actorsMutex_;
     std::map< uint64_t, std::weak_ptr< BaseActor>> actors_;
};
}

#endif
