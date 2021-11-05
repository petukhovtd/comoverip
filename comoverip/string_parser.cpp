#include <string_parser.h>

namespace comoverip
{
std::pair <std::string_view, std::optional< std::string_view>>
SplitBy( std::string_view s, std::string_view delimiter )
{
     size_t pos = s.find( delimiter );
     if( pos == std::string_view::npos )
     {
          return { s, std::nullopt };
     }

     return { s.substr( 0, pos ), s.substr( pos + delimiter.length() ) };
}

std::string_view ReadToken( std::string_view& s, std::string_view delimiter )
{
     const auto[lhs, rhs] = SplitBy( s, delimiter );
     s = rhs.value_or( "" );
     return lhs;
}

}
