/**
 * @file fsm_ultrasound.h
 * @brief Header for fsm_ultrasound.c file.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * @date 28/03/2025
 */

#ifndef FSM_ULTRASOUND_H_
#define FSM_ULTRASOUND_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include "fsm.h"

/* Defines and enums ----------------------------------------------------------*/
#define FSM_ULTRASOUND_NUM_MEASUREMENTS  5 /*!< Number of measurements to average */

/**
 * @brief States of the ultrasound FSM.
 * 
 */
enum  	FSM_ULTRASOUND { 
    WAIT_START = 0,
    TRIGGER_START,
    WAIT_ECHO_START,
    WAIT_ECHO_END,
    SET_DISTANCE
  };
/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure of the Ultrasound FSM.
 * 
 */
typedef struct fsm_ultrasound_t fsm_ultrasound_t;
/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Creates a new ultrasound FSM.
 * 
 * @param ultrasound_id ID of the ultrasound sensor.
 * @return fsm_ultrasound_t* Pointer to the new ultrasound FSM.
 */
fsm_ultrasound_t *fsm_ultrasound_new (uint32_t ultrasound_id);

/**
 * @brief Destroys an ultrasound FSM.
 * 
 * @param p_fsm Pointer to the ultrasound FSM to destroy.
 */
void fsm_ultrasound_destroy (fsm_ultrasound_t *p_fsm);

/**
 * @brief Retrieves the distance measured.
 * 
 * @param p_fsm Pointer to the ultrasound FSM.
 */
uint32_t fsm_ultrasound_get_distance (fsm_ultrasound_t *p_fsm);

/**
 * @brief Fires the ultrasound FSM.
 * 
 * @param p_fsm Pointer to the ultrasound FSM.
 */
void fsm_ultrasound_fire (fsm_ultrasound_t *p_fsm);

/**
 * @brief Retrieves the status of the ultrasound FSM.
 * 
 * @param p_fsm Pointer to the ultrasound FSM.
 * @return true 
 * @return false 
 */
bool fsm_ultrasound_get_status (fsm_ultrasound_t *p_fsm);

/**
 * @brief Sets the status of the ultrasound FSM.
 * 
 * @param p_fsm Pointer to the ultrasound FSM.
 * @param status New status of the ultrasound FSM.
 */
void fsm_ultrasound_set_status (fsm_ultrasound_t *p_fsm, bool status);

/**
 * @brief Retrieves the value of the trigger ready of the ultrasound FSM.
 * 
 * @param p_fsm Pointer to the ultrasound FSM.
 * @return true 
 * @return false 
 */
bool fsm_ultrasound_get_ready (fsm_ultrasound_t *p_fsm);

/**
 * @brief Retrieves the value of the new measurement flag of the ultrasound FSM.
 * 
 * @param p_fsm Pointer to the ultrasound FSM.
 * @return true 
 * @return false 
 */
bool fsm_ultrasound_get_new_measurement_ready (fsm_ultrasound_t *p_fsm);

/**
 * @brief Stops the ultrasound FSM.
 * 
 * @param p_fsm Pointer to the ultrasound FSM.
 */
void fsm_ultrasound_stop (fsm_ultrasound_t *p_fsm);

/**
 * @brief Starts the ultrasound FSM.
 * 
 * @param p_fsm Pointer to the ultrasound FSM.
 */
void fsm_ultrasound_start (fsm_ultrasound_t *p_fsm);

/**
 * @brief Retrieves the inner FSM of the ultrasound FSM.
 * 
 * @param p_fsm Pointer to the ultrasound FSM.
 * @return fsm_t* Pointer to the inner FSM.
 */
fsm_t *fsm_ultrasound_get_inner_fsm (fsm_ultrasound_t *p_fsm);

/**
 * @brief Retrieves the state of the ultrasound FSM.
 * 
 * @param p_fsm Pointer to the ultrasound FSM.
 * @return uint32_t State of the ultrasound FSM.
 */
uint32_t fsm_ultrasound_get_state (fsm_ultrasound_t *p_fsm);


/**
 * @brief TODO in v4
 * 
 * @param p_fsm Pointer to the ultrasound FSM.
 * @return false
 */
bool fsm_ultrasound_check_activity (fsm_ultrasound_t *p_fsm);



/**
 * @brief Set the state of the ultrasound FSM.
 *
 * This function sets the current state of the ultrasound FSM.
 *
 * > &nbsp;&nbsp;&nbsp;&nbsp;💡 This function is important because the struct is private and external functions such as those of the unit tests cannot access the state of the FSM directly. \n
 * 
 * @param p_fsm Pointer to an `fsm_ultrasound_t` struct.
 * @param state New state of the ultrasound FSM.
 */
void fsm_ultrasound_set_state(fsm_ultrasound_t *p_fsm, int8_t state);

#endif /* FSM_ULTRASOUND_H_ */
