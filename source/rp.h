/**
 * @file
 * @brief Route planner (rp) module. Handles order, order lights and gives elevator directions.
 */
#ifndef RP_H
#define RP_H

#include "hww.h"

/**
 * @brief Decides whether the elevator should stop.
 * 
 * @param[in] direction Direction of movement.
 * 
 * @returns @c true if an order exists at the current floor in the given direction or if the current floor is the last floor with
 * an order in the given the direction, @c false otherwise.
 */
bool rp_should_stop(HwwDirection direction);
/**
 * @brief Removes all orders associated with the current floor. Turns off associated order lights.
 */
void rp_floor_served();
/**
 * @brief Remove all orders. Turns off associated order lights.
 */
void rp_flush();
/**
 * @brief Decides whether the elevator should start moving.
 * 
 * @returns @c true if there are pedning orders, false @c otherwise.
 */
bool rp_should_move();
/**
 * @brief Decides which direction the elevator should move in.
 * 
 * @param[in] direction Direction of movement.
 * 
 * @returns @p direction if there are more orders in that direction. Returns opposite otherwise.
 */
HwwDirection rp_get_direction(HwwDirection direction);
/**
 * @brief Add order to route planner. Turns on order light.
 */
void rp_add_order();


#endif