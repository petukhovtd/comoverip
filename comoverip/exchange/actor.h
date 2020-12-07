#ifndef COMOVERIP_ACTOR_H
#define COMOVERIP_ACTOR_H

#include <memory>
#include <exchange/base_actor.h>

namespace comoverip
{

/// @brief Класс Actor реализующий enable_shared_from_this
/// @tparam A Тип класса
/// Надстройка над BaseActor, реализующий enable_shared_from_this принцип, необходимый для получения слабых ссылок
/// и передачи в другой поток исполнения
template< typename A >
class Actor: public std::enable_shared_from_this< A >, public BaseActor
{
public:
     using Ptr = std::shared_ptr< A >;
     using Weak = std::weak_ptr< A >;

     Actor() = default;

     Actor( const Actor& ) = delete;

     Actor( Actor&& ) = delete;

     Actor& operator=( const Actor& ) = delete;

     Actor& operator=( Actor&& ) = delete;

     /// @brief Создание экземпляра класса
     /// @param t параметры конструктора
     /// @return экземпляр класса
     template< typename... T >
     static Ptr Create( T... t )
     {
          return std::make_shared< A >( t... );
     }

     /// @brief Получение слабой ссылки на экземпляр класса
     /// @return слабая ссылка на экземпляр класса
     Weak GetWeak()
     {
          // TODO in g++ 17 change to weak_from_this()
          return Weak( std::enable_shared_from_this< A >::shared_from_this() );
     }

};

}

#endif
