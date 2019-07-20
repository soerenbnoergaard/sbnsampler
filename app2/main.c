#include <stdio.h>
#include "vco.h"

int main(void)
{
    if (vco_init() != STATUS_OK) {
        error("Error initializing VCO");
    };

    vco_close();
    return 0;
}
