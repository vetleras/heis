#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "door.h"
#include "hww.h"
#include "elevator.h"

#define log_unexpected_event() printf("ERROR: %s is not handled in state %d\n", __func__, state)

int TIMEOUT = 3000;

typedef enum{
    closed,
    open,
    seizedOpen
} State;

static State state = closed;
static unsigned long long time = 0;

static unsigned long long now(){
    static struct timeval time;
    gettimeofday(&time, 0);
    return time.tv_sec * 1000 + time.tv_usec / 1000;
}

static void restart_timer() {
    time = now();
}

static bool timed_out() {
    return now() - TIMEOUT >= time;
}

void door_open(){
    switch(state) {
        case closed:
            hww_open_door();
            restart_timer();
            state = open;
            break;
        
        case open:
            restart_timer();
            break;
        
        case seizedOpen:
            break;            
        
        default:
            log_unexpected_event();
            exit(1);
    }
}

void door_seize_open(){
    switch (state){
        case closed:
            hww_open_door();
            state = seizedOpen;
            break;
        
        case open:
            state = seizedOpen;
            break;
        
        case seizedOpen:
            break;
        
        default:
            log_unexpected_event();
            exit(1);
    } 
}

void door_unseize() {
    switch (state) {
        case closed:
            break;
        
        case open:
            break;
        
        case seizedOpen:
            restart_timer();
            state = open;
            break;
        
        default:
            log_unexpected_event();
            exit(1);
    }
}

void door_timer_update() {
    switch (state) {
        case closed:
            break;
        
        case open:            
            if (timed_out() && !hww_obstructed) {
                hww_close_door();
                elevator_door_closed();
                state = closed;
            }
            break;
        
        case seizedOpen:
            break;
        
        default:
            log_unexpected_event();
            exit(1);
    }
}

bool door_is_open() {
    return state != closed;
}