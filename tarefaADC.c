//  INCLUSÃO DE BIBLIOTECAS 
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"        // Funções básicas do Raspberry Pi Pico
#include "hardware/adc.h"       // Controle do ADC (Conversor Analógico-Digital)
#include "hardware/i2c.h"       // Comunicação I2C
#include "hardware/pwm.h"       // Controle de sinais PWM

//  BIBLIOTECAS DO PROJETO 
#include "include/ssd1306.h"    // Biblioteca para o display OLED SSD1306
#include "include/font.h"       // Fontes utilizadas no display
#include "define.h"            // Definições de constantes e pinos
#include "inicia.h"            // Inicialização de periféricos
#include "ledjoy.h"            // Controle de LEDs com o joystick
#include "display.h"           // Atualização do display com joystick

//  FUNÇÃO PRINCIPAL 
int main() {
    //  CONFIGURAÇÃO INICIAL 
    stdio_init_all();            // Inicializa comunicação serial padrão
    printf("Programa iniciado\n");
    inicia();                    // Configura GPIOs, ADC e outros periféricos
    inipwm();                    // Inicializa o PWM para controle de LEDs
    inii2();                     // Configura a comunicação I2C para o display OLED

    //  CONFIGURAÇÃO DE INTERRUPÇÕES 
    gpio_set_irq_enabled_with_callback(
        Botao_A,                 // Pino do botão A
        GPIO_IRQ_EDGE_FALL,      // Interrupção acionada na borda de descida
        true,                    // Habilitar a interrupção
        &gpio_irq_handler        // Função de callback associada à interrupção
    );

    //  LOOP PRINCIPAL 
    while (true) {
        ledjoy();                // Atualiza LEDs com base na posição do joystick
        quadradodisplay();       // Atualiza o display com a posição do quadrado
        sleep_ms(10);            // Pequeno atraso para evitar uso excessivo da CPU
    }
}

// FIM DO CÓDIGO
