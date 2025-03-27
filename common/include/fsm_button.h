/**
 * @file fsm_button.h
 * @brief Header for fsm_button.c file.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * @date 28/03/2025
 */

#ifndef FSM_BUTTON_H_
#define FSM_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"


/* Defines and enums ----------------------------------------------------------*/
/* Enums */
/**
 * @brief Enumeration of button states in the FSM.
 */

enum FSM_BUTTON_STATES {
    BUTTON_RELEASED = 0,
    BUTTON_RELEASED_WAIT,
    BUTTON_PRESSED,
    BUTTON_PRESSED_WAIT
};

/* Typedefs --------------------------------------------------------------------*/

/**
 * @brief Opaque structure representing the button FSM.
 */
typedef struct fsm_button_t fsm_button_t;


/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Creates a new button FSM instance.
 * 
 * @param debounce_time_ms Debounce time in milliseconds.
 * @param button_id Identifier for the button.
 * 
 * @return Pointer to the newly created button FSM instance.
 */

fsm_button_t * 	fsm_button_new (uint32_t debounce_time_ms, uint32_t button_id);

/**
 * @brief Destroys a button FSM instance.
 * 
 * @param p_fsm Pointer to the button FSM instance to destroy.
 */

void fsm_button_destroy (fsm_button_t *p_fsm);

/**
 * @brief Triggers the FSM to process the next state transition.
 * 
 * @param p_fsm Pointer to the button FSM instance.
 */

void fsm_button_fire (fsm_button_t *p_fsm);

/**
 * @brief Gets the inner FSM of the button FSM.
 * 
 * @param p_fsm Pointer to the button FSM.
 * 
 * @return Pointer to the inner FSM.
 */

fsm_t * fsm_button_get_inner_fsm (fsm_button_t *p_fsm);

/**
 * @brief Gets the current state of the button FSM.
 * 
 * @param p_fsm Pointer to the button FSM instance.
 * 
 * @return Current state of the button FSM.
 */

uint32_t fsm_button_get_state (fsm_button_t *p_fsm);

/**
 * @brief Gets the duration of the current state.
 * 
 * @param p_fsm Pointer to the button FSM instance.
 * 
 * @return Duration of the current state in milliseconds.
 */

uint32_t fsm_button_get_duration (fsm_button_t *p_fsm);

/**
 * @brief Resets the duration of the current state.
 * 
 * @param p_fsm Pointer to the button FSM instance.
 */

void fsm_button_reset_duration (fsm_button_t *p_fsm);

/**
 * @brief Gets the debounce time for the button FSM.
 * 
 * @param p_fsm Pointer to the button FSM instance.
 * 
 * @return Debounce time in milliseconds.
 */

uint32_t fsm_button_get_debounce_time_ms (fsm_button_t *p_fsm);

/**
 * @brief Checks if the button FSM has detected activity.
 * 
 * @param p_fsm Pointer to the button FSM instance.
 * 
 * @return True if activity is detected, false otherwise.
 */

bool fsm_button_check_activity (fsm_button_t *p_fsm);


#endif