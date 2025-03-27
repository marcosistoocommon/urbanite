/**
 * @file stm32f4_ultrasound.c
 * @brief Portable functions to interact with the ultrasound FSM library. All portable functions must be implemented in this file.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * @date 28/03/2025
 */

/* Standard C includes */
#include <stdio.h>
#include <math.h>
/* HW dependent includes */
#include "port_ultrasound.h"
#include "port_system.h"
/* Microcontroller dependent includes */
#include "stm32f4_system.h"
#include "stm32f4_ultrasound.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure that represents the HW characteristics of the ultrasound sensors connected to the STM32F4 platform.
 * 
 */
typedef struct
{
    GPIO_TypeDef *p_trigger_port; /*!<GPIO where the trigger signal is connected*/
    GPIO_TypeDef *p_echo_port; /*!<GPIO where the echo signal is connected*/
    uint8_t trigger_pin; /*!<Pin where the trigger signal is connected*/
    uint8_t echo_pin; /*!<Pin where the echo signal is connected*/
    uint8_t echo_alt_fun; /*!<Alternate function of the echo signal*/
    bool trigger_ready; /*!<Flag to indicate if the trigger signal is ready to start a new measurement*/
    bool trigger_end; /*!<Flag to indicate if the trigger signal has ended*/
    bool echo_received; /*!<Flag to indicate if the echo signal has been received*/
    uint32_t echo_init_tick; /*!<Initial tick of the echo signal*/
    uint32_t echo_end_tick;    /*!<End tick of the echo signal*/
    uint32_t echo_overflows; /*!<Number of overflows of the echo signal*/
} stm32f4_ultrasound_hw_t;

/* Global variables */
 
/**
 * @brief Array of elements that represents the HW characteristics of the ultrasounds connected to the STM32F4 platform.
 * 
 */
stm32f4_ultrasound_hw_t ultrasounds_arr[] = {
    [PORT_REAR_PARKING_SENSOR_ID] = {
        .p_trigger_port = STM32F4_REAR_PARKING_SENSOR_TRIGGER_GPIO, 
        .trigger_pin = STM32F4_REAR_PARKING_SENSOR_TRIGGER_PIN,
        .p_echo_port=STM32F4_REAR_PARKING_SENSOR_ECHO_GPIO,
        .echo_pin=STM32F4_REAR_PARKING_SENSOR_ECHO_PIN, 
        .echo_alt_fun=STM32F4_AF1,
        .trigger_ready = false, 
        .trigger_end = false, 
        .echo_received = false, 
        .echo_init_tick = 0, 
        .echo_end_tick = 0, 
        .echo_overflows = 0},
};

/* Private functions ----------------------------------------------------------*/

/**
*@brief Configures the timer of the trigger signal
*/
static void _timer_trigger_setup(){
    // Enable the timer clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    // Disable the timer
    TIM3->CR1 &= ~TIM_CR1_CEN;
    // Enable the auto-reload preload
    TIM3->CR1 |= TIM_CR1_ARPE;
    // Reset the counter
    TIM3->CNT = 0;
    //PS and ARR
    double system_core_clock = (double)SystemCoreClock;
    double max_arr = TIMER_MAX_ARR;
    double trigger_duration_us = (double)PORT_PARKING_SENSOR_TRIGGER_UP_US/1000000;
    double psc = round(system_core_clock*trigger_duration_us/(max_arr+1.0)-1.0);
    double arr = round(system_core_clock*trigger_duration_us/(psc+1.0)-1.0);
    if (arr > max_arr)
    {
        psc += 1.0;
        arr = round(system_core_clock*trigger_duration_us/(psc+1.0)-1.0);
    }
    TIM3->PSC = (uint32_t)psc;
    TIM3->ARR = (uint32_t)arr;
    // Generate an update event to update the prescaler value
    TIM3->EGR |= TIM_EGR_UG;
    // Clear the update interrupt flag
    TIM3->SR &= ~TIM_SR_UIF;
    // Enable the update interrupt
    TIM3->DIER |= TIM_DIER_UIE;
    // Set the priority of the timer interrupt
    NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 4, 0));
}

