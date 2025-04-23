/**
 * @file fsm_urbanite.c
 * @brief Implementation of the finite state machine (FSM) for the Urbanite system.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * * @date fecha
 */

/* Includes ------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "port_system.h"
#include "fsm.h"
#include "fsm_urbanite.h"

/* Typedefs ------------------------------------------------------------------*/
/**
 * @brief Structure representing the FSM for the Urbanite system.
 */
struct fsm_urbanite_t{
    fsm_t f; /*!< Base FSM structure */
    fsm_button_t * p_fsm_button; /*!< Pointer to the button FSM. */
    uint32_t on_off_press_time_ms; /*!< Time in milliseconds to toggle the system on/off. */
    uint32_t pause_display_time_ms; /*!< Time in milliseconds to pause/resume the display. */
    bool is_paused; /*!< Indicates if the display is paused. */
    fsm_ultrasound_t * p_fsm_ultrasound_rear; /*!< Pointer to the rear ultrasound FSM. */
    fsm_display_t * p_fsm_display_rear; /*!< Pointer to the rear display FSM.  */
};

/* Private functions ---------------------------------------------------------*/

/**
 * @brief Checks if the system should be turned on.
 * 
 * @param p_this Pointer to the FSM instance.
 * 
 * @return `true` if the system should be turned on, `false` otherwise.
 */

static bool check_on (fsm_t *p_this){
    fsm_urbanite_t *p_fsm_urbanite = (fsm_urbanite_t *) p_this;
    if (fsm_button_get_duration(p_fsm_urbanite->p_fsm_button)>(p_fsm_urbanite->on_off_press_time_ms)){
        return true;
    }
    return false;
}

/**
 * @brief Checks if the system should be turned off.
 * 
 * @param p_this Pointer to the FSM instance.
 * 
 * @return `true` if the system should be turned off, `false` otherwise.
 */

static bool check_off (fsm_t *p_this){
   return check_on(p_this);
}

/**
 * @brief Checks if a new ultrasound measurement is ready.
 * 
 * @param p_this Pointer to the FSM instance.
 * 
 * @return `true` if a new measurement is ready, `false` otherwise.
 */

static bool check_new_measure (fsm_t *p_this){
    fsm_urbanite_t *p_fsm_urbanite = (fsm_urbanite_t *) p_this;
    return fsm_ultrasound_get_new_measurement_ready(p_fsm_urbanite->p_fsm_ultrasound_rear);
}

/**
 * @brief Checks if the display should be paused or resumed.
 * 
 * @param p_this Pointer to the FSM instance.
 * 
 * @return `true` if the display should be paused/resumed, `false` otherwise.
 */

static bool check_pause_display (fsm_t *p_this){
    fsm_urbanite_t *p_fsm_urbanite = (fsm_urbanite_t *) p_this;
    if ((fsm_button_get_duration(p_fsm_urbanite->p_fsm_button)>(p_fsm_urbanite->pause_display_time_ms)) && (fsm_button_get_duration(p_fsm_urbanite->p_fsm_button)<(p_fsm_urbanite->on_off_press_time_ms))){
        return true;
    }
    return false;
}

/**
 * @brief Checks if there is activity in any of the subsystems.
 * 
 * @param p_this Pointer to the FSM instance.
 * 
 * @return `true` if there is activity, `false` otherwise.
 */

static bool check_activity (fsm_t *p_this){
    fsm_urbanite_t *p_fsm_urbanite = (fsm_urbanite_t *) p_this;
    return (fsm_button_check_activity(p_fsm_urbanite->p_fsm_button) || fsm_ultrasound_check_activity(p_fsm_urbanite->p_fsm_ultrasound_rear) || fsm_display_check_activity(p_fsm_urbanite->p_fsm_display_rear));
}

/**
 * @brief Checks if there is no activity in any of the subsystems.
 * 
 * @param p_this Pointer to the FSM instance.
 * 
 * @return `true` if there is no activity, `false` otherwise.
 */

static bool check_no_activity (fsm_t *p_this){
    return (!check_activity(p_this));
}

/**
 * @brief Checks if there is activity in the measurement state.
 * 
 * @param p_this Pointer to the FSM instance.
 * 
 * @return `true` if there is activity in the measurement state, `false` otherwise.
 */

