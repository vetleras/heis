/**
 * @file
 * @brief Callback-based wrapper for @c hardware.h. Also contains some global variables.
 */
#ifndef HWW_H
#define HWW_H

#include <stdbool.h>

/**
 * @brief Possible ocations for an order.
 */
typedef enum {
    hwwCab, hwwHall
} HwwLocation;
/**
 * @brief Possible directions.
 */
typedef enum {
    hwwUp, hwwDown
} HwwDirection;
/**
 * @brief Order type.
 */
typedef struct {
    /*@{*/
    int floor; /**< Zero-indexed destination floor of order */
    HwwLocation location; /**< Location of order */
    HwwDirection direction; /**< Direction of order, not used if order is placed from cab */
    /*@}*/
} HwwOrder;
/**
 * @brief Relative position to @c hww_floor.
 */
typedef enum {
    hwwAbove, hwwBelow
} HwwRelativePosition;
/**
 * @brief Whether or not door is obstructed.
 */
bool hww_obstructed;
/**
 * @brief Whether or not the elevator is at a floor.
 */
bool hww_at_floor;
/**
 * @brief Current floor. (Or last if elevator is between floors. See @c hww_relative_position)
 */
int hww_floor;
/**
 * @brief Whether the elevator is below or above @c hww_floor.
 */
HwwRelativePosition hww_relative_position;

/**
 * @brief Pointer to callback function to call in @c hww_update() if stop button has become pressed.
 */
void (*HWW_STOP_PRESSED_CB)();
/**
 * @brief Pointer to callback function to call in @c hww_update() if stop button has become released.
 */
void (*HWW_STOP_RELEASED_CB)();
/**
 * @brief Pointer to callback function to call in @c hww_update() if elevator has entered a floor.
 */
void (*HWW_ENTERED_FLOOR_CB)();
/**
 * @brief Pointer to callback function to call in @c hww_update() if a new order has been received.
 *  
 * @param[in] order Order recieved.
 */
void (*HWW_ORDER_RECEIVED_CB)(HwwOrder order);

/**
 * @brief Initializes hardware. Exits if hardware cannot be initialized.
 */
void hww_init();
/**
 * @brief Commands hardware to go in @p direction.
 * 
 * @param[in] direction Direction of movement.
 */
void hww_start(HwwDirection direction);
/**
 * @brief Commands hardware to stop.
 */
void hww_stop();
/**
 * @brief Turns on light associated with the given order.
 * 
 * @param[in] order Given order.
 */
void hww_order_light_on(HwwOrder order);
/**
 * @brief Turns off light associated with the gived order.
 * 
 * @param[in] order Given order.
 */
void hww_order_light_off(HwwOrder order);
/**
 * @brief Calls apropriate callbacks and updates global variables.
 */
void hww_update();
/**
 * @brief Opens door.
 */
void hww_open_door();
/**
 * @brief Closes door.
 */
void hww_close_door();

#endif