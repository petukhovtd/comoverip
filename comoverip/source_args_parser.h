#ifndef COMOVERIP_SOURCE_ARGS_PARSER_H
#define COMOVERIP_SOURCE_ARGS_PARSER_H

#include <rtu_client.h>
#include <tcp_client.h>
#include <tcp_server.h>
#include <udp_client.h>

namespace comoverip
{

/// @brief Парсинг аргументов для RtuClient
/// @param[in] s
/// @return
RtuClient::Args ParsingRtuClientArgs( std::string_view s );

/// @brief Парсинг аргументов для TcpClient
/// @param[in] s
/// @return
TcpClient::Args ParsingTcpClientArgs( std::string_view s );

/// @brief Парсинг аргументов для TcpServer
/// @param[in] s
/// @return
TcpServer::Args ParsingTcpServerArgs( std::string_view s );

/// @brief Парсинг аргументов для UdpClient
/// @param[in] s
/// @return
UdpClient::Args ParsingUdpClientArgs( std::string_view s );

/// @brief Прочитать ip адрес и порт через разделитель :
/// @param[in] s
/// @return
std::pair< std::string_view, uint16_t > ReadIpAddrAndPort( std::string_view s );

/// @brief Прочитать ip порт
/// @param[in] s
/// @return
uint16_t ReadIpPort( std::string_view s );

/// @brief Парсинг аргументов источника заданного типа
/// @param[in] type
/// @param[in] args
/// @return
SourceArgsPtr MakeSourceArgs( const std::string_view& type, std::string_view args );

}

#endif //COMOVERIP_SOURCE_ARGS_PARSER_H
