#ifndef COMOVERIP_CONFIGURATOR_H
#define COMOVERIP_CONFIGURATOR_H

#include <common/logger.h>
#include <i_source.h>

#include <vector>

namespace comoverip
{

/// @brief Конфигуратор
class Configurator
{
     /// @brief Количество источников
     enum Sources
     {
          Lhs,
          Rhs,
          SourcesSize
     };

public:
     /// @brief Конструктор класса
     /// @param[in] argc
     /// @param[in] argv
     Configurator( int argc, char* argv[] );

     /// @brief Проверяет количество источников, при ошибке кидает исключение
     void CheckSourcesSize() const;

     /// @brief Печать help-a
     /// @param[in,out] os
     static void PrintHelp( std::ostream& os );

     /// @brief Получить уровень логирования
     /// @return
     Logger::LogLevel GetLogLevel() const;

     /// @brief Источник
     /// @return
     SourceArgsPtr GetLhsArgs() const;

     /// @brief Источник
     /// @return
     SourceArgsPtr GetRhsArgs() const;

     /// @brief Необходимо печатать справку
     /// @return
     bool NeedPrintHelp() const;

private:
     /// @brief Парсинг аргументов
     /// @param[in] argc
     /// @param[in] argv
     void ParsingArgs( int argc, char* argv[] );

     /// @brief Чтение ключей и значений
     /// @param current текущий аргумент
     /// @param next следующий аргумент или пустая строка
     /// @return true если использован следующий аргумент
     bool ParsingOptions( std::string_view current, std::string_view next );

     /// @brief Парсинг источника
     /// @param[in] s
     void ParsingSource( std::string_view s );

private:
     Logger::LogLevel logLevel_;
     std::vector< SourceArgsPtr > sourceArgs_;
     bool printHelp_;
};

}

#endif //COMOVERIP_CONFIGURATOR_H
