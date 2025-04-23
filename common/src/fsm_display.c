/**
 * @file fsm_display.c
 * @brief Display system FSM main file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */

#include <stdlib.h>
#include <stdio.h>

/* HW dependent includes */

#include "port_display.h"
#include "port_system.h"
 
/* Project includes */
 
#include "fsm.h"
#include "fsm_display.h"
 
/* Typedefs --------------------------------------------------------------------*/

/**
 * @brief Structure representing the FSM for the display system.
 */

struct fsm_display_t
{
    fsm_t f;                /**< Base FSM structure. */
    int32_t distance_cm;    /**< Distance in centimeters. */
    bool new_color;         /**< Flag indicating if a new color is set. */
    bool status;            /**<  Status of the FSM (active or paused). */
    bool idle;              /**< Idle state of the FSM.*/
    uint32_t display_id;     /**< ID of the associated display. */
};


/* Private functions -----------------------------------------------------------*/

/**
 * @brief Computes the RGB color levels based on the distance.
 * 
 * @param pcolor Pointer to the RGB color structure to update.
 * @param distance_cm The distance in centimeters.
 */

void _compute_display_levels(rgb_color_t *pcolor, int32_t distance_cm){
    if(distance_cm<=WARNING_MIN_CM && distance_cm>=DANGER_MIN_CM){
        pcolor->r=255;
        pcolor->g=0;
        pcolor->b=0;
    }
    else 
    if(distance_cm>WARNING_MIN_CM && distance_cm <=NO_PROBLEM_MIN_CM){
        pcolor->r=237;
        pcolor->g=237;
        pcolor->b=0;
    }
    else
    if(distance_cm>NO_PROBLEM_MIN_CM && distance_cm <=INFO_MIN_CM){
        pcolor->r=0;
        pcolor->g=255;
        pcolor->b=0;
    }
    else
    if(distance_cm>INFO_MIN_CM && distance_cm <=OK_MIN_CM){
        pcolor->r=25;
        pcolor->g=89;
        pcolor->b=81;
    }
    else
    if(distance_cm>OK_MIN_CM && distance_cm <=OK_MAX_CM){
        pcolor->r=0;
        pcolor->g=0;
        pcolor->b=255;
    }
    else{
        pcolor->r=0;
        pcolor->g=0;
        pcolor->b=0;
    }
}

/* State machine input or transition functions */

/**
 * @brief Checks if the FSM is active.
 * 
 * @param p_this Pointer to the FSM instance.
 * 
 * @return `true` if the FSM is active, `false` otherwise.
 */

static bool check_active(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    return p_fsm->status;
}

/**
 * @brief Checks if a new color needs to be set.
 * 
 * @param p_this Pointer to the FSM instance.
 * 
 * @return `true` if a new color needs to be set, `false` otherwise.
 */

static bool check_set_new_color(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    return p_fsm->new_color;
}

/**
 * @brief Checks if the FSM should be turned off.
 * 
 * @param p_this Pointer to the FSM instance.
 * 
 * @return `true` if the FSM should be turned off, `false` otherwise.
 */

static bool check_off(fsm_t *p_this)
{
    return !check_active(p_this);
}

/* State machine output or action functions */

/**
 * @brief Action to set the display to the "on" state.
 * 
 * @param p_this Pointer to the FSM instance.
 */

static void do_set_on(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    port_display_set_rgb(p_fsm->display_id, COLOR_OFF);
}

/**
 * @brief Action to set the display color based on the distance.
 * 
 * @param p_this Pointer to the FSM instance.
 */

static void do_set_color(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    rgb_color_t color;
    _compute_display_levels(&color, p_fsm->distance_cm);
    port_display_set_rgb(p_fsm->display_id, color);
    p_fsm->new_color = false;
    p_fsm->idle = true;
}

/**
 * @brief Action to turn off the display.
 * 
 * @param p_this Pointer to the FSM instance.
 */

static void do_set_off(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    port_display_set_rgb(p_fsm->display_id, COLOR_OFF);
    p_fsm->idle = false;
}

/**
 * @brief Transition table for the FSM.
 */

fsm_trans_t fsm_trans_display[] = { 
    {WAIT_DISPLAY, check_active, SET_DISPLAY, do_set_on},
    {SET_DISPLAY, check_set_new_color, SET_DISPLAY, do_set_color},
    {SET_DISPLAY, check_off, WAIT_DISPLAY, do_set_off},
    {-1, NULL, -1, NULL},
};


