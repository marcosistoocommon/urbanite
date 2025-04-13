# UltRasonic-Based pArkiNg aId sysTEm

## Authors

* **Jorge Lopez-Galvez** - email: [jorge.lopez-galvez@alumnos.upm.es](mailto:jorge.lopez-galvez@alumnos.upm.es)
* **Marcos Perez** - email: [marcos.perez@alumnos.upm.es](mailto:marcos.perez@alumnos.upm.es)

El proyecto consiste en un sistema de ayuda al aparcamiento mediante un transceptor de ultrasonidos y un LED. Con el transceptor de ultrasonidos se medirá la distancia a los obstáculos, y el LED cambiará de color en función de la distancia medida. Por el momento hemos desarrollado el botón de encendido (no la gestión de consumo, correspondiente a la V4), y el medidor de distancia mediante el transceptor de ultrasonidos.

The project consists of a parking assistance system using an ultrasonic transceiver and an LED. The ultrasonic transceiver will measure the distance to obstacles, and the LED will change color depending on the measured distance. At the moment we have developed the power button (not the consumption management, corresponding to V4), and the distance meter using the ultrasonic transceiver.

## Version 1
En esta versión desarrollamos el funcionamiento del botón que usaremos en la V4 para la gestión de consumo, además de programar unas funciones generales que nos serán útiles para el resto del proyecto.

**Funciones generales**

Las funciones que hemos programado antes de empezar con el botón son:

*Temporización*
* port_system_delay_ms()
* port_system_delay_until_ms()
* port_system_get_millis()
* port_system_set_millis()
* SysTick_Handler()

*Gestion de GPIOs*
* stm32f4_system_gpio_read()
* stm32f4_system_gpio_write()
* stm32f4_system_gpio_toggle()


**Hardware**

Las funciones relacionadas con el hardware del botón que hemos programado son:

*stm32f4_button.c*
* _stm32f4_button_get()
* port_button_init()
* stm32f4_button_set_new_gpio()
* port_button_get_pressed()
* port_button_get_value()
* port_button_set_pressed()
* port_button_get_pending_interrupt()
* port_button_clear_pending_interrupt()
* port_button_disable_interrupts()

*interr.c*
* EXTI15_10_IRQHandler()

**FSM**

Las funciones relacionadas con la FSM del botón que hemos programado son:

*fsm_button.c*
* check_button_pressed()
* check_button_released()
* check_timeout()
* do_store_tick_pressed()
* do_set_duration()
* fsm_button_init()
* fsm_button_new()
* fsm_button_fire()
* fsm_button_destroy()
* fsm_button_get_inner_fsm()
* fsm_button_get_state()
* fsm_button_get_duration()
* fsm_button_reset_duration()
* fsm_button_get_debounce_time_ms()


## Version 2

En esta versión desarrollamos el funcionamiento del transceptor de ultrasonidos que usaremos para medir distancias.

**Hardware**

Las funciones relacionadas con el hardware del transceptor que hemos programado son:

*stm32f4_ultrasound.c*
* _timer_trigger_setup() 
* _timer_echo_setup() 
* _timer_new_measurement_setup()
* _stm32f4_ultrasound_get() 
* port_ultrasound_init() 
* port_ultrasound_stop_trigger_timer() 
* port_ultrasound_get_trigger_end() 
* port_ultrasound_get_trigger_ready() 
* port_ultrasound_set_trigger_end()
* port_ultrasound_set_trigger_ready() 
* port_ultrasound_get_echo_end_tick()
* port_ultrasound_get_echo_init_tick() 
* port_ultrasound_get_echo_overflows() 
* port_ultrasound_get_echo_received() 
* port_ultrasound_reset_echo_ticks() 
* port_ultrasound_set_echo_end_tick() 
* port_ultrasound_set_echo_init_tick() 
* port_ultrasound_set_echo_overflows() 
* port_ultrasound_set_echo_received() 
* port_ultrasound_start_measurement() 
* port_ultrasound_start_new_measurement_timer() 
* port_ultrasound_stop_echo_timer()
* port_ultrasound_stop_new_measurement_timer()
* port_ultrasound_stop_ultrasound()
* stm32f4_ultrasound_set_new_trigger_gpio()
* stm32f4_ultrasound_set_new_echo_gpio()

*interr.c*
* TIM3_IRQHandler() 
* TIM2_IRQHandler() 
* TIM5_IRQHandler()

**Software**

Las funciones relacionadas con la FSM del transceptor que hemos programado son:

*fsm_ultrasound.c*
* check_on()
* check_off()
* check_trigger_end()
* check_echo_init()
* check_echo_received()
* check_new_measurement()
* do_start_measurement()
* do_start_new_measurement()
* do_stop_trigger()
* do_set_distance()
* do_stop_measurement()
* fsm_ultrasound_init()
* fsm_ultrasound_new()
* fsm_ultrasound_destroy()
* fsm_ultrasound_fire()
* fsm_ultrasound_get_distance()
* fsm_ultrasound_stop()
* fsm_ultrasound_start()
* fsm_ultrasound_get_status()
* fsm_ultrasound_set_status()
* fsm_ultrasound_get_ready()
* fsm_ultrasound_get_new_measurement_ready()
* fsm_ultrasound_get_inner_fsm()
* fsm_ultrasound_get_state()
* fsm_ultrasound_set_state()
* fsm_ultrasound_check_activity()

## Version 3

En esta versión desarrollamos el funcionamiento de un LED para dar una respuesta en función de la distancia medida

**Hardware**

Las funciones relacionadas con el hardware del LED que hemos programado son:

*stm32f4_display.c*
* _stm32f4_display_get()
* _timer_pwm_config()
* port_display_init()
* port_display_set_rgb()

**Software**

Las funciones relacionadas con la FSM del LED que hemos programado son:

*fsm_display.c*
* _compute_display_levels()
* check_set_new_color()
* check_active()
* check_off()
* do_set_on()
* do_set_color()
* do_set_off()
* fsm_display_init()
* fsm_display_new()
* fsm_display_destroy()
* fsm_display_fire()
* fsm_display_get_distance()
* fsm_display_set_distance()
* fsm_display_get_status()
* fsm_display_set_status()
* fsm_display_check_activity()
* fsm_display_get_inner_fsm()
* fsm_display_get_state()
* fsm_display_set_state()

## Version 4

En desarrollo

## Version 5

En desarrollo
