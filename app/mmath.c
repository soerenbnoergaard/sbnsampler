#include "mmath.h"

uint8_t mmath_add_and_saturate(uint8_t x, uint8_t y)
{
    return (x+y>127) ? 127 : x+y;
}


uint8_t mmath_multiply(uint8_t x, uint8_t y)
{
    uint16_t acc = (uint16_t)x * (uint16_t)y;
    return acc>>7;
}
