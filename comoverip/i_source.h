#ifndef COMOVERIP_I_SOURCE_H
#define COMOVERIP_I_SOURCE_H

#include <atomic>
#include <exchange/actor.h>
#include <exchange/exchange.h>

namespace comoverip
{

/// @brief Базовый класс чтения источника данных
class ISource: public BaseActor
{
public:
     /// @brief Типы источников
     enum class Type
     {
          RtuClient,
          TcpClient,
          TcpServer,
          UdpClient
     };

     /// @brief Аргументы источника
     struct Args
     {
          virtual ~Args() = default;

          explicit Args( Type t )
          : type_( t )
          {}

          Type GetType() const
          {
               return type_;
          }

     protected:
          Type type_;
     };

     ISource() = default;
     ISource( ISource& ) = delete;
     ISource( ISource&& ) = delete;

     virtual ~ISource() = default;

     /// @brief Установить id для второй стороны обмена
     /// @param id
     virtual void SetWriterId( ActorId id )
     {
          writerId_ = id;
     }

     /// @brief Старт работы, начать чтение из порта
     virtual void Start() = 0;

     /// @brief Остановка работы, завершить все асинхронные задачи и закрыть порт
     virtual void Stop() = 0;

protected:
     std::atomic< ActorId > writerId_ = defaultId;
};

using SourcePtr = std::shared_ptr< ISource >;
using SourceArgsPtr = std::shared_ptr< ISource::Args >;

}

#endif
