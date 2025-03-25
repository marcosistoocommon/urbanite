/**
 * @file interr.c
 * @brief Interrupt service routines for the STM32F4 platform.
 * @author SDG2. Román Cárdenas (r.cardenas@upm.es) and Josué Pagán (j.pagan@upm.es)
 * @date 2025-01-01
 */
// Include HW dependencies:
#include "port_system.h"
#include "stm32f4_system.h"
#include <port_button.h>
#include <port_ultrasound.h>

// Include headers of different port elements:

//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
//------------------------------------------------------
/**
 * @brief Interrupt service routine for the System tick timer (SysTick).
 *
 * @note This ISR is called when the SysTick timer generates an interrupt.
 * The program flow jumps to this ISR and increments the tick counter by one millisecond.
 *
 * > **TO-DO alumnos:**
 * >
 * > ✅ 1. **Increment the System tick counter `msTicks` in 1 count.** To do so, use the function `port_system_get_millis()` and `port_system_set_millis()`.
 *
 * @warning **The variable `msTicks` must be declared volatile!** Just because it is modified by a call of an ISR, in order to avoid [*race conditions*](https://en.wikipedia.org/wiki/Race_condition). **Added to the definition** after *static*.
 *
 */
void SysTick_Handler(void)
{
    uint32_t temp = port_system_get_millis();
    port_system_set_millis(temp + 1);
}

void EXTI15_10_IRQHandler (void)
{
    /* ISR parking button */
    if (port_button_get_pending_interrupt (PORT_PARKING_BUTTON_ID))
    {
        if(port_button_get_value (PORT_PARKING_BUTTON_ID)){
            port_button_set_pressed (PORT_PARKING_BUTTON_ID, false);
        }else{
            port_button_set_pressed (PORT_PARKING_BUTTON_ID, true);
        }
        port_button_clear_pending_interrupt (PORT_PARKING_BUTTON_ID);

    }
}


void TIM3_IRQHandler(void){
    TIM3->SR &= ~TIM_SR_UIF;
    port_ultrasound_set_trigger_end(PORT_REAR_PARKING_SENSOR_ID, true);
}

void TIM2_IRQHandler(void){
    uint32_t current_tick;
    uint32_t ultrasound_id = 0;

    if (TIM2->SR & TIM_SR_UIF) {
        uint32_t overflows = port_ultrasound_get_echo_overflows(ultrasound_id);
        port_ultrasound_set_echo_overflows(ultrasound_id, overflows + 1);
        TIM2->SR &= ~TIM_SR_UIF;
    }
    if (TIM2->SR & TIM_SR_CC2IF) {
        current_tick = TIM2->CCR2;
        uint32_t echo_init_tick = port_ultrasound_get_echo_init_tick(ultrasound_id);
        uint32_t echo_end_tick = port_ultrasound_get_echo_end_tick(ultrasound_id);
        if (echo_init_tick == 0 && echo_end_tick == 0) {
            port_ultrasound_set_echo_init_tick(ultrasound_id, current_tick);
        } else {
            port_ultrasound_set_echo_end_tick(ultrasound_id, current_tick);
            port_ultrasound_set_echo_received(ultrasound_id, true);
        }
        TIM2->SR &= ~TIM_SR_CC2IF;
    }
}

void TIM5_IRQHandler(void)
{
    // Clear the interrupt flag UIF in the status register SR
    TIM5->SR &= ~TIM_SR_UIF;
    // Call the function to set the flag that indicates that a new measurement can be started
    port_ultrasound_set_trigger_ready(PORT_REAR_PARKING_SENSOR_ID, true);
}