/**
 * @brief Configures the timer of the echo signal.
 * 
 * @param ultrasound_id 
 */
static void _timer_echo_setup(uint32_t ultrasound_id) {
    if (ultrasound_id == PORT_REAR_PARKING_SENSOR_ID) {
        // Enable the timer clock
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
        // Disable the timer
        TIM2 -> CR1 &= ~ TIM_CR1_CEN ;
        // Set the ARR and PSC values
        double arr = TIMER_MAX_ARR;
        double psc = 15;
        TIM2->PSC = (uint32_t)psc;
        TIM2->ARR = (uint32_t)arr;
        // Enable the auto-reload preload
        TIM2->CR1 &= TIM_CR1_ARPE;
        // Generate an update event to update the prescaler value
        TIM2->EGR |= TIM_EGR_UG;
        // Clear the update interrupt flag
        TIM2->CCMR1 &= ~TIM_CCMR1_CC2S;
        // Configure the input capture mode
        TIM2->CCMR1 |= (0x1 << TIM_CCMR1_CC2S_Pos);
        // Configure the input capture prescaler
        TIM2->CCMR1 &= ~TIM_CCMR1_IC2F;
        // Configure the input capture filter
        TIM2->CCER &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
        TIM2->CCER |= (TIM_CCER_CC2P | TIM_CCER_CC2NP);
        // Configure the input capture prescaler
        TIM2->CCMR1 &= ~TIM_CCMR1_IC2PSC;
        //Configure the Capture Compare 2 input
        TIM2->CCER |= TIM_CCER_CC2E;
        TIM2->DIER |= TIM_DIER_CC2IE;
        // Enable the update interrupt
        TIM2->DIER |= TIM_DIER_UIE;  
        // Set the priority of the timer interrupt
        NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0));
}
}

/**
 * @brief Configures the timer used for the measurements.
 */
void _timer_new_measurement_setup(){
    // Enable the timer clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    // Disable the timer
    TIM5->CR1 &= ~TIM_CR1_CEN;
    // Enable the auto-reload preload
    TIM5->CR1 |= TIM_CR1_ARPE;
    //PS and ARR
    double system_core_clock = (double)SystemCoreClock;
    double desired_duration_ms = (double) PORT_PARKING_SENSOR_TIMEOUT_MS/1000; 
    double max_arr = 65535.0;
    double psc = round(system_core_clock*(desired_duration_ms) / (max_arr + 1.0) - 1.0); // 10 Hz for 100 ms period
    double arr = round(system_core_clock*desired_duration_ms/(psc+1.0)-1.0);
    if (arr > max_arr)
    {
        psc += 1.0;
        arr = round(system_core_clock*desired_duration_ms/(psc+1.0)-1.0);
    }
    TIM5->PSC = (uint32_t)psc;
    TIM5->ARR = (uint32_t)arr;
    // Generate an update event to update the prescaler value
    TIM5->EGR |= TIM_EGR_UG;
    // Clear the update interrupt flag
    TIM5->SR &= ~TIM_SR_UIF;
    // Enable the update interrupt
    TIM5->DIER |= TIM_DIER_UIE;
    // Set the priority of the timer interrupt
    NVIC_SetPriority(TIM5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
}	

/**
 * @brief Returns the pointer to the ultrasound sensor with the given ID.
 * 
 * @param ultrasound_id 
 * @return stm32f4_ultrasound_hw_t* 
 */
stm32f4_ultrasound_hw_t *_stm32f4_ultrasound_get(uint32_t ultrasound_id)	
{
        // Return the pointer to the button with the given ID. If the ID is not valid, return NULL.
        if (ultrasound_id < sizeof(ultrasounds_arr) / sizeof(ultrasounds_arr[0]))
        {
            return &ultrasounds_arr[ultrasound_id];
        }
        else
        {
            return NULL;
        }
}


/* Public functions -----------------------------------------------------------*/
void port_ultrasound_init(uint32_t ultrasound_id)
{
    /* Get the ultrasound sensor */
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);

    /* Trigger pin configuration */
    stm32f4_system_gpio_config(p_ultrasound->p_trigger_port, p_ultrasound->trigger_pin, STM32F4_GPIO_MODE_OUT, STM32F4_GPIO_PUPDR_NOPULL);
    /* Echo pin configuration */
    stm32f4_system_gpio_config(p_ultrasound->p_echo_port, p_ultrasound->echo_pin, STM32F4_GPIO_MODE_AF, STM32F4_GPIO_PUPDR_NOPULL);
    stm32f4_system_gpio_config_alternate(p_ultrasound->p_echo_port, p_ultrasound->echo_pin, STM32F4_AF1);
    /* Configure timers */
    _timer_trigger_setup();
    _timer_echo_setup(ultrasound_id);
    _timer_new_measurement_setup();
    /* Initialize the ultrasound sensor */
    p_ultrasound->echo_init_tick = 0;
    p_ultrasound->echo_end_tick = 0;
    p_ultrasound->echo_overflows = 0;
    p_ultrasound->trigger_ready = true;
    p_ultrasound->trigger_end = false;
    p_ultrasound->echo_received = false;
}

