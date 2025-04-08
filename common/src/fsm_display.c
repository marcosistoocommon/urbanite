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

struct fsm_display_t
{
    fsm_t f;
    int32_t distance_cm;
    bool new_color;
    bool status;
    bool idle;
    uint32_t display_id;
};


/* Private functions -----------------------------------------------------------*/
//Falta posible return
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

static bool check_active(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    return p_fsm->status;
}

static bool check_set_new_color(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    return p_fsm->new_color;
}

static bool check_off(fsm_t *p_this)
{
    return !check_active(p_this);
}

/* State machine output or action functions */

static void do_set_on(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    port_display_set_rgb(p_fsm->display_id, COLOR_OFF);
}

static void do_set_color(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    rgb_color_t color;
    _compute_display_levels(&color, p_fsm->distance_cm);
    port_display_set_rgb(p_fsm->display_id, color);
    p_fsm->new_color = false;
    p_fsm->idle = true;
}

static void do_set_off(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    port_display_set_rgb(p_fsm->display_id, COLOR_OFF);
    p_fsm->idle = false;
}
fsm_trans_t fsm_trans_display[] = { 
    {WAIT_DISPLAY, check_active, SET_DISPLAY, do_set_on},
    {SET_DISPLAY, check_set_new_color, SET_DISPLAY, do_set_color},
    {SET_DISPLAY, check_off, WAIT_DISPLAY, do_set_off},
    {-1, NULL, -1, NULL},
};


/* Other auxiliary functions */

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
fsm_display_t *fsm_display_new(uint32_t display_id)
{
    fsm_display_t *p_fsm_display = malloc(sizeof(fsm_display_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_display_init(p_fsm_display, display_id); /* Initialize the FSM */
    return p_fsm_display;
}

void fsm_display_fire(fsm_display_t *p_fsm)
{
    fsm_fire(&p_fsm->f); 
}

void fsm_display_destroy(fsm_display_t *p_fsm)
{
    free(&p_fsm->f);
}

fsm_t *fsm_display_get_inner_fsm(fsm_display_t *p_fsm)
{
    return &p_fsm->f;
}

uint32_t fsm_display_get_state(fsm_display_t *p_fsm)
{
    return p_fsm->f.current_state;
}

uint32_t fsm_display_get_distance(fsm_display_t *p_fsm)
{
    return p_fsm->distance_cm;
}

void fsm_display_set_distance(fsm_display_t *p_fsm, uint32_t distance_cm)
{
    p_fsm->distance_cm = distance_cm;
    p_fsm->new_color = true;
}

bool fsm_display_get_status(fsm_display_t *p_fsm)
{
    return p_fsm->status;
}

void fsm_display_set_status(fsm_display_t *p_fsm, bool status)
{
    p_fsm->status = status;
}

void fsm_display_set_state(fsm_display_t *p_fsm, int8_t state)
{
    p_fsm->f.current_state = state;
}

bool fsm_display_check_activity(fsm_display_t *p_fsm)
{
    return !(p_fsm->idle);
}