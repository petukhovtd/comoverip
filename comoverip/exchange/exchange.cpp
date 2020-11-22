#include <exchange/exchange.h>

namespace comoverip
{

uint64_t Exchange::Insert( std::shared_ptr< BaseActor > actor )
{
     return GetInstance().InsertImpl( std::move( actor ) );
}

bool Exchange::Send( uint64_t id, std::shared_ptr< BaseMessage > message )
{
     return GetInstance().SendImpl( id, std::move( message ) );
}

void Exchange::Remove( uint64_t id )
{
     return GetInstance().RemoveImpl( id );
}

uint64_t Exchange::GetNextId()
{
     static uint64_t id = 0;
     return ++id;
}

Exchange& Exchange::GetInstance()
{
     static std::unique_ptr< Exchange > instance( new Exchange() );
     return *instance;
}

uint64_t Exchange::InsertImpl( std::shared_ptr< BaseActor > actor )
{
     std::lock_guard< std::mutex > local( actorsMutex_ );
     uint64_t id = GetNextId();
     actors_[ id ] = actor;
     return id;
}

bool Exchange::SendImpl( uint64_t id, std::shared_ptr< BaseMessage > message )
{
     std::lock_guard< std::mutex > local( actorsMutex_ );
     auto it = actors_.find( id );
     if( it == actors_.end() )
     {
          return false;
     }
     std::shared_ptr< BaseActor > actor = it->second.lock();
     if( !actor )
     {
          return false;
     }
     actor->Receive( std::move( message ) );
     return true;
}

void Exchange::RemoveImpl( uint64_t id )
{
     std::lock_guard< std::mutex > local( actorsMutex_ );
     auto it = actors_.find( id );
     if( it == actors_.end() )
     {
          return;
     }
     actors_.erase( it );
}

}