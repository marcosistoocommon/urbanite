/**
 * @file fsm_button.c
 * @brief Button FSM main file.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * @date 28/03/2025
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */

#include <stdlib.h>

/* HW dependent includes */
#include "port_button.h"
#include "port_system.h"

/* Project includes */
#include "fsm_button.h"

/**
 * @brief Structure of the Button FSM.
 */

struct fsm_button_t
{
    fsm_t f; /*!< Base FSM structure */
    uint32_t debounce_time; /*!< Debounce time in milliseconds */
    uint32_t next_timeout; /*!< Next timeout value in milliseconds */
    uint32_t tick_pressed; /*!< Timestamp when the button was pressed */
    uint32_t duration; /*!< Duration the button was pressed in milliseconds */
    uint32_t button_id; /*!< ID of the button */
};


/* State machine input or transition functions */

/**
 * @brief Checks if the button is pressed.
 * @param p_this Pointer to the FSM instance.
 * @return true if the button is pressed, false otherwise.
 */

static bool check_button_pressed(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    return port_button_get_pressed(p_fsm->button_id);
}

/**
 * @brief Checks if the button is released.
 * @param p_this Pointer to the FSM instance.
 * @return true if the button is released, false otherwise.
 */

static bool check_button_released(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    return !port_button_get_pressed(p_fsm->button_id);
}

/**
 * @brief Checks if the debounce timeout has elapsed.
 * @param p_this Pointer to the FSM instance.
 * @return true if the timeout has elapsed, false otherwise.
 */

static bool check_timeout(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    return port_system_get_millis()>= p_fsm->next_timeout;
}

/* State machine output or action functions */

/**
 * @brief Stores the timestamp when the button was pressed.
 * @param p_this Pointer to the FSM instance.
 */

static void do_store_tick_pressed(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    p_fsm->tick_pressed = port_system_get_millis();
    p_fsm->next_timeout = p_fsm->tick_pressed + p_fsm->debounce_time;
}

/**
 * @brief Sets the duration the button was pressed.
 * @param p_this Pointer to the FSM instance.
 */

static void do_set_duration(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    uint32_t current_tick = port_system_get_millis();
    p_fsm->duration = current_tick - p_fsm->tick_pressed;
    p_fsm->next_timeout = current_tick + p_fsm->debounce_time;
}

/**
 * @brief State transition table for the Button FSM.
 */

static fsm_trans_t fsm_trans_button[] = {
    {BUTTON_RELEASED, check_button_pressed, BUTTON_PRESSED_WAIT, do_store_tick_pressed},
    {BUTTON_PRESSED_WAIT, check_timeout, BUTTON_PRESSED, NULL},
    {BUTTON_PRESSED, check_button_released, BUTTON_RELEASED_WAIT, do_set_duration},
    {BUTTON_RELEASED_WAIT, check_timeout, BUTTON_RELEASED, NULL},
    {-1, NULL, -1, NULL},
};


/* Other auxiliary functions */

/**
 * @brief Initializes the Button FSM.
 * @param p_fsm_button Pointer to the Button FSM instance.
 * @param debounce_time Debounce time in milliseconds.
 * @param button_id ID of the button.
 */

void fsm_button_init(fsm_button_t *p_fsm_button, uint32_t debounce_time, uint32_t button_id)
{
    fsm_init(&p_fsm_button->f, fsm_trans_button);

    /* TODO alumnos: */
    p_fsm_button->debounce_time = debounce_time;
    p_fsm_button->button_id = button_id;
    p_fsm_button->tick_pressed = 0;
    p_fsm_button->duration = 0;
    port_button_init(button_id);
}

/* Public functions -----------------------------------------------------------*/

/**
 * @brief Creates a new Button FSM instance.
 * @param debounce_time Debounce time in milliseconds.
 * @param button_id ID of the button.
 * @return Pointer to the newly created Button FSM instance.
 */

fsm_button_t *fsm_button_new(uint32_t debounce_time, uint32_t button_id)
{
    fsm_button_t *p_fsm_button = malloc(sizeof(fsm_button_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_button_init(p_fsm_button, debounce_time, button_id);   /* Initialize the FSM */
    return p_fsm_button;                                       /* Composite pattern: return the fsm_t pointer as a fsm_button_t pointer */
}

/* FSM-interface functions. These functions are used to interact with the FSM */

/**
 * @brief Triggers the FSM to process the next state transition.
 * @param p_fsm Pointer to the Button FSM instance.
 */

void fsm_button_fire(fsm_button_t *p_fsm)
{
    fsm_fire(&p_fsm->f); // Is it also possible to it in this way: fsm_fire((fsm_t *)p_fsm);
}

/**
 * @brief Destroys a Button FSM instance.
 * @param p_fsm Pointer to the Button FSM instance to destroy.
 */

void fsm_button_destroy(fsm_button_t *p_fsm)
{
    free(&p_fsm->f);
}

/**
 * @brief Retrieves the inner FSM structure.
 * @param p_fsm Pointer to the Button FSM instance.
 * @return Pointer to the inner FSM structure.
 */

fsm_t *fsm_button_get_inner_fsm(fsm_button_t *p_fsm)
{
    return &p_fsm->f;
}

/**
 * @brief Gets the current state of the Button FSM.
 * @param p_fsm Pointer to the Button FSM instance.
 * @return Current state of the Button FSM.
 */

uint32_t fsm_button_get_state(fsm_button_t *p_fsm)
{
    return p_fsm->f.current_state;
}

/**
 * @brief Gets the duration the button was pressed.
 * @param p_fsm Pointer to the Button FSM instance.
 * @return Duration in milliseconds.
 */

uint32_t fsm_button_get_duration(fsm_button_t *p_fsm)
{
    return p_fsm->duration;
}

/**
 * @brief Resets the duration of the button press.
 * @param p_fsm Pointer to the Button FSM instance.
 */

void fsm_button_reset_duration(fsm_button_t *p_fsm)
{
    p_fsm->duration = 0;
}

/**
 * @brief Gets the debounce time of the Button FSM.
 * @param p_fsm Pointer to the Button FSM instance.
 * @return Debounce time in milliseconds.
 */

uint32_t fsm_button_get_debounce_time_ms(fsm_button_t *p_fsm)
{
    return p_fsm->debounce_time;
}
