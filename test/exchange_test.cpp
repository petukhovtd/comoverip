#include <gtest/gtest.h>
#include <exchange/exchange.h>
#include <exchange/actor.h>
#include <exchange/message.h>

using namespace comoverip;

class TestMessage: public Message< TestMessage >
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
public:
     TestActor()
     : message_()
     , distatcherId_( 0 )
     {}

     explicit TestActor( uint64_t dispatcherId )
     : message_()
     , distatcherId_( dispatcherId )
     {}

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

     bool SendToDispatcher( std::string const& message ) const
     {
          return Exchange::Send( distatcherId_, std::make_shared< TestMessage >( message ));
     }

private:
     std::string message_;
     uint64_t distatcherId_;
};


TEST( ExchangeTest, EmptyTest )
{
     std::string testMessage = "test";
     uint64_t testActorId = 0;
     EXPECT_FALSE( Exchange::Send( testActorId, TestMessage::Create( testMessage ) ) );
}

TEST( ExchangeTest, ReceiveTest )
{
     std::string testMessage = "test";
     TestActor::Ptr actor = TestActor::Create();
     uint64_t testActorId = Exchange::Insert( actor );
     EXPECT_TRUE( Exchange::Send( testActorId, TestMessage::Create( testMessage ) ) );
     EXPECT_EQ( testMessage, actor->GetMessage() );

     Exchange::Remove( testActorId );
     EXPECT_FALSE( Exchange::Send( testActorId, TestMessage::Create( testMessage ) ) );
}

TEST( ExchangeTest, SendFromActorTest )
{
     std::string testMessage = "test";

     TestActor::Ptr receiver = TestActor::Create();
     uint64_t receiverId = Exchange::Insert( receiver );

     TestActor::Ptr sender = TestActor::Create( receiverId );

     EXPECT_TRUE( sender->SendToDispatcher( testMessage ) );
     EXPECT_EQ( testMessage, receiver->GetMessage() );

     Exchange::Remove( receiverId );
     EXPECT_FALSE( sender->SendToDispatcher( testMessage ) );
}

