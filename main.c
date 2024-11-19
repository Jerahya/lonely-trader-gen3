#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "includes/trade.h"
#include "includes/worker.h"


int main() {
    stdio_init_all();
    
    sleep_ms(3000);
    printf("start\n");
    
    multicore_launch_core1(wrk_main);

    gLink.Init();
    gLink.Run();
    
    stdio_flush();
}