/**
 * @file stm32f4_button.h
 * @brief Header for stm32f4_button.c file.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * @date 28/03/2025
 */
#ifndef STM32F4_BUTTON_H_
#define STM32F4_BUTTON_H_
/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>

/* HW dependent includes */
#include "stm32f4xx.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define STM32F4_PARKING_BUTTON_GPIO GPIOC /*!< GPIO port for the parking button.*/
#define STM32F4_PARKING_BUTTON_PIN 13 /*!< GPIO pin for the parking button.*/



/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Auxiliary function to change the GPIO and pin of a button. This function is used for testing purposes mainly although it can be used in the final implementation if needed.
 *
 * @param button_id ID of the button to change.
 * @param p_port New GPIO port for the button.
 * @param pin New GPIO pin for the button.
 *
 */
void stm32f4_button_set_new_gpio(uint32_t button_id, GPIO_TypeDef *p_port, uint8_t pin);

#endif /* STM32F4_BUTTON_H_ */