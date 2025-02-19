// Proteção contra múltiplas inclusões
#ifndef LEDJOY_H
#define LEDJOY_H

// Inicializa o PWM
void inipwm();

// Controla os LEDs via joystick
void ledjoy();

// Manipulador de interrupções GPIO
void gpio_irq_handler(uint gpio, uint32_t events);

#endif
