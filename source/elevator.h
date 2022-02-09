/**
 * @file
 * @brief Module for handling elevator control and logic. Controls movement of elevator.
 */
#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "hww.h"

/**
 * @brief Initializes elevator. Sends elevator downwards if not currently at a floor.
 */
void elevator_init();
/**
 * @brief Function to call when elevator enters a floor.
 */
void elevator_entered_floor();
/**
 * @brief Function to call when stop button becomes pressed.
 */
void elevator_stop_pressed();
/**
 * @brief Function to call when stop button becomes released.
 */
void elevator_stop_released();
/**
 * @brief Function to call when a new order has been received.
 * 
 * @param[in] order New order.
 */
void elevator_order_received(HwwOrder order);
/**
 * @brief Function to call when door has become closed.
 */
void elevator_door_closed();

#endif