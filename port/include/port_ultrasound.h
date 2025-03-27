/**
 * @file port_ultrasound.h
 * @brief Header for the portable functions to interact with the HW of the ultrasound sensors. The functions must be implemented in the platform-specific code.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * @date 28/03/2025
 */
#ifndef PORT_ULTRASOUND_H_
#define PORT_ULTRASOUND_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Defines and enums ----------------------------------------------------------*/

#define	PORT_REAR_PARKING_SENSOR_ID   0 /*!<Identifier of the rear parking sensor*/
#define	PORT_PARKING_SENSOR_TRIGGER_UP_US   10 /*!<Time in microseconds that the trigger signal must be up*/
#define PORT_PARKING_SENSOR_TIMEOUT_MS 100   /*!<Timeout in milliseconds to wait for the echo signal*/
#define SPEED_OF_SOUND_MS   343 /*!<Speed of sound in meters per second*/
#define TIMER_MAX_ARR 65535 /*!<Maximum value of the timer auto-reload register*/

/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Initializes the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 */                                                 
void port_ultrasound_init (uint32_t ultrasound_id);

/**
 * @brief Starts the trigger signal of the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 */
void port_ultrasound_start_measurement (uint32_t ultrasound_id);

/**
 * @brief Stops the trigger signal of the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 */
void port_ultrasound_stop_trigger_timer (uint32_t ultrasound_id);

/**
 * @brief Starts the echo signal of the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 */
void port_ultrasound_stop_echo_timer (uint32_t ultrasound_id);

/**
 * @brief Starts the timer to start a new measurement of the ultrasound sensor.
 */
void port_ultrasound_start_new_measurement_timer (void);

/**
 * @brief Stops the timer to start a new measurement of the ultrasound sensor.
 */
void port_ultrasound_stop_new_measurement_timer (void);

/**
 * @brief Resets the number of ticks of the timer.
 */
void port_ultrasound_reset_echo_ticks (uint32_t ultrasound_id);

/**
 * @brief Stops the ultrasound sensor with the specified identifier.
 */
void port_ultrasound_stop_ultrasound (uint32_t ultrasound_id);

/**
 * @brief Returns the value of the field trigger ready from the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 *
 * @retval Value of trigger ready.
 */
bool port_ultrasound_get_trigger_ready (uint32_t ultrasound_id);

/**
 * @brief Sets the value of the field trigger ready from the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 * @param trigger_ready 
 */
void port_ultrasound_set_trigger_ready (uint32_t ultrasound_id, bool trigger_ready);

/**
 * @brief Returns the value of the field trigger end from the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 *
 * @retval Value of trigger end.
 */
bool port_ultrasound_get_trigger_end (uint32_t ultrasound_id);

/**
 * @brief Sets the value of the field trigger end from the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 * @param trigger_end 
 */
void port_ultrasound_set_trigger_end (uint32_t ultrasound_id, bool trigger_end);

/**
 * @brief Returns the value of the field echo init tick from the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 *
 * @retval Value of echo init tick.
 */
uint32_t port_ultrasound_get_echo_init_tick (uint32_t ultrasound_id);

/**
 * @brief Sets the value of the field echo init tick from the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 * @param echo_init_tick
 */
void port_ultrasound_set_echo_init_tick (uint32_t ultrasound_id, uint32_t echo_init_tick);

/**
 * @brief Returns the value of the field echo end tick from the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 *
 * @retval Value of echo end tick.
 */
uint32_t port_ultrasound_get_echo_end_tick (uint32_t ultrasound_id);

/**
 * @brief Sets the value of the field echo end tick from the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 * @param echo_end_tick
 */
void port_ultrasound_set_echo_end_tick (uint32_t ultrasound_id, uint32_t echo_end_tick);

/**
 * @brief Returns the value of the field echo received from the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id
 *
 * @retval Value of echo received.
 */
bool port_ultrasound_get_echo_received (uint32_t ultrasound_id);

/**
 * @brief Sets the value of the field echo received from the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 * @param echo_received 
 */
void port_ultrasound_set_echo_received (uint32_t ultrasound_id, bool echo_received);

/**
 * @brief Returns the value of the field echo overflows from the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 *
 * @retval Value of echo overflows.
 */
uint32_t port_ultrasound_get_echo_overflows (uint32_t ultrasound_id);

/**
 * @brief Sets the value of the field echo overflows from the ultrasound sensor with the specified identifier.
 *
 * @param ultrasound_id 
 * @param echo_overflows 
 */
void port_ultrasound_set_echo_overflows (uint32_t ultrasound_id, uint32_t echo_overflows);


#endif /* PORT_ULTRASOUND_H_ */
