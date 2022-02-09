/**
 * @file
 * @brief Module for handling door timing and logic. Handles obstruction, and opens and closes door.
 */
#ifndef DOOR_H
#define DOOR_H

/**
 *@brief Opens door. Closes door when not obstructed and timed out. Calls @c elevator_door_closed() on closing.
 */
void door_open();
/**
 * @brief Opens door. Will not close until @c door_unseize() is called.
 */
void door_seize_open();
/**
 * @brief Closes door when not obstructed and timed out. Calls @c elevator_door_closed() on closing.
 */
void door_unseize();
/**
 * @brief Checks if door is timed out. Calls @c elevator_door_closed() on closing.
 */
void door_timer_update();
/**
 * @brief Returns wheter door is open.
 */
bool door_is_open();

#endif