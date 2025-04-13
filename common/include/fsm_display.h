/**
 * @file fsm_display.h
 * @brief Header for fsm_display.c file.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * @date 08/04/2025
 */

#ifndef FSM_DISPLAY_SYSTEM_H_
#define FSM_DISPLAY_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include "fsm.h"
/* Defines and enums ----------------------------------------------------------*/
#define DANGER_MIN_CM 0 /*!< inimum distance (in cm) for the "Danger" state.*/

#define WARNING_MIN_CM 25 /*!< Minimum distance (in cm) for the "Warning" state.*/
 
#define NO_PROBLEM_MIN_CM 50 /*!< Minimum distance (in cm) for the "No Problem" state.*/
 
#define INFO_MIN_CM 150 /*!< Minimum distance (in cm) for the "Info" state.*/
 
#define OK_MIN_CM 175 /*!< Minimum distance (in cm) for the "OK" state.*/
 
#define OK_MAX_CM 200 /*!< Maximum distance (in cm) for the "OK" state.*/
/* Enums */

/* Defines and enums ----------------------------------------------------------*/

/**
 * @brief Enum representing the states of the FSM display system.
 */

enum  FSM_DISPLAY_SYSTEM {
    WAIT_DISPLAY = 0,
    SET_DISPLAY
  };

/* Typedefs --------------------------------------------------------------------*/

/**
 * @brief Opaque structure representing the FSM for the display system.
 */

typedef struct fsm_display_t fsm_display_t;

/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Creates a new FSM instance for the display system.
 * 
 * @param display_id The ID of the display to associate with the FSM.
 * @return Pointer to the newly created FSM instance.
 */

fsm_display_t * fsm_display_new(uint32_t display_id);

/**
 * @brief Destroys an FSM instance and frees its resources.
 * 
 * @param p_fsm Pointer to the FSM instance to destroy.
 */
 
void fsm_display_destroy(fsm_display_t *p_fsm);

/**
 * @brief Gets the current distance in centimeters.
 * 
 * @param p_fsm Pointer to the FSM instance.
 * 
 * @return The current distance in centimeters.
 */

 uint32_t fsm_display_get_distance(fsm_display_t *p_fsm);

/**
 * @brief Sets the distance (in cm) for the FSM to process.
 * 
 * @param p_fsm Pointer to the FSM instance.
 * @param distance_cm The distance in centimeters.
 */

void fsm_display_set_distance(fsm_display_t *p_fsm, uint32_t distance_cm);
 
/**
 * @brief Triggers the FSM to process its current state and transition if necessary.
 * 
 * @param p_fsm Pointer to the FSM instance.
 */

void fsm_display_fire(fsm_display_t *p_fsm);
 
/**
 * @brief Gets the current status of the FSM (active or paused).
 * 
 * @param p_fsm Pointer to the FSM instance.
 * 
 * @return `true` if the FSM is active, `false` if it is paused.
 */

bool fsm_display_get_status(fsm_display_t *p_fsm);
 
/**
 * @brief Sets the status of the FSM (active or paused).
 * 
 * @param p_fsm Pointer to the FSM instance.
 * @param pause `true` to pause the FSM, `false` to activate it.
 */

void fsm_display_set_status(fsm_display_t *p_fsm, bool pause);
 
/**
 * @brief Checks if the FSM is currently active.
 * 
 * @param p_fsm Pointer to the FSM instance.
 * 
 * @return `true` if the FSM is active, `false` otherwise.
 */

bool fsm_display_check_activity(fsm_display_t *p_fsm);

/**
 * @brief Retrieves the inner FSM structure for advanced control.
 * 
 * @param p_fsm Pointer to the FSM instance.
 * 
 * @return Pointer to the inner FSM structure.
 */
 
fsm_t * fsm_display_get_inner_fsm (fsm_display_t *p_fsm);

/**
 * @brief Retrieves the inner FSM structure for advanced control.
 * 
 * @param p_fsm Pointer to the FSM instance.
 * 
 * @return Pointer to the inner FSM structure.
 */
 
uint32_t fsm_display_get_state(fsm_display_t *p_fsm);
 
/**
 * @brief Sets the current state of the FSM.
 * 
 * @param p_fsm Pointer to the FSM instance.
 * @param state The state to set.
 */

void fsm_display_set_state(fsm_display_t *p_fsm, int8_t state);

#endif /* FSM_DISPLAY_SYSTEM_H_ */