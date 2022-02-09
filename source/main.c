#include <stdio.h>

#include "hww.h"
#include "elevator.h"
#include "door.h"

void (*HWW_STOP_PRESSED_CB)() = &elevator_stop_pressed;
void (*HWW_STOP_RELEASED_CB)() = &elevator_stop_released;
void (*HWW_ENTERED_FLOOR_CB)() = &elevator_entered_floor;
void (*HWW_ORDER_RECEIVED_CB)(HwwOrder) = &elevator_order_received;

int main(){    
    hww_init();
    elevator_init();
    while (1){
        hww_update();
        door_timer_update();
    }    
}
