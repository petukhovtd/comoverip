#ifndef COMOVERIP_EXCHANGE_H
#define COMOVERIP_EXCHANGE_H

#include <memory>
#include <map>
#include <mutex>
#include <exchange/base_actor.h>
#include <exchange/base_message.h>

namespace comoverip
{

typedef uint64_t ActorId;

static const constexpr ActorId defaultId = 0;

class Exchange
{
public:
     /// @brief Добавить актора для обмена
     /// @param actor
     /// @return id актора
     static ActorId Insert( const std::shared_ptr< BaseActor >& actor );

     /// @brief Отправить сообщение актору
     /// @param id актора
     /// @param message сообщение
     /// @return true сообщение отправлено актору
     /// @return false актор с таким id не найден, ссылка на актор невалидна
     static bool Send( ActorId id, const std::shared_ptr< BaseMessage >& message );

     /// @brief Удалить атора из обмена
     /// @param id актора
     static void Remove( ActorId id );

private:
     Exchange() = default;

     static Exchange& GetInstance();

     static ActorId GetNextId();

     ActorId InsertImpl( const std::shared_ptr< BaseActor >& actor );

     bool SendImpl( ActorId id, const std::shared_ptr< BaseMessage >& message );

     void RemoveImpl( ActorId id );

private:
     std::mutex mutex_;
     std::map< ActorId, std::weak_ptr< BaseActor>> actors_;
};
}

#endif