// Getters and setters functions


void port_ultrasound_stop_trigger_timer(uint32_t ultrasound_id){
    stm32f4_system_gpio_write(ultrasounds_arr[ultrasound_id].p_trigger_port, ultrasounds_arr[ultrasound_id].trigger_pin, 0);
    TIM3->CR1 &= ~TIM_CR1_CEN;
}


bool port_ultrasound_get_trigger_end(uint32_t ultrasound_id){
    return _stm32f4_ultrasound_get(ultrasound_id)->trigger_end;
}


bool port_ultrasound_get_trigger_ready(uint32_t ultrasound_id){
    return _stm32f4_ultrasound_get(ultrasound_id)->trigger_ready;
}


void port_ultrasound_set_trigger_end(uint32_t ultrasound_id, bool trigger_end){
    _stm32f4_ultrasound_get(ultrasound_id)->trigger_end = trigger_end;
}


void port_ultrasound_set_trigger_ready(uint32_t ultrasound_id, bool trigger_ready){
    _stm32f4_ultrasound_get(ultrasound_id)->trigger_ready = trigger_ready;
}


uint32_t port_ultrasound_get_echo_end_tick	(uint32_t ultrasound_id){
    return _stm32f4_ultrasound_get(ultrasound_id)->echo_end_tick;
}  


uint32_t port_ultrasound_get_echo_init_tick(uint32_t ultrasound_id){
    return _stm32f4_ultrasound_get(ultrasound_id)->echo_init_tick;
}


uint32_t port_ultrasound_get_echo_overflows(uint32_t ultrasound_id){
    return _stm32f4_ultrasound_get(ultrasound_id)->echo_overflows;
}


bool port_ultrasound_get_echo_received(uint32_t ultrasound_id)	{
    return _stm32f4_ultrasound_get(ultrasound_id)->echo_received;
}

void port_ultrasound_reset_echo_ticks(uint32_t 	ultrasound_id){
    _stm32f4_ultrasound_get(ultrasound_id)->echo_init_tick = 0;
    _stm32f4_ultrasound_get(ultrasound_id)->echo_end_tick = 0;
    _stm32f4_ultrasound_get(ultrasound_id)->echo_overflows = 0;
    _stm32f4_ultrasound_get(ultrasound_id)->echo_received = false;
}


