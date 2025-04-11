/**
 * @file stm32f4_display.h
 * @brief Header for stm32f4_display.c file.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * @date 01/04/2025
 */
#ifndef STM32F4_DISPLAY_SYSTEM_H_
#define STM32F4_DISPLAY_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>

/* HW dependent includes */
#include "stm32f4xx.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define STM32F4_REAR_PARKING_DISPLAY_RGB_R_GPIO GPIOB/*!< GPIO port for the red component of the RGB LED.*/
 
#define STM32F4_REAR_PARKING_DISPLAY_RGB_R_PIN 6 /*!< GPIO pin for the red component of the RGB LED.*/
 
#define STM32F4_REAR_PARKING_DISPLAY_RGB_G_GPIO GPIOB /*!< GPIO port for the green component of the RGB LED.*/
 
#define STM32F4_REAR_PARKING_DISPLAY_RGB_G_PIN 8 /*!< GPIO pin for the green component of the RGB LED.*/
 
#define STM32F4_REAR_PARKING_DISPLAY_RGB_B_GPIO GPIOB /*!< GPIO port for the blue component of the RGB LED.*/
 
#define STM32F4_REAR_PARKING_DISPLAY_RGB_B_PIN 9 /*!< GPIO pin for the blue component of the RGB LED.*/

#endif /* STM32F4_DISPLAY_SYSTEM_H_ */