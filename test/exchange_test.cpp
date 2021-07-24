#include <gtest/gtest.h>
#include <exchange/exchange.h>
#include <exchange/actor.h>
#include <exchange/message.h>

#include <utility>

using namespace comoverip;

class TestMessage
          : public Message< TestMessage >
{
public:
     explicit TestMessage( std::string message )
               : message_( std::move( message ) )
     {}

     const std::string& GetMessage() const
     {
          return message_;
     }

private:
     std::string message_;
};

class TestActor
          : public Actor< TestActor >
{
public:
     TestActor()
               : message_()
               , dispatcherId_( 0 )
     {}

     explicit TestActor( ActorId dispatcherId )
               : message_()
               , dispatcherId_( dispatcherId )
     {}

     void Receive( const std::shared_ptr< BaseMessage >& message ) override
     {
          auto testMessage = std::dynamic_pointer_cast< TestMessage >( message );
          if( !testMessage )
          {
               return;
          }
          message_ = testMessage->GetMessage();
     }

     const std::string& GetMessage() const
     {
          return message_;
     }

     bool SendToDispatcher( std::string const& message ) const
     {
          return Exchange::Send( dispatcherId_, TestMessage::Create( message ) );
     }

private:
     std::string message_;
     ActorId dispatcherId_;
};


TEST( ExchangeTest, Empty )
{
     std::string testMessage = "test";
     uint64_t testActorId = 0;
     EXPECT_FALSE( Exchange::Send( testActorId, TestMessage::Create( testMessage ) ) );
}

TEST( ExchangeTest, Receive )
{
     std::string testMessage = "test";
     TestActor::Ptr actor = TestActor::Create();
     uint64_t testActorId = Exchange::Insert( actor );
     EXPECT_TRUE( Exchange::Send( testActorId, TestMessage::Create( testMessage ) ) );
     EXPECT_EQ( testMessage, actor->GetMessage() );

     Exchange::Remove( testActorId );
     EXPECT_FALSE( Exchange::Send( testActorId, TestMessage::Create( testMessage ) ) );
}

TEST( ExchangeTest, SendFromActorToActor )
{
     const std::string testMessage = "test";

     TestActor::Ptr receiver = TestActor::Create();
     uint64_t receiverId = Exchange::Insert( receiver );

     TestActor::Ptr sender = TestActor::Create( receiverId );

     EXPECT_TRUE( sender->SendToDispatcher( testMessage ) );
     EXPECT_EQ( testMessage, receiver->GetMessage() );

     Exchange::Remove( receiverId );
     EXPECT_FALSE( sender->SendToDispatcher( testMessage ) );
}

