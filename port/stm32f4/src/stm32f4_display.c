/**
 * @file stm32f4_display.c
 * @brief Portable functions to interact with the display system FSM library. All portable functions must be implemented in this file.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * @date 01/04/2025
 */

/* Standard C includes */
#include <stdio.h>
#include <math.h>
/* HW dependent includes */
#include "port_display.h"
#include "port_system.h"
/* Microcontroller dependent includes */
#include "stm32f4_system.h"
#include "stm32f4_display.h"
/* Defines --------------------------------------------------------------------*/
#define TIMER_MAX_ARR 0xFFFF
#define frec_PWD 50
/* Typedefs --------------------------------------------------------------------*/
typedef struct {
    GPIO_TypeDef *p_port_red;
    uint8_t pin_red;
    GPIO_TypeDef *p_port_green;
    uint8_t pin_green;
    GPIO_TypeDef * p_port_blue;
    uint8_t 	pin_blue;
} stm32f4_display_hw_t;
/* Global variables */
static 
stm32f4_display_hw_t displays_arr[]= {
    [PORT_REAR_PARKING_DISPLAY_ID] ={
        STM32F4_REAR_PARKING_DISPLAY_RGB_R_GPIO,
        STM32F4_REAR_PARKING_DISPLAY_RGB_R_PIN,
        STM32F4_REAR_PARKING_DISPLAY_RGB_G_GPIO,
        STM32F4_REAR_PARKING_DISPLAY_RGB_G_PIN,
        STM32F4_REAR_PARKING_DISPLAY_RGB_B_GPIO,
        STM32F4_REAR_PARKING_DISPLAY_RGB_B_PIN
    }
};
/* Private functions -----------------------------------------------------------*/
stm32f4_display_hw_t *_stm32f4_display_get(uint32_t display_id)
{
    // Return the pointer to the display with the given ID. If the ID is not valid, return NULL.
    // TO-DO alumnos
    if (display_id < sizeof(displays_arr) / sizeof(displays_arr[0]))
    {
        return &displays_arr[display_id];
    }
    else
    {
        return NULL;
    }
}

void _timer_pwm_config(uint32_t display_id)
{
    if(display_id == PORT_REAR_PARKING_DISPLAY_ID)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; 
        TIM4->CR1 &= ~TIM_CR1_CEN; 
        TIM4->CR1 |= TIM_CR1_ARPE; 
        TIM4->CNT = 0; 
        double system_core_clock = (double)SystemCoreClock;
        double max_arr = TIMER_MAX_ARR;
        double time_PWD = (double)1/frec_PWD;
        double psc = round(system_core_clock*time_PWD/(max_arr+1.0)-1.0);
        double arr = round(system_core_clock*time_PWD/(psc+1.0)-1.0);
        if (arr > max_arr)
        {
            psc += 1.0;
            arr = round(system_core_clock*time_PWD/(psc+1.0)-1.0);
        }
        TIM4->PSC = (uint32_t)psc;
        TIM4->ARR = (uint32_t)arr;
        // Revisar (Se hizo con sueño)
        TIM4->CCER &= ~TIM_CCER_CC1E;
        TIM4->CCER &= ~TIM_CCER_CC3E;
        TIM4->CCER &= ~TIM_CCER_CC4E;
        TIM4->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
        TIM4->CCER &= ~(TIM_CCER_CC3P | TIM_CCER_CC3NP);
        TIM4->CCER &= ~(TIM_CCER_CC4P | TIM_CCER_CC4NP);
        TIM4->CCMR1 |= TIM_CCMR1_OC1M;
        TIM4->CCMR2 |= TIM_CCMR2_OC3M;
        TIM4->CCMR2 |= TIM_CCMR2_OC4M;
        TIM4->CCMR1 |= TIM_CCMR1_OC1PE;
        TIM4->CCMR2 |= TIM_CCMR2_OC3PE;
        TIM4->CCMR2 |= TIM_CCMR2_OC4PE;
        TIM4->EGR |= TIM_EGR_UG;  
    }
}
/* Public functions -----------------------------------------------------------*/
void port_display_init(uint32_t display_id){
    stm32f4_display_hw_t *p_display = _stm32f4_display_get(display_id);
    stm32f4_system_gpio_config(p_display->p_port_red, p_display->pin_red, STM32F4_GPIO_MODE_AF, STM32F4_GPIO_PUPDR_NOPULL);
    stm32f4_system_gpio_config_alternate(p_display->p_port_red, p_display->pin_red, STM32F4_AF2);
    stm32f4_system_gpio_config(p_display->p_port_green, p_display->pin_green, STM32F4_GPIO_MODE_AF, STM32F4_GPIO_PUPDR_NOPULL);
    stm32f4_system_gpio_config_alternate(p_display->p_port_green, p_display->pin_green, STM32F4_AF2);
    stm32f4_system_gpio_config(p_display->p_port_blue, p_display->pin_blue, STM32F4_GPIO_MODE_AF, STM32F4_GPIO_PUPDR_NOPULL);
    stm32f4_system_gpio_config_alternate(p_display->p_port_blue, p_display->pin_blue, STM32F4_AF2);
    _timer_pwm_config(display_id);
    port_display_set_rgb(display_id, COLOR_OFF);
}

void port_display_set_rgb(uint32_t display_id, rgb_color_t color){
    uint8_t r = color.r;
    uint8_t g = color.g;
    uint8_t b = color.b;
    if (display_id == PORT_REAR_PARKING_DISPLAY_ID)
    {
        TIM4->CR1 &= ~TIM_CR1_CEN;
        if((r==0) & (g== 0) & (b==0)){
            TIM4->CCER &= ~TIM_CCER_CC1E;
            TIM4->CCER &= ~TIM_CCER_CC3E;
            TIM4->CCER &= ~TIM_CCER_CC4E;
            return;
        }
        else if(r==0){
            TIM4->CCER &= ~TIM_CCER_CC1E;
        }
        else{
            //Revisar (GPTAZO)
            double duty_cycle = (double)r/(double)PORT_DISPLAY_RGB_MAX_VALUE;
            TIM4->CCR1 = (uint32_t)(duty_cycle*(double)TIM4->ARR);
            TIM4->CCER |= TIM_CCER_CC1E;
        }
        if(g==0){
            TIM4->CCER &= ~TIM_CCER_CC3E;
        }
        else{
            double duty_cycle = (double)g/(double)PORT_DISPLAY_RGB_MAX_VALUE;
            TIM4->CCR3 = (uint32_t)(duty_cycle*(double)TIM4->ARR);
            TIM4->CCER |= TIM_CCER_CC3E;
        }
        if(b==0){
            TIM4->CCER &= ~TIM_CCER_CC4E;
        }
        else{
            double duty_cycle = (double)b/(double)PORT_DISPLAY_RGB_MAX_VALUE;
            TIM4->CCR4 = (uint32_t)(duty_cycle*(double)TIM4->ARR);
            TIM4->CCER |= TIM_CCER_CC4E;
        }
        TIM4->EGR |= TIM_EGR_UG;  
        TIM4->CR1 |= TIM_CR1_CEN;
        return;
    }
}