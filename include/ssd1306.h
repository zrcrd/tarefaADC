// Inclusões de bibliotecas padrão e do Pico SDK
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Definições de dimensões do display SSD1306
#define WIDTH 128
#define HEIGHT 64

// Enumeração dos comandos do SSD1306
typedef enum {
  SET_CONTRAST = 0x81,
  SET_ENTIRE_ON = 0xA4,
  SET_NORM_INV = 0xA6,
  SET_DISP = 0xAE,
  SET_MEM_ADDR = 0x20,
  SET_COL_ADDR = 0x21,
  SET_PAGE_ADDR = 0x22,
  SET_DISP_START_LINE = 0x40,
  SET_SEG_REMAP = 0xA0,
  SET_MUX_RATIO = 0xA8,
  SET_COM_OUT_DIR = 0xC0,
  SET_DISP_OFFSET = 0xD3,
  SET_COM_PIN_CFG = 0xDA,
  SET_DISP_CLK_DIV = 0xD5,
  SET_PRECHARGE = 0xD9,
  SET_VCOM_DESEL = 0xDB,
  SET_CHARGE_PUMP = 0x8D
} ssd1306_command_t;

// Estrutura para configuração e manipulação do SSD1306
typedef struct {
  uint8_t width, height, pages, address;       // Dimensões e endereço do display
  i2c_inst_t *i2c_port;                       // Instância I2C
  bool external_vcc;                          // Indicação de alimentação externa
  uint8_t *ram_buffer;                        // Buffer de memória RAM
  size_t bufsize;                             // Tamanho do buffer
  uint8_t port_buffer[2];                     // Buffer para envio de dados via I2C
} ssd1306_t;

// Funções de inicialização e configuração do SSD1306
void ssd1306_init(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c);
void ssd1306_config(ssd1306_t *ssd);
void ssd1306_command(ssd1306_t *ssd, uint8_t command);
void ssd1306_send_data(ssd1306_t *ssd);

// Funções de manipulação de pixels e formas geométricas no display
void ssd1306_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value);
void ssd1306_fill(ssd1306_t *ssd, bool value);
void ssd1306_rect(ssd1306_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value, bool fill);
void ssd1306_line(ssd1306_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value);
void ssd1306_hline(ssd1306_t *ssd, uint8_t x0, uint8_t x1, uint8_t y, bool value);
void ssd1306_vline(ssd1306_t *ssd, uint8_t x, uint8_t y0, uint8_t y1, bool value);

// Funções de desenho de caracteres e strings no display
void ssd1306_draw_char(ssd1306_t *ssd, char c, uint8_t x, uint8_t y);
void ssd1306_draw_string(ssd1306_t *ssd, const char *str, uint8_t x, uint8_t y);
