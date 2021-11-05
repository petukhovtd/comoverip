#ifndef COMOVERIP_STRING_PARSER_H
#define COMOVERIP_STRING_PARSER_H

#include <string_view>
#include <optional>

namespace comoverip
{

/// @brief Разделить строку по разделителю
/// @param s исходная строка
/// @param delimiter разделитель
/// @return { строка до разделителя, строка после разделителя }
/// @return { исходная строка, nullopt } если разделитель не найден
std::pair< std::string_view, std::optional< std::string_view>>
SplitBy( std::string_view s, std::string_view delimiter );

/// @brief Получить соедующий токен
/// @param s строка для чтения
/// @param delimiter разделитель
/// @return токен или пустая строка
std::string_view ReadToken( std::string_view& s, std::string_view delimiter );

}

#endif //COMOVERIP_STRING_PARSER_H
