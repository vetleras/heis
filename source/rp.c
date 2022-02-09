#include <stdio.h>

#include "rp.h"
#include "hww.h"
#include "hardware.h"


static HwwOrder orders[HARDWARE_NUMBER_OF_BUTTONS*HARDWARE_NUMBER_OF_FLOORS];
static int end_index = -1;

static bool order_is_new(HwwOrder order){
    for (int i = 0; i <= end_index; i++) {
        if (order.floor == orders[i].floor && order.location == orders[i].location && order.direction == orders[i].direction){
            return false;
        }
    }
    return true;
}

static void append(HwwOrder order){
    orders[++end_index] = order;
    hww_order_light_on(order);
}

void rp_add_order(HwwOrder order) {
    if (order_is_new(order)){
        append(order);
    }
}

static void pop(int index){
    const HwwOrder order = orders[index];
    for (int i = index; i<=end_index; i++) {
        orders[i] = orders[i+1];
    }
    end_index--;
    hww_order_light_off(order);
}

void rp_floor_served() {
    for (int i = end_index; i >= 0; i--) {
        if (orders[i].floor == hww_floor){
            pop(i);
        }
    }
}

void rp_flush() {
    for (int i = end_index; i >= 0; i--) {
        pop(i);
    }
}

static bool pending_orders() {
    return end_index >= 0;
}

bool rp_should_move() {
    return pending_orders();

}

static bool cab_order_in_queue(int floor) {
    for (int i = 0; i<=end_index; i++) {
        if (orders[i].floor == floor && orders[i].location == hwwCab){
            return true;
        }
    }
    return false;
}

static bool hall_order_in_queue(int floor, HwwDirection direction) {
    for (int i = 0; i<=end_index; i++) {
        if (orders[i].floor == floor && orders[i].direction == direction){
            return true;
        }
    }
    return false;
}

static bool last_floor_with_order_in_given_direction(int floor, HwwDirection direction) {
    if (direction == hwwUp) {
        for (int i = 0; i<=end_index; i++) {
            HwwOrder order = orders[i];
            if (order.floor > floor) {
                return false;
            }
        }
    } else {
        for (int i = 0; i<=end_index; i++) {
            HwwOrder order = orders[i];
            if (order.floor < floor) {
                return false;
            }
        }
    }
    return true;
}

bool rp_should_stop(HwwDirection direction) {
    if (hww_at_floor) {
        if (cab_order_in_queue(hww_floor) | hall_order_in_queue(hww_floor, direction)) {
            return true;
        } else if (last_floor_with_order_in_given_direction(hww_floor, direction)) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}

HwwDirection rp_get_direction(HwwDirection direction) {
    if (hww_at_floor) {
        if (direction == hwwUp) {
            for (int i = 0; i<=end_index; i++) {
                HwwOrder order = orders[i];
                if (order.floor > hww_floor) {
                    return hwwUp;
                }
            }
            return hwwDown;
        } else {
            for (int i = 0; i<=end_index; i++) {
                HwwOrder order = orders[i];
                if (order.floor < hww_floor) {
                    return hwwDown;
                }
            }
            return hwwUp;
        }
    } else {
        if (hww_relative_position == hwwAbove) {
            printf("Hell4o\n");
            if (direction == hwwUp) {
                for (int i = 0; i<=end_index; i++) {
                    HwwOrder order = orders[i];
                    if (order.floor > hww_floor) {
                        return hwwUp;
                    }
                }
                return hwwDown;
            } else {
                printf("H3ello\n");
                for (int i = 0; i<=end_index; i++) {
                    HwwOrder order = orders[i];
                    if (order.floor <= hww_floor) {
                        return hwwDown;
                    }
                }
                return hwwUp;
            }
        } else {
            printf("Hello2\n");
            if (direction == hwwUp) {
                for (int i = 0; i<=end_index; i++) {
                    HwwOrder order = orders[i];
                    if (order.floor >= hww_floor) {
                        return hwwUp;
                    }
                }
                return hwwDown;
            } else {
                printf("Hello\n");
                for (int i = 0; i<=end_index; i++) {
                    HwwOrder order = orders[i];
                    if (order.floor < hww_floor) {
                        return hwwDown;
                    }
                }
                return hwwUp;
            }
        }
    }
}