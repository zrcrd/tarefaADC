#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "define.h"
#include "inicia.h"
#include "hardware/pwm.h"

// Configura o GPIO como saída PWM e ativa o módulo correspondente
uint pwm_init_gpio(uint gpio, uint wrap) {
    gpio_set_function(gpio, GPIO_FUNC_PWM); // Ativa a função PWM no GPIO
    uint slice_num = pwm_gpio_to_slice_num(gpio); // Identifica o slice PWM associado
    pwm_set_wrap(slice_num, wrap); // Define o valor limite do contador PWM
    pwm_set_enabled(slice_num, true); // Habilita o PWM no slice configurado
    return slice_num;
}

// Inicializa o PWM para os LEDs Azul e Vermelho
void inipwm() {
    const uint pwm_wrap = 4096; // Resolução PWM de 12 bits (0 a 4095)
    pwm_init_gpio(pinB, pwm_wrap); // Configura PWM para o LED Azul
    pwm_init_gpio(pinR, pwm_wrap); // Configura PWM para o LED Vermelho
}

// Variável que controla o estado dos LEDs (true = ligados, false = desligados)
bool estadoLeds = true;

// Interrupção associada ao Botão A para alternar o estado dos LEDs
void gpio_irq_handler(uint gpio, uint32_t events) {
    static volatile uint32_t last_timeA = 0; // Timestamp do último evento
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se a interrupção é do Botão A e aplica debounce
    if (gpio == Botao_A && (current_time - last_timeA > 200000)) {
        last_timeA = current_time; // Atualiza o timestamp

        estadoLeds = !estadoLeds; // Alterna o estado dos LEDs

        if (estadoLeds) {
            printf("LEDs ligados\n");
        } else {
            printf("LEDs desligados\n");
            pwm_set_gpio_level(pinB, 0); // Apaga o LED Azul
            pwm_set_gpio_level(pinR, 0); // Apaga o LED Vermelho
        }
    }
}

// Controla os LEDs com base nos valores do joystick
void ledjoy() {
    // Leitura do eixo X do joystick
    adc_select_input(0);
    uint16_t vrx_value = adc_read();

    // Leitura do eixo Y do joystick
    adc_select_input(1);
    uint16_t vry_value = adc_read();

    // Ajusta o brilho dos LEDs apenas se estiverem ligados
    if (estadoLeds) {
        // Calcula o nível PWM para o LED Azul
        uint16_t pwm_level_azul = abs(vry_value - 1892) * 2;
        pwm_set_gpio_level(pinR, pwm_level_azul > 500 ? pwm_level_azul : 0); // Evita brilho fraco

        // Calcula o nível PWM para o LED Vermelho
        uint16_t pwm_level_vermelho = abs(vrx_value - 1876) * 2;
        pwm_set_gpio_level(pinB, pwm_level_vermelho > 500 ? pwm_level_vermelho : 0); // Evita brilho fraco
    }
}