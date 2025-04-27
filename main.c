/**
 * @file main.c
 * @brief Main file.
 * @author Sistemas Digitales II
 * @date 2025-01-01
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C libraries */
#include <stdio.h> // printf
#include <stdlib.h>
#include <stdint.h>

/* HW libraries */
#include "port_system.h"
#include "port_button.h"
#include "port_ultrasound.h"
#include "port_display.h"
#include "fsm.h"
#include "fsm_button.h"
#include "fsm_ultrasound.h"
#include "fsm_display.h"
#include "fsm_urbanite.h"

/* Defines ------------------------------------------------------------------*/
#define URBANITE_ON_OFF_PRESS_TIME_MS 1000 /*!< Time in milliseconds to toggle the system on/off */
#define URBANITE_PAUSE_DISPLAY_TIME_MS 500 /*!< Time in milliseconds to pause/resume the display */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* Init board */
    port_system_init();
    fsm_button_t *p_fsm_button = fsm_button_new(PORT_PARKING_BUTTON_DEBOUNCE_TIME_MS, PORT_PARKING_BUTTON_ID);
    fsm_ultrasound_t *p_fsm_ultrasound_rear = fsm_ultrasound_new(PORT_REAR_PARKING_SENSOR_ID);
    fsm_display_t *p_fsm_display_rear = fsm_display_new(PORT_REAR_PARKING_DISPLAY_ID);
    fsm_urbanite_t *p_fsm_urbanite = fsm_urbanite_new(p_fsm_button, URBANITE_ON_OFF_PRESS_TIME_MS, URBANITE_PAUSE_DISPLAY_TIME_MS, p_fsm_ultrasound_rear, p_fsm_display_rear);

    /* Infinite loop */
    while (1)
    {
        fsm_button_fire(p_fsm_button); // Check button state and fire FSM
        fsm_ultrasound_fire(p_fsm_ultrasound_rear); // Check ultrasound state and fire FSM
        fsm_display_fire(p_fsm_display_rear); // Check display state and fire FSM
        fsm_urbanite_fire(p_fsm_urbanite); // Check urbanite state and fire FSM
    } // End of while(1)

    fsm_urbanite_destroy(p_fsm_urbanite); // Destroy urbanite FSM
    fsm_button_destroy(p_fsm_button); // Destroy button FSM
    fsm_ultrasound_destroy(p_fsm_ultrasound_rear); // Destroy ultrasound FSM
    fsm_display_destroy(p_fsm_display_rear); // Destroy display FSM
    return 0;
}
