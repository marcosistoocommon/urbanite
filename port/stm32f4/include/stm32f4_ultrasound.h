/**
 * @file stm32f4_ultrasound.h
 * @brief Header for stm32f4_ultrasound.c file.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * @date 28/03/2025
 */
#ifndef STM32F4_ULTRASOUND_H_
#define STM32F4_ULTRASOUND_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include "stm32f4xx.h"

/* HW dependent includes */


/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define STM32F4_REAR_PARKING_SENSOR_TRIGGER_GPIO GPIOB /*!< GPIO port for the trigger signal of the rear parking sensor.*/
#define STM32F4_REAR_PARKING_SENSOR_TRIGGER_PIN 0 /*!< GPIO pin for the trigger signal of the rear parking sensor.*/
#define STM32F4_REAR_PARKING_SENSOR_ECHO_GPIO GPIOA /*!< GPIO port for the echo signal of the rear parking sensor.*/
#define STM32F4_REAR_PARKING_SENSOR_ECHO_PIN 1 /*!< GPIO pin for the echo signal of the rear parking sensor.*/
/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Auxiliary function to change the GPIO and pin of the trigger pin of an ultrasound transceiver. This function is used for testing purposes mainly although it can be used in the final implementation if needed.
 *
 * @param ultrasound_id ID of the ultrasound sensor.
 * @param p_port GPIO port where the trigger signal is connected.
 * @param pin Pin where the trigger signal is connected.
 *
 */
void stm32f4_ultrasound_set_new_trigger_gpio(uint32_t ultrasound_id, GPIO_TypeDef *p_port, uint8_t pin);

/**
 * @brief Auxiliary function to change the GPIO and pin of the echo pin of an ultrasound transceiver. This function is used for testing purposes mainly although it can be used in the final implementation if needed.
 *
 * @param ultrasound_id ID of the ultrasound sensor.
 * @param p_port GPIO port where the echo signal is connected.
 * @param pin Pin where the echo signal is connected.
 *
 */
void stm32f4_ultrasound_set_new_echo_gpio(uint32_t ultrasound_id, GPIO_TypeDef *p_port, uint8_t pin);


#endif /* STM32F4_ULTRASOUND_H_ */
