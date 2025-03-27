/**
 * @file fsm_ultrasound.c
 * @brief Ultrasound sensor FSM main file.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * @date 28/03/2025
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <string.h>


/* HW dependent includes */
#include "port_ultrasound.h"
#include "port_system.h"
/* Project includes */
#include "fsm.h"
#include "fsm_ultrasound.h"

/**
 * @brief Structure of the Ultrasound FSM.
 * 
 */
struct fsm_ultrasound_t
{
    fsm_t f; /*!< Base FSM structure */
    uint32_t distance_cm; /*!< Distance measured in cm */
    bool status; /*!< Status of the ultrasound sensor */
    bool new_measurement; /*!< Flag to indicate if a new measurement is ready */
    uint32_t ultrasound_id; /*!< ID of the ultrasound sensor */
    uint32_t distance_arr[FSM_ULTRASOUND_NUM_MEASUREMENTS]; /*!< Array of distances measured */
    uint32_t distance_idx; /*!< Index of the distance array */

};
/* Typedefs --------------------------------------------------------------------*/

/* Private functions -----------------------------------------------------------*/
/**
 * @brief Comparison function for qsort.
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int _compare(const void *a, const void *b)
{
    return (*(uint32_t *)a - *(uint32_t *)b);
}

/* State machine input or transition functions */
/**
 * @brief Checks if the ultrasound sensor is on.
 * 
 * @param p_this 
 * @return true 
 * @return false 
 */
static bool check_on (fsm_t *p_this){
    fsm_ultrasound_t *p_fsm_ultrasound = (fsm_ultrasound_t *)p_this;
    return port_ultrasound_get_trigger_ready(p_fsm_ultrasound->ultrasound_id);
}

/**
 * @brief Checks if the ultrasound sensor is off.
 * 
 * @param p_this 
 * @return true 
 * @return false 
 */
static bool check_off (fsm_t *p_this){
    fsm_ultrasound_t *p_fsm_ultrasound = (fsm_ultrasound_t *)p_this;
    return !(p_fsm_ultrasound->status);
}


/**
 * @brief Checks if the trigger signal has ended.
 * 
 * @param p_this 
 * @return true 
 * @return false 
 */
static bool check_trigger_end (fsm_t *p_this){
    fsm_ultrasound_t *p_fsm_ultrasound = (fsm_ultrasound_t *)p_this;
    return port_ultrasound_get_trigger_end(p_fsm_ultrasound->ultrasound_id);
}


/**
 * @brief Checks if the echo signal has started.
 * 
 * @param p_this 
 * @return true 
 * @return false 
 */
static bool check_echo_init (fsm_t *p_this){
    fsm_ultrasound_t *p_fsm_ultrasound = (fsm_ultrasound_t *)p_this;
    return (bool) (port_ultrasound_get_echo_init_tick(p_fsm_ultrasound->ultrasound_id)>0);
}


/**
 * @brief Checks if the echo signal has been received.
 * 
 * @param p_this 
 * @return true 
 * @return false 
 */
static bool check_echo_received (fsm_t *p_this){
    fsm_ultrasound_t *p_fsm_ultrasound = (fsm_ultrasound_t *)p_this;
    bool rcv = port_ultrasound_get_echo_received(p_fsm_ultrasound->ultrasound_id);
    return (rcv && check_echo_init(p_this));
}


/**
 * @brief Checks if a new measurement is ready.
 * 
 * @param p_this 
 * @return true 
 * @return false 
 */
static bool check_new_measurement (fsm_t *p_this){
    fsm_ultrasound_t *p_fsm_ultrasound = (fsm_ultrasound_t *)p_this;
    return port_ultrasound_get_trigger_ready(p_fsm_ultrasound->ultrasound_id);
}

/* State machine output or action functions */


/**
 * @brief Starts a measurement.
 * 
 * @param p_this 
 */
static void do_start_measurement (fsm_t *p_this){
    fsm_ultrasound_t *p_fsm_ultrasound = (fsm_ultrasound_t *)p_this;
    port_ultrasound_start_measurement(p_fsm_ultrasound->ultrasound_id);
}


