/**
 * @file port_display.h
 * @brief Header for the portable functions to interact with the HW of the display system. The functions must be implemented in the platform-specific code.
 * @author Marcos Perez
 * @author Jorge Lopez-Galvez
 * @date 01/04/2025
 */

#ifndef PORT_DISPLAY_SYSTEM_H_
#define PORT_DISPLAY_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>

/* Typedefs --------------------------------------------------------------------*/

/**
 * @brief Structure representing an RGB color.
 */

typedef struct {
    uint8_t r; /**< Red component of the color (0-255). */
    uint8_t g; /**< Green component of the color (0-255). */
    uint8_t b; /**< Blue component of the color (0-255). */
} rgb_color_t;

/* Defines and enums ----------------------------------------------------------*/
/* Defines */


#define PORT_REAR_PARKING_DISPLAY_ID 0 /*!<Identifier of the rear parking display*/
 
#define PORT_DISPLAY_RGB_MAX_VALUE 255 /*!<Maximum value of the RGB color component*/
 
#define COLOR_RED (rgb_color_t){255, 0, 0} /**< Predefined color: Red.*/
 
#define COLOR_GREEN (rgb_color_t){0, 255, 0} /**< Predefined color: Green.*/
 
#define COLOR_BLUE (rgb_color_t){0, 0, 255} /**< Predefined color: Blue.*/
 
#define COLOR_YELLOW (rgb_color_t){255, 255, 0} /**< Predefined color: Yellow.*/
 
#define COLOR_TURQUOISE (rgb_color_t){0, 255, 255} /**< Predefined color: Turquoise.*/
 
#define COLOR_OFF (rgb_color_t){0, 0, 0} /**< Predefined color: Off (no color).*/

/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Initializes the display system.
 * 
 * @param display_id The ID of the display to initialize.
 */

void port_display_init(uint32_t display_id);

/**
 * @brief Sets the RGB color of the specified display.
 * 
 * @param display_id The ID of the display to set the color for.
 * @param color The RGB color to set.
 *
 */
void port_display_set_rgb (uint32_t display_id, rgb_color_t color);

#endif /*PORT_DISPLAY_SYSTEM_H_*/