static bool check_activity_in_measure (fsm_t *p_this){
    return check_new_measure(p_this); 
}

/**
 * @brief Starts the Urbanite system and initializes measurements.
 * 
 * @param p_this Pointer to the FSM instance.
 */

static void do_start_up_measure (fsm_t *p_this){
    fsm_urbanite_t *p_fsm_urbanite = (fsm_urbanite_t *) p_this;
    fsm_button_reset_duration(p_fsm_urbanite->p_fsm_button);
    fsm_ultrasound_start(p_fsm_urbanite->p_fsm_ultrasound_rear);
    fsm_display_set_status(p_fsm_urbanite->p_fsm_display_rear, true);
    printf("[URBANITE][%ld] Urbanite system ON\n", port_system_get_millis());
}

/**
 * @brief Displays the distance measured by the ultrasound sensor.
 * 
 * @param p_this Pointer to the FSM instance.
 */

static void do_display_distance (fsm_t *p_this){
    fsm_urbanite_t *p_fsm_urbanite = (fsm_urbanite_t *) p_this;

    uint32_t distance_cm = fsm_ultrasound_get_distance(p_fsm_urbanite->p_fsm_ultrasound_rear);

    if (p_fsm_urbanite->is_paused) {
        if (distance_cm < (WARNING_MIN_CM / 2)) {
            fsm_display_set_distance(p_fsm_urbanite->p_fsm_display_rear, distance_cm);
            fsm_display_set_status(p_fsm_urbanite->p_fsm_display_rear, true);
            printf("[URBANITE][%ld] DANGER: Distance: %ld cm\n", port_system_get_millis(), distance_cm);
        } else {
            fsm_display_set_status(p_fsm_urbanite->p_fsm_display_rear, false);
        }
    } else {
        fsm_display_set_distance(p_fsm_urbanite->p_fsm_display_rear, distance_cm);
        printf("[URBANITE][%ld] Distance: %ld cm\n", port_system_get_millis(), distance_cm);
    }
}

/**
 * @brief Pauses or resumes the display.
 * 
 * @param p_this Pointer to the FSM instance.
 */

static void do_pause_display (fsm_t *p_this){
    fsm_urbanite_t *p_fsm_urbanite = (fsm_urbanite_t *) p_this;

    fsm_button_reset_duration(p_fsm_urbanite->p_fsm_button);

    p_fsm_urbanite->is_paused = !p_fsm_urbanite->is_paused;

    fsm_display_set_status(p_fsm_urbanite->p_fsm_display_rear, !p_fsm_urbanite->is_paused);

    if (p_fsm_urbanite->is_paused) {
        printf("[URBANITE][%ld] Urbanite system display PAUSE\n", port_system_get_millis());
    } else {
        printf("[URBANITE][%ld] Urbanite system display RESUME\n", port_system_get_millis());
    }
}
 
/**
 * @brief Stops the Urbanite system.
 * 
 * @param p_this Pointer to the FSM instance.
 */

static void do_stop_urbanite (fsm_t *p_this){
    fsm_urbanite_t *p_fsm_urbanite = (fsm_urbanite_t *) p_this;

    fsm_button_reset_duration(p_fsm_urbanite->p_fsm_button);

    fsm_ultrasound_stop(p_fsm_urbanite->p_fsm_ultrasound_rear);

    fsm_display_set_status(p_fsm_urbanite->p_fsm_display_rear, false);

    if (p_fsm_urbanite->is_paused) {
        p_fsm_urbanite->is_paused = false;
    }

    printf("[URBANITE][%ld] Urbanite system OFF\n", port_system_get_millis());
}

/**
 * @brief Puts the system to sleep while off.
 * 
 * @param p_this Pointer to the FSM instance.
 */

static void do_sleep_while_off (fsm_t *p_this){
    port_system_sleep();
}

/**
 * @brief Puts the system to sleep while on.
 * 
 * @param p_this Pointer to the FSM instance.
 */

static void do_sleep_while_on (fsm_t *p_this){
    port_system_sleep();
}

