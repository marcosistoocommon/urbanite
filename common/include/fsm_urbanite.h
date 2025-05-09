/**
 * @file fsm_urbanite.h
 * @brief Implementation of the finite state machine (FSM) for the Urbanite system.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * * @date fecha
 */
#ifndef FSM_URBANITE_H_
#define FSM_URBANITE_H_

#include <stdint.h>
#include "fsm_button.h"
#include "fsm_display.h"
#include "fsm_ultrasound.h"


/* Defines and enums ----------------------------------------------------------*/

/**
 * @brief States of the Urbanite FSM.
 * 
 */
enum FSM_URBANITE {
    OFF = 0,
    MEASURE,
    SLEEP_WHILE_OFF,
    SLEEP_WHILE_ON
  };

  
/**
 * @brief Structure of the Urbanite FSM.
 * 
 */
typedef struct fsm_urbanite_t fsm_urbanite_t;

  /**
 * @brief Creates a new Urbanite FSM instance.
 * 
 * @param p_fsm_button Pointer to the button FSM.
 * @param on_off_press_time_ms Time in milliseconds to toggle the system on/off.
 * @param pause_display_time_ms Time in milliseconds to pause/resume the display.
 * @param p_fsm_ultrasound_rear Pointer to the rear ultrasound FSM.
 * @param p_fsm_display_rear Pointer to the rear display FSM.
 *
 * @return Pointer to the newly created Urbanite FSM instance.
 */
fsm_urbanite_t * fsm_urbanite_new (fsm_button_t *p_fsm_button, uint32_t on_off_press_time_ms, uint32_t pause_display_time_ms, fsm_ultrasound_t *p_fsm_ultrasound_rear, fsm_display_t *p_fsm_display_rear);


/**
 * @brief Triggers the Urbanite FSM to process its current state and transition if necessary.
 * 
 * @param p_fsm Pointer to the Urbanite FSM instance.
 */
void fsm_urbanite_fire (fsm_urbanite_t *p_fsm);



/**
 * @brief Destroys the Urbanite FSM instance and frees its resources.
 * 
 * @param p_fsm Pointer to the Urbanite FSM instance to destroy.
 */
void fsm_urbanite_destroy (fsm_urbanite_t *p_fsm);

#endif /* FSM_URBANITE_H_ */