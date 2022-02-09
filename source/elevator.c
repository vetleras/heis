#include <stdio.h>
#include <stdlib.h>

#include "elevator.h"
#include "hww.h"
#include "door.h"
#include "rp.h"

#define log_unexpected_event() printf("ERROR: %s is not handled in state %d\n", __func__, state)

typedef enum {
    undefined,
    idle,
    moving,
    stopped,
    doorOpen
} State;

static State state;
static HwwDirection direction;

void elevator_init(){
    if (!hww_at_floor) {
        hww_start(hwwDown);
    }     
    state = undefined;    
}

void elevator_entered_floor(){
    switch (state) {
        case undefined:
            hww_stop();
            state = idle;
            break;

        case moving:
            if (rp_should_stop(direction)){
                hww_stop();
                door_open();
                rp_floor_served();
                state = doorOpen;
            }
            break;
            
        default:
            log_unexpected_event();
            exit(1);        
    }
}

void elevator_stop_pressed() {
    switch(state) {
        case undefined:
            break;

        case idle:
            if (hww_at_floor) {
                door_seize_open();
            }
            state = stopped;
            break;

        case moving:
            hww_stop();
            if (hww_at_floor) {
                door_seize_open();
            }
            rp_flush();
            state = stopped;
            break;
        
        case doorOpen:
            door_seize_open();
            rp_flush();
            state = stopped;
            break;
        
        default:
            log_unexpected_event();
            exit(1);
    }
}

void elevator_stop_released(){
    switch (state){
        case undefined:
            break;

        case stopped:
            if (door_is_open()) {
                door_unseize();
                state = doorOpen;
            } else {
                state = idle;
            }
            break;
        
        default:
            log_unexpected_event();
            exit(1);
    }
}

void elevator_door_closed() {
    switch(state) {
        case doorOpen:
            if (rp_should_move()) {
                direction = rp_get_direction(direction);
                hww_start(direction);
                state = moving;
            } else {
                state = idle;
            }            
            break;

        default:
            log_unexpected_event();
            exit(1);         
    }
}    

void elevator_order_received(HwwOrder order) {
    switch (state){
        case undefined:
            break;

        case idle:            
            if (hww_at_floor && hww_floor == order.floor) {
                door_open();
                state = doorOpen;
            } else {
                rp_add_order(order);
                direction = rp_get_direction(direction);
                hww_start(direction);
                state = moving;
            }            
            break;
        
        case moving:
            rp_add_order(order);
            break;
        
        case stopped:
            break;
        
        case doorOpen:
            if (hww_at_floor && hww_floor == order.floor) {
                door_open();
            } else {
                rp_add_order(order);
            }
            break;
        
        default:
            log_unexpected_event();
            exit(1);
    }
}