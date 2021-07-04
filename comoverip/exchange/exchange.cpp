#include <exchange/exchange.h>

namespace comoverip
{

ActorId Exchange::Insert( const std::shared_ptr< BaseActor >& actor )
{
     return GetInstance().InsertImpl( actor );
}

bool Exchange::Send( ActorId id, const std::shared_ptr< BaseMessage >& message )
{
     return GetInstance().SendImpl( id, message );
}

void Exchange::Remove( ActorId id )
{
     return GetInstance().RemoveImpl( id );
}

ActorId Exchange::GetNextId()
{
     static ActorId id = 0;
     return ++id;
}

Exchange& Exchange::GetInstance()
{
     static Exchange instance;
     return instance;
}

ActorId Exchange::InsertImpl( const std::shared_ptr< BaseActor >& actor )
{
     std::lock_guard< std::mutex > local( mutex_ );
     ActorId id = GetNextId();
     actors_[ id ] = actor;
     return id;
}

bool Exchange::SendImpl( ActorId id, const std::shared_ptr< BaseMessage >& message )
{
     std::lock_guard< std::mutex > lock( mutex_ );
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
     actor->Receive( message );
     return true;
}

void Exchange::RemoveImpl( ActorId id )
{
     std::lock_guard< std::mutex > lock( mutex_ );
     auto it = actors_.find( id );
     if( it == actors_.end() )
     {
          return;
     }
     actors_.erase( it );
}

}