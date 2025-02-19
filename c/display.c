#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "include/ssd1306.h"
#include "include/font.h"
#include "define.h"

// Estrutura do display OLED
ssd1306_t ssd;

// Variável para o estado do botão (toggle entre modos)
bool estado_pb = false;

// Função para inicializar o barramento I2C e o display OLED
void inii2() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

// Função para controlar o quadrado no display usando o joystick
void quadradodisplay() {
    // Leitura do joystick nos eixos X e Y
    adc_select_input(0);
    uint16_t vrx_value = adc_read();
    adc_select_input(1);
    uint16_t vry_value = adc_read();

    // Conversão dos valores do joystick para coordenadas no display
    int pos_x = 60 + ((vry_value - 2048) * 60) / 2048;
    int pos_y = 28 - ((vrx_value - 2048) * 28) / 2048;

    // Ajuste para manter o quadrado dentro da área visível
    if (pos_x < 8) pos_x = 8;
    if (pos_x > 112) pos_x = 112;
    if (pos_y < 8) pos_y = 8;
    if (pos_y > 48) pos_y = 48;

    // Controle do botão para alternar modos
    static bool botao_ultimo_estado = false;
    bool botao_atual = !gpio_get(JOYSTICK_PB); // Botão é PULL-UP (0 quando pressionado)

    if (botao_atual && !botao_ultimo_estado) {
        estado_pb = !estado_pb; // Alterna o estado do botão
        printf("Botão pressionado! Novo estado: %d\n", estado_pb);
    }
    botao_ultimo_estado = botao_atual;

    // Atualização do display
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false); // Desenha a borda externa

    if (estado_pb) {
        gpio_put(pinG, true); // Acende LED verde

        // Calcula nova posição no modo alternativo
        int pos_xPB = 59 + ((vry_value - 2048) * 60) / 2048;
        int pos_yPB = 27 - ((vrx_value - 2048) * 28) / 2048;

        // Limita os valores no modo alternativo
        if (pos_xPB < 8) pos_xPB = 8;
        if (pos_xPB > 112) pos_xPB = 112;
        if (pos_yPB < 8) pos_yPB = 8;
        if (pos_yPB > 48) pos_yPB = 48;

        ssd1306_draw_string(&ssd, "Z", pos_xPB, pos_yPB);
        ssd1306_rect(&ssd, 5, 5, 118, 54, true, false); // Borda interna adicional
    } else {
        gpio_put(pinG, false); // Apaga LED verde
        ssd1306_draw_string(&ssd, "Z", pos_x, pos_y); // Desenha o quadrado padrão
    }

    ssd1306_send_data(&ssd); // Envia os dados para o display
}
