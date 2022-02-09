#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "hww.h"
#include "hardware.h"

static HwwOrder POTENTIAL_ORDERS[HARDWARE_NUMBER_OF_FLOORS*HARDWARE_NUMBER_OF_BUTTONS];
static HwwDirection direction;

bool hww_obstructed = false;
bool hww_at_floor = false;
int hww_floor = -1;
HwwRelativePosition hww_relative_position = hwwAbove;

static void populate_orders(){
    for (int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
        int j = i*HARDWARE_NUMBER_OF_BUTTONS;
        POTENTIAL_ORDERS[j].floor = i;
        POTENTIAL_ORDERS[j].location = hwwCab;
        POTENTIAL_ORDERS[j].direction = hwwUp;

        POTENTIAL_ORDERS[j+1].floor = i;
        POTENTIAL_ORDERS[j+1].location = hwwHall;
        POTENTIAL_ORDERS[j+1].direction = hwwUp;

        POTENTIAL_ORDERS[j+2].floor = i;
        POTENTIAL_ORDERS[j+2].location = hwwHall;
        POTENTIAL_ORDERS[j+2].direction = hwwDown;
    }    
}

void hww_init(){
    populate_orders();
    if (hardware_init() != 0) {
        printf("Failed to initialize hardware\n");
        exit(1);
    }
    // Find current floor
    for (int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
        if(hardware_read_floor_sensor(i)) {
            hww_floor = i;
        }    
    }
    if (hww_floor != -1) {
        hww_at_floor = true;
    }

}

static HardwareOrder hardware_order_from_hww_order(HwwOrder hww_order){
    if (hww_order.location == hwwCab) {
        return HARDWARE_ORDER_INSIDE;
    } else {
        if (hww_order.direction == hwwUp) {
            return HARDWARE_ORDER_UP;
        } else {
            return HARDWARE_ORDER_DOWN;
        }
    }
}

void hww_update(){
    static bool last_stop_state = false;
    bool current_stop_state = hardware_read_stop_signal();
    if (last_stop_state != current_stop_state){
        if (current_stop_state) {
            (*HWW_STOP_PRESSED_CB)();
            hardware_command_stop_light(1);
        } else {
            (*HWW_STOP_RELEASED_CB)();
            hardware_command_stop_light(0);
        }
        last_stop_state = current_stop_state;
    }

    hww_obstructed = hardware_read_obstruction_signal();

    static bool last_floor_sensor_states[HARDWARE_NUMBER_OF_FLOORS] = {false};
    for (int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
        bool current_floor_sensor_state = hardware_read_floor_sensor(i);
        if (last_floor_sensor_states[i] != current_floor_sensor_state) {
            if (current_floor_sensor_state) {
                hardware_command_floor_indicator_on(i);
                hww_floor = i;
                hww_at_floor = true;
                (*HWW_ENTERED_FLOOR_CB)();
            } else {
                hww_at_floor = false;
                if (direction == hwwUp) {
                    hww_relative_position = hwwAbove;
                } else {
                    hww_relative_position = hwwBelow;
                }
            }
            last_floor_sensor_states[i] = current_floor_sensor_state;
        }
    }

    static bool last_order_states[HARDWARE_NUMBER_OF_FLOORS*HARDWARE_NUMBER_OF_BUTTONS] = {false};
    for (int i = 0; i<HARDWARE_NUMBER_OF_FLOORS*HARDWARE_NUMBER_OF_BUTTONS; i++){
        HardwareOrder hardware_order = hardware_order_from_hww_order(POTENTIAL_ORDERS[i]);
        bool current_order_state = hardware_read_order(POTENTIAL_ORDERS[i].floor, hardware_order);
        if (last_order_states[i] != current_order_state){
            if (current_order_state) {
                (*HWW_ORDER_RECEIVED_CB)(POTENTIAL_ORDERS[i]);
            }
            last_order_states[i] = current_order_state;
        }
    }
}

void hww_start(HwwDirection _direction) {
    direction = _direction;
    if (direction == hwwUp) {
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
    } else {
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
}

void hww_stop(){
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
}

void hww_open_door(){
    hardware_command_door_open(1);
}

void hww_close_door(){
    hardware_command_door_open(0);
}

void hww_order_light_on(HwwOrder order){
    HardwareOrder hardware_order = hardware_order_from_hww_order(order);
    hardware_command_order_light(order.floor, hardware_order, true);
}

void hww_order_light_off(HwwOrder order){
    HardwareOrder hardware_order = hardware_order_from_hww_order(order);
    hardware_command_order_light(order.floor, hardware_order, false);
}
