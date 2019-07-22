#include "vca.h"

int16_t vca(int16_t x, uint8_t amplitude)
{
    return (x * amplitude)>>7;
}
