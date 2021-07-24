#include <common/data.h>

namespace comoverip
{

DataPtr BuildDataBuffer()
{
     return std::make_shared< Data >( dataBufferSize );
}

}