/* Other auxiliary functions */

/**
 * @brief Initializes the FSM for the display system.
 * 
 * @param p_fsm_display Pointer to the FSM instance.
 * @param display_id The ID of the associated display.
 */

static void fsm_display_init(fsm_display_t *p_fsm_display, uint32_t display_id){
    fsm_init(&p_fsm_display->f, fsm_trans_display);
    p_fsm_display->distance_cm = -1;
    p_fsm_display->display_id = display_id;
    p_fsm_display->new_color = false;
    p_fsm_display->status = false;
    p_fsm_display->idle = false;
    port_display_init(display_id);
}


/* Public functions -----------------------------------------------------------*/

/**
 * @brief Creates a new FSM instance for the display system.
 * 
 * @param display_id The ID of the display to associate with the FSM.
 * 
 * @return Pointer to the newly created FSM instance.
 */

fsm_display_t *fsm_display_new(uint32_t display_id)
{
    fsm_display_t *p_fsm_display = malloc(sizeof(fsm_display_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_display_init(p_fsm_display, display_id); /* Initialize the FSM */
    return p_fsm_display;
}

/**
 * @brief Triggers the FSM to process its current state and transition if necessary.
 * 
 * @param p_fsm Pointer to the FSM instance.
 */

void fsm_display_fire(fsm_display_t *p_fsm)
{
    fsm_fire(&p_fsm->f); 
}

/**
 * @brief Destroys an FSM instance and frees its resources.
 * 
 * @param p_fsm Pointer to the FSM instance to destroy.
 */

void fsm_display_destroy(fsm_display_t *p_fsm)
{
    free(&p_fsm->f);
}

/**
 * @brief Retrieves the inner FSM structure for advanced control.
 * 
 * @param p_fsm Pointer to the FSM instance.
 * 
 * @return Pointer to the inner FSM structure.
 */

fsm_t *fsm_display_get_inner_fsm(fsm_display_t *p_fsm)
{
    return &p_fsm->f;
}

/**
 * @brief Gets the current state of the FSM.
 * 
 * @param p_fsm Pointer to the FSM instance.
 * @return The current state of the FSM.
 */

uint32_t fsm_display_get_state(fsm_display_t *p_fsm)
{
    return p_fsm->f.current_state;
}

/**
 * @brief Gets the current distance in centimeters.
 * 
 * @param p_fsm Pointer to the FSM instance.
 * 
 * @return The current distance in centimeters.
 */

uint32_t fsm_display_get_distance(fsm_display_t *p_fsm)
{
    return p_fsm->distance_cm;
}

/**
 * @brief Sets the distance for the FSM to process.
 * 
 * @param p_fsm Pointer to the FSM instance.
 * 
 * @param distance_cm The distance in centimeters.
 */

void fsm_display_set_distance(fsm_display_t *p_fsm, uint32_t distance_cm)
{
    p_fsm->distance_cm = distance_cm;
    p_fsm->new_color = true;
}

/**
 * @brief Gets the current status of the FSM (active or paused).
 * 
 * @param p_fsm Pointer to the FSM instance.
 * @return `true` if the FSM is active, `false` otherwise.
 */

bool fsm_display_get_status(fsm_display_t *p_fsm)
{
    return p_fsm->status;
}

/**
 * @brief Sets the status of the FSM (active or paused).
 * 
 * @param p_fsm Pointer to the FSM instance.
 * @param status `true` to activate the FSM, `false` to pause it.
 */

void fsm_display_set_status(fsm_display_t *p_fsm, bool status)
{
    p_fsm->status = status;
}

/**
 * @brief Sets the current state of the FSM.
 * 
 * @param p_fsm Pointer to the FSM instance.
 * @param state The state to set.
 */

void fsm_display_set_state(fsm_display_t *p_fsm, int8_t state)
{
    p_fsm->f.current_state = state;
}

/**
 * @brief Checks if the FSM is currently active.
 * 
 * @param p_fsm Pointer to the FSM instance.
 * @return `true` if the FSM is active, `false` otherwise.
 */

bool fsm_display_check_activity(fsm_display_t *p_fsm)
{
    return !(p_fsm->idle);
}