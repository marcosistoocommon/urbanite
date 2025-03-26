/**
 * @file port_button.h
 * @brief Header for the portable functions to interact with the HW of the buttons. The functions must be implemented in the platform-specific code.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * @date 28/03/2025
 */

#ifndef PORT_BUTTON_H_
#define PORT_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
// Define here all the button identifiers that are used in the system
#define PORT_PARKING_BUTTON_ID 0 /*!<Identifier of the parking button*/
#define PORT_PARKING_BUTTON_DEBOUNCE_TIME_MS 100 /*!<Debounce time in milliseconds*/
/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Initializes the button with the specified identifier.
 *
 * @param button_id Identifier of the button to initialize.
 */
void port_button_init (uint32_t button_id);


/**
 * @brief Returns the state of the button with the specified identifier.
 *
 * @param button_id Identifier of the button to get the state.
 *
 * @retval true if the button is pressed.
 * @retval false if the button is not pressed.
 */
bool port_button_get_pressed (uint32_t button_id);


/**
 * @brief Returns the value of the button with the specified identifier.
 *
 * @param button_id Identifier of the button to get the value.
 *
 * @retval true if the button is pressed.
 * @retval false if the button is not pressed.
 */
bool port_button_get_value (uint32_t button_id);


/**
 * @brief Sets the value of the button with the specified identifier.
 *
 * @param button_id Identifier of the button to set the value.
 * @param pressed Value to set.
 */
void port_button_set_pressed (uint32_t button_id, bool pressed);

/**
 * @brief Enables the interrupts of the button with the specified identifier.
 *
 * @param button_id Identifier of the button to enable the interrupts.
 */
bool port_button_get_pending_interrupt (uint32_t button_id);


/**
 * @brief Clears the pending interrupts of the button with the specified identifier.
 *
 * @param button_id Identifier of the button to clear the pending interrupts.
 */
void port_button_clear_pending_interrupt (uint32_t button_id);


/**
 * @brief Enables the interrupts of the button with the specified identifier.
 *
 * @param button_id Identifier of the button to enable the interrupts.
 */
void port_button_disable_interrupts (uint32_t button_id);

#endif