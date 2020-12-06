#include <gtest/gtest.h>
#include <exchange/exchange.h>
#include <exchange/base_message.h>
#include <exchange/actor.h>

using namespace comoverip;

class TestMessage: public BaseMessage
{
public:
     explicit TestMessage( const std::string& message )
               : message_( message )
     {}

     const std::string& GetMessage() const
     {
          return message_;
     }

private:
     std::string message_;
};


class TestActor: public Actor< TestActor >
{
     friend class Actor< TestActor >;
public:
     void Receive( std::shared_ptr< BaseMessage > message ) override
     {
          std::shared_ptr< TestMessage > testMessage = std::dynamic_pointer_cast< TestMessage >( message );
          if( testMessage )
          {
               message_ = testMessage->GetMessage();
          }
     }

     const std::string& GetMessage() const
     {
          return message_;
     }

private:
     TestActor() = default;

private:
     std::string message_;
};


TEST(ExchangeTest, EmptyTest)
{
     std::string testMessage = "test";
     uint64_t testActorId = 0;
     EXPECT_FALSE( Exchange::Send( testActorId, std::make_shared< TestMessage >( testMessage )));
     TestActor::Ptr ta = TestActor::Create();
//     std::shared_ptr< TestActor > ta2 = std::make_shared< TestActor >();
//     TestActor ta3;
     testActorId = Exchange::Insert( ta );
}

