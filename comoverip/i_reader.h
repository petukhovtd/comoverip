#ifndef COMOVERIP_I_READER_H
#define COMOVERIP_I_READER_H

#include <atomic>
#include <exchange/actor.h>
#include <exchange/exchange.h>

namespace comoverip
{

/// @brief Базовый класс чтения источника данных
template< typename T >
class IReader
          : public Actor< T >
{
public:
     IReader() = default;
     IReader( IReader& ) = delete;
     IReader( IReader&& ) = delete;

     virtual ~IReader() = default;

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

}

#endif
