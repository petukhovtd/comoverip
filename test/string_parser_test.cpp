#include <gtest/gtest.h>
#include <string_parser.h>

using namespace comoverip;

TEST( StringParserTest, SplitTest )
{
     {
          std::string_view s = { "abc,def" };
          const auto[ first, second ] = SplitBy( s, "," );
          EXPECT_EQ( first, "abc" );
          EXPECT_EQ( second, "def" );
     }
     {
          std::string_view s = { "abc!!def" };
          const auto[ first, second ] = SplitBy( s, "!!" );
          EXPECT_EQ( first, "abc" );
          EXPECT_EQ( second, "def" );
     }
     {
          std::string_view s = { "abc," };
          const auto[ first, second ] = SplitBy( s, "," );
          EXPECT_EQ( first, "abc" );
          EXPECT_EQ( second, "" );
     }
     {
          std::string_view s = { ",def" };
          const auto[ first, second ] = SplitBy( s, "," );
          EXPECT_EQ( first, "" );
          EXPECT_EQ( second, "def" );
     }
     {
          std::string_view s = { "abc" };
          const auto[ first, second ] = SplitBy( s, "," );
          EXPECT_EQ( first, "abc" );
          EXPECT_FALSE( second.has_value() );
     }
     {
          std::string_view s = { "" };
          const auto[ first, second ] = SplitBy( s, "," );
          EXPECT_EQ( first, "" );
          EXPECT_FALSE( second.has_value() );
     }
     {
          std::string_view s = { "abc" };
          const auto[ first, second ] = SplitBy( s, "" );
          EXPECT_EQ( first, "" );
          EXPECT_EQ( second, "abc" );
     }
}

TEST( StringParserTest, ReadToken )
{
     {
          std::string_view s { "a,b,c" };
          EXPECT_EQ( "a", ReadToken( s, ",") );
          EXPECT_EQ( "b", ReadToken( s, ",") );
          EXPECT_EQ( "c", ReadToken( s, ",") );
          EXPECT_EQ( "", ReadToken( s, ",") );
          EXPECT_TRUE( s.empty() );
     }
     {
          std::string_view s { "a," };
          EXPECT_EQ( "a", ReadToken( s, ",") );
          EXPECT_EQ( "", ReadToken( s, ",") );
          EXPECT_TRUE( s.empty() );
     }
     {
          std::string_view s { ",a" };
          EXPECT_EQ( "", ReadToken( s, ",") );
          EXPECT_EQ( "a", ReadToken( s, ",") );
          EXPECT_TRUE( s.empty() );
     }
}