void port_ultrasound_set_echo_end_tick(uint32_t ultrasound_id, uint32_t echo_end_tick ){
    _stm32f4_ultrasound_get(ultrasound_id)->echo_end_tick = echo_end_tick;
}


void port_ultrasound_set_echo_init_tick(uint32_t ultrasound_id, uint32_t echo_init_tick){
    _stm32f4_ultrasound_get(ultrasound_id)->echo_init_tick = echo_init_tick;
}


void port_ultrasound_set_echo_overflows(uint32_t ultrasound_id, uint32_t echo_overflows){
    _stm32f4_ultrasound_get(ultrasound_id)->echo_overflows = echo_overflows;
}	


void port_ultrasound_set_echo_received(uint32_t ultrasound_id, bool echo_received){
    _stm32f4_ultrasound_get(ultrasound_id)->echo_received = echo_received;
}	


void port_ultrasound_start_measurement(uint32_t ultrasound_id){
    /* Get the ultrasound sensor */
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    /* Reset the flag trigger_ready to indicate that a new measurement has started */
    p_ultrasound->trigger_ready = false;

    /* Reset the counters (CNT) of the trigger timer, the echo timer, and the new measurement timer */
    if(ultrasound_id==PORT_REAR_PARKING_SENSOR_ID){
        TIM3->CNT = 0; // Trigger timer
        TIM2->CNT = 0; // Echo timer
    }
    TIM5->CNT = 0; // New measurement timer

    /* Set the trigger pin to high */
    stm32f4_system_gpio_write(p_ultrasound->p_trigger_port, p_ultrasound->trigger_pin, 1);
    /* Enable the timers interrupts in the NVIC */
    NVIC_EnableIRQ(TIM3_IRQn); // Trigger timer interrupt
    NVIC_EnableIRQ(TIM2_IRQn); // Echo timer interrupt
    NVIC_EnableIRQ(TIM5_IRQn); // New measurement timer interrupt

    /* Enable the timers */

    if (ultrasound_id== 0)
    {
    TIM3->CR1 |= TIM_CR1_CEN; // Trigger timer

    TIM2->CR1 |= TIM_CR1_CEN; // Echo timer
    }

    TIM5->CR1 |= TIM_CR1_CEN; // New measurement timer
}


void port_ultrasound_start_new_measurement_timer(){
        // Enable the interrupt of the new measurement timer in the NVIC
        NVIC_EnableIRQ(TIM5_IRQn);
        // Enable the new measurement timer (register CR1 of the timer)
        TIM5->CR1 |= TIM_CR1_CEN;
}


void port_ultrasound_stop_echo_timer(uint32_t ultrasound_id){
    if(ultrasound_id==PORT_REAR_PARKING_SENSOR_ID){
        TIM2->CR1 &= ~TIM_CR1_CEN;
    }
}


void port_ultrasound_stop_new_measurement_timer(void){
    TIM5->CR1 &= ~TIM_CR1_CEN;
}


void port_ultrasound_stop_ultrasound(uint32_t ultrasound_id){
        // Stop the trigger timer
        port_ultrasound_stop_trigger_timer(ultrasound_id);

        // Stop the echo timer
        port_ultrasound_stop_echo_timer(ultrasound_id);
    
        // Stop the new measurement timer
        port_ultrasound_stop_new_measurement_timer();
    
        // Reset the echo ticks
        port_ultrasound_reset_echo_ticks(ultrasound_id);
}

// Util
void stm32f4_ultrasound_set_new_trigger_gpio(uint32_t ultrasound_id, GPIO_TypeDef *p_port, uint8_t pin)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->p_trigger_port = p_port;
    p_ultrasound->trigger_pin = pin;
}

void stm32f4_ultrasound_set_new_echo_gpio(uint32_t ultrasound_id, GPIO_TypeDef *p_port, uint8_t pin)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->p_echo_port = p_port;
    p_ultrasound->echo_pin = pin;
}