/**
 * @brief Starts a new measurement.
 * 
 * @param p_this 
 */
static void do_start_new_measurement (fsm_t *p_this){
    do_start_measurement(p_this);
}


/**
 * @brief Stops the trigger signal.
 * 
 * @param p_this 
 */
static void do_stop_trigger (fsm_t *p_this){
    fsm_ultrasound_t *p_fsm_ultrasound = (fsm_ultrasound_t *)p_this;
    port_ultrasound_stop_trigger_timer(p_fsm_ultrasound->ultrasound_id);
    port_ultrasound_set_trigger_end(p_fsm_ultrasound->ultrasound_id,false);
}


/**
 * @brief Sets the distance measured by the ultrasound sensor.
 * 
 * @param p_this 
 */
static void do_set_distance (fsm_t *p_this){
    fsm_ultrasound_t *p_fsm_ultrasound = (fsm_ultrasound_t *)p_this;
    int32_t init_tick = port_ultrasound_get_echo_init_tick(p_fsm_ultrasound->ultrasound_id);
    int32_t end_tick = port_ultrasound_get_echo_end_tick(p_fsm_ultrasound->ultrasound_id);
    int32_t over_tick = port_ultrasound_get_echo_overflows(p_fsm_ultrasound->ultrasound_id);
    double time = (double)(end_tick-(init_tick-65535.0*over_tick));
    double distance = (time * SPEED_OF_SOUND_MS)/(2*10000);
    p_fsm_ultrasound->distance_arr[p_fsm_ultrasound->distance_idx] = (uint32_t)distance;
    if(p_fsm_ultrasound->distance_idx==FSM_ULTRASOUND_NUM_MEASUREMENTS-1){
        qsort(p_fsm_ultrasound->distance_arr, FSM_ULTRASOUND_NUM_MEASUREMENTS, sizeof(uint32_t), _compare);
        if (FSM_ULTRASOUND_NUM_MEASUREMENTS % 2 == 0) {
            p_fsm_ultrasound->distance_cm = (p_fsm_ultrasound->distance_arr[FSM_ULTRASOUND_NUM_MEASUREMENTS / 2 - 1]+p_fsm_ultrasound->distance_arr[FSM_ULTRASOUND_NUM_MEASUREMENTS / 2]) / 2;
        } else {
            p_fsm_ultrasound->distance_cm = p_fsm_ultrasound->distance_arr[FSM_ULTRASOUND_NUM_MEASUREMENTS / 2];
        }
        p_fsm_ultrasound->new_measurement = true;
    }
    p_fsm_ultrasound->distance_idx = (p_fsm_ultrasound->distance_idx + 1) % FSM_ULTRASOUND_NUM_MEASUREMENTS;
    port_ultrasound_stop_echo_timer(p_fsm_ultrasound->ultrasound_id);
    port_ultrasound_reset_echo_ticks(p_fsm_ultrasound->ultrasound_id);
}


/**
 * @brief Stops a measurement.
 * 
 * @param p_this 
 */
static void do_stop_measurement (fsm_t *p_this){
    fsm_ultrasound_t *p_fsm_ultrasound = (fsm_ultrasound_t *)p_this;
    port_ultrasound_stop_ultrasound(p_fsm_ultrasound->ultrasound_id);
}


/* State machine definition ----------------------------------------------------*/
/**
 * @brief Ultrasound sensor FSM transitions.
 * 
 */
static fsm_trans_t 	fsm_trans_ultrasound []= {
    {WAIT_START, check_on, TRIGGER_START, do_start_measurement},
    {TRIGGER_START, check_trigger_end, WAIT_ECHO_START, do_stop_trigger},
    {WAIT_ECHO_START, check_echo_init, WAIT_ECHO_END, NULL},
    {WAIT_ECHO_END, check_echo_received, SET_DISTANCE, do_set_distance},
    {SET_DISTANCE, check_new_measurement, TRIGGER_START, do_start_new_measurement},
    {SET_DISTANCE, check_off, WAIT_START, do_stop_measurement},
    {-1, NULL, -1, NULL},
};

/* Other auxiliary functions */
/**
 * @brief Initializes the ultrasound FSM.
 * 
 * @param p_fsm_ultrasound Pointer to the ultrasound FSM.
 * @param ultrasound_id ID of the ultrasound sensor.
 */
void fsm_ultrasound_init(fsm_ultrasound_t *p_fsm_ultrasound, uint32_t ultrasound_id)
{
    // Initialize the FSM
    fsm_init(&p_fsm_ultrasound->f, fsm_trans_ultrasound);

    /* TODO alumnos: */
    // Initialize the fields of the FSM structure
    p_fsm_ultrasound -> distance_cm = 0;
    p_fsm_ultrasound -> distance_idx = 0;
    p_fsm_ultrasound->status = false;
    p_fsm_ultrasound->new_measurement = false;
    p_fsm_ultrasound->ultrasound_id = ultrasound_id;
    memset(p_fsm_ultrasound->distance_arr, 0, sizeof(p_fsm_ultrasound->distance_arr));
    port_ultrasound_init(p_fsm_ultrasound->ultrasound_id);

}

/* Public functions -----------------------------------------------------------*/
/**
 * @brief Creates a new ultrasound FSM.
 * 
 * @param ultrasound_id ID of the ultrasound sensor.
 * @return fsm_ultrasound_t* Pointer to the new ultrasound FSM.
 */
fsm_ultrasound_t *fsm_ultrasound_new(uint32_t ultrasound_id)
{
    fsm_ultrasound_t *p_fsm_ultrasound = malloc(sizeof(fsm_ultrasound_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_ultrasound_init(p_fsm_ultrasound, ultrasound_id);                  /* Initialize the FSM */
    return p_fsm_ultrasound;
}

void fsm_ultrasound_destroy (fsm_ultrasound_t *p_fsm){
    free(&p_fsm->f);
}


void fsm_ultrasound_fire (fsm_ultrasound_t *p_fsm){
    fsm_fire(&p_fsm->f);
}


uint32_t fsm_ultrasound_get_distance (fsm_ultrasound_t *p_fsm){
    p_fsm->new_measurement = false;
    return p_fsm->distance_cm;
}

void fsm_ultrasound_stop (fsm_ultrasound_t *p_fsm){
    p_fsm->status=false;
    port_ultrasound_stop_ultrasound(p_fsm->ultrasound_id);
}

void fsm_ultrasound_start (fsm_ultrasound_t *p_fsm){
    p_fsm->status=true;
    p_fsm->distance_idx=0;
    p_fsm->distance_cm=0;
    port_ultrasound_reset_echo_ticks(p_fsm->ultrasound_id);
    port_ultrasound_set_trigger_ready(p_fsm->ultrasound_id,true);
    port_ultrasound_start_new_measurement_timer();
}

bool fsm_ultrasound_get_status (fsm_ultrasound_t *p_fsm){
    return p_fsm->status;
}

void fsm_ultrasound_set_status (fsm_ultrasound_t *p_fsm, bool status){
    p_fsm->status= status;
}

bool fsm_ultrasound_get_ready (fsm_ultrasound_t *p_fsm){
    return port_ultrasound_get_trigger_ready(p_fsm->ultrasound_id);
}

bool fsm_ultrasound_get_new_measurement_ready (fsm_ultrasound_t *p_fsm){
    return p_fsm->new_measurement;
}

fsm_t *fsm_ultrasound_get_inner_fsm (fsm_ultrasound_t *p_fsm){
    return &p_fsm->f;
}


uint32_t fsm_ultrasound_get_state (fsm_ultrasound_t *p_fsm){
    return p_fsm->f.current_state;
}



// Other auxiliary functions
void fsm_ultrasound_set_state(fsm_ultrasound_t *p_fsm, int8_t state)
{
    p_fsm->f.current_state = state;
}


bool fsm_ultrasound_check_activity (fsm_ultrasound_t *p_fsm){
    return false;
}