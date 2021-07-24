#ifndef COMOVERIP_DATA_H
#define COMOVERIP_DATA_H

#include <vector>
#include <memory>

namespace comoverip
{

using Data = std::vector< uint8_t >;
using DataPtr = std::shared_ptr< Data >;
static const constexpr size_t dataBufferSize = 0xFFFF;

/// @brief Получить буфер данных с размером dataBufferSize
/// @return
DataPtr BuildDataBuffer();

}

#endif