/**
 * @brief Puts the system to sleep while in the measurement state.
 * 
 * @param p_this Pointer to the FSM instance.
 */

static void do_sleep_while_measure (fsm_t *p_this){
    port_system_sleep();
}

static void do_sleep_off (fsm_t *p_this){
    port_system_sleep();
}

/**
 * @brief Transition table for the Urbanite FSM.
 */

static fsm_trans_t fsm_trans_urbanite[] = {
    {OFF, check_no_activity, SLEEP_WHILE_OFF, do_sleep_off},
    {OFF, check_on, MEASURE, do_start_up_measure},
    {MEASURE, check_new_measure, MEASURE, do_display_distance},
    {MEASURE, check_pause_display, MEASURE, do_pause_display},
    {MEASURE, check_no_activity, SLEEP_WHILE_ON, do_sleep_while_on},
    {MEASURE, check_off, OFF, do_stop_urbanite},
    {SLEEP_WHILE_OFF, check_activity, OFF, NULL},
    {SLEEP_WHILE_OFF, check_no_activity, SLEEP_WHILE_OFF, do_sleep_while_off},
    {SLEEP_WHILE_ON, check_activity_in_measure, MEASURE, NULL},
    {SLEEP_WHILE_ON, check_no_activity, SLEEP_WHILE_ON, do_sleep_while_measure},
    {-1, NULL, -1, NULL},
};

/* Public functions ----------------------------------------------------------*/

/**
 * @brief Initializes the Urbanite FSM.
 * 
 * @param p_fsm_urbanite Pointer to the Urbanite FSM instance.
 * @param p_fsm_button Pointer to the button FSM.
 * @param on_off_press_time_ms Time in milliseconds to toggle the system on/off.
 * @param pause_display_time_ms Time in milliseconds to pause/resume the display.
 * @param p_fsm_ultrasound_rear Pointer to the rear ultrasound FSM.
 * @param p_fsm_display_rear Pointer to the rear display FSM.
 */

static void fsm_urbanite_init (fsm_urbanite_t *p_fsm_urbanite, fsm_button_t *p_fsm_button, uint32_t on_off_press_time_ms, uint32_t pause_display_time_ms, fsm_ultrasound_t *p_fsm_ultrasound_rear, fsm_display_t *p_fsm_display_rear){
        
    fsm_init((fsm_t *)p_fsm_urbanite, fsm_trans_urbanite);

    p_fsm_urbanite->p_fsm_button = p_fsm_button;
    p_fsm_urbanite->on_off_press_time_ms = on_off_press_time_ms;
    p_fsm_urbanite->pause_display_time_ms = pause_display_time_ms;
    p_fsm_urbanite->p_fsm_ultrasound_rear = p_fsm_ultrasound_rear;
    p_fsm_urbanite->p_fsm_display_rear = p_fsm_display_rear;

    p_fsm_urbanite->is_paused = false;
}

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

fsm_urbanite_t *fsm_urbanite_new (fsm_button_t *p_fsm_button, uint32_t on_off_press_time_ms, uint32_t pause_display_time_ms, fsm_ultrasound_t *p_fsm_ultrasound_rear, fsm_display_t *p_fsm_display_rear){

        fsm_urbanite_t *p_fsm_urbanite = (fsm_urbanite_t *)malloc(sizeof(fsm_urbanite_t));

        fsm_urbanite_init(p_fsm_urbanite, p_fsm_button, on_off_press_time_ms, pause_display_time_ms, p_fsm_ultrasound_rear, p_fsm_display_rear);
    
        return p_fsm_urbanite;
}

/**
 * @brief Triggers the Urbanite FSM to process its current state and transition if necessary.
 * 
 * @param p_fsm_urbanite Pointer to the Urbanite FSM instance.
 */

void fsm_urbanite_fire (fsm_urbanite_t *p_fsm_urbanite){
    fsm_fire(&p_fsm_urbanite->f);
}
 
**
 * @brief Destroys the Urbanite FSM instance and frees its resources.
 * 
 * @param p_fsm Pointer to the Urbanite FSM instance to destroy.
 */

void fsm_urbanite_destroy (fsm_urbanite_t *p_fsm){
    free(&p_fsm->f);
}