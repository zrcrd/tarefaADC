// Arquivo: define.h
// Descrição: Definições de pinos e configurações do projeto

#ifndef DEFINE_H
#define DEFINE_H

// Definições para comunicação I2C
#define I2C_PORT i2c1        // Porta I2C usada no projeto
#define I2C_SDA 14           // GPIO para o pino SDA
#define I2C_SCL 15           // GPIO para o pino SCL
#define endereco 0x3C        // Endereço I2C do display SSD1306

// Configurações do Joystick
#define JOYSTICK_X_PIN 26    // GPIO para o eixo X
#define JOYSTICK_Y_PIN 27    // GPIO para o eixo Y
#define JOYSTICK_PB 22       // GPIO para o botão do joystick

// Configuração do botão adicional
#define Botao_A 5            // GPIO para o botão A

// Definições dos LEDs RGB
#define pinR 13              // GPIO do LED vermelho
#define pinB 12              // GPIO do LED azul
#define pinG 11              // GPIO do LED verde

#endif