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
     /// @brief �������� ������ ��� ������
     /// @param actor
     /// @return id ������
     static uint64_t Insert( std::shared_ptr< BaseActor > actor );

     /// @brief ��������� ��������� ������
     /// @param id ������
     /// @param message ���������
     /// @return true ��������� ���������� ������
     /// @return false ����� � ����� id �� ������, ������ �� ����� ���������
     static bool Send( uint64_t id, std::shared_ptr< BaseMessage > message );

     /// @brief ������� ������ �� ������
     /// @param id ������
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
