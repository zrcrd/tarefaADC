#include "ssd1306.h"
#include "font.h"

// Inicializa a estrutura do display SSD1306
void ssd1306_init(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c) {
    ssd->width = width;
    ssd->height = height;
    ssd->pages = height / 8U; // Número de páginas de memória
    ssd->address = address;
    ssd->i2c_port = i2c;
    ssd->bufsize = ssd->pages * ssd->width + 1; // Tamanho do buffer
    ssd->ram_buffer = calloc(ssd->bufsize, sizeof(uint8_t));
    ssd->ram_buffer[0] = 0x40; // Prefixo para dados gráficos
    ssd->port_buffer[0] = 0x80; // Prefixo para comandos
}

// Configuração inicial do display com os comandos necessários
void ssd1306_config(ssd1306_t *ssd) {
    ssd1306_command(ssd, SET_DISP | 0x00);          // Desliga o display
    ssd1306_command(ssd, SET_MEM_ADDR);            // Define o modo de endereçamento
    ssd1306_command(ssd, 0x01);                    // Endereçamento horizontal
    ssd1306_command(ssd, SET_DISP_START_LINE | 0x00);
    ssd1306_command(ssd, SET_SEG_REMAP | 0x01);    // Mapeamento dos segmentos
    ssd1306_command(ssd, SET_MUX_RATIO);
    ssd1306_command(ssd, HEIGHT - 1);              // Multiplexação
    ssd1306_command(ssd, SET_COM_OUT_DIR | 0x08);
    ssd1306_command(ssd, SET_DISP_OFFSET);
    ssd1306_command(ssd, 0x00);                    // Sem deslocamento
    ssd1306_command(ssd, SET_COM_PIN_CFG);
    ssd1306_command(ssd, 0x12);                    // Configuração das conexões COM
    ssd1306_command(ssd, SET_DISP_CLK_DIV);
    ssd1306_command(ssd, 0x80);                    // Frequência do clock
    ssd1306_command(ssd, SET_PRECHARGE);
    ssd1306_command(ssd, 0xF1);                    // Período de pré-carga
    ssd1306_command(ssd, SET_VCOM_DESEL);
    ssd1306_command(ssd, 0x30);                    // Nível de deselect de VCOM
    ssd1306_command(ssd, SET_CONTRAST);
    ssd1306_command(ssd, 0xFF);                    // Contraste máximo
    ssd1306_command(ssd, SET_ENTIRE_ON);           // Habilita pixels normais
    ssd1306_command(ssd, SET_NORM_INV);            // Normaliza a exibição
    ssd1306_command(ssd, SET_CHARGE_PUMP);
    ssd1306_command(ssd, 0x14);                    // Habilita a bomba de carga
    ssd1306_command(ssd, SET_DISP | 0x01);         // Liga o display
}

// Envia um comando ao display
void ssd1306_command(ssd1306_t *ssd, uint8_t command) {
    ssd->port_buffer[1] = command;
    i2c_write_blocking(ssd->i2c_port, ssd->address, ssd->port_buffer, 2, false);
}

// Atualiza os dados gráficos no display
void ssd1306_send_data(ssd1306_t *ssd) {
    ssd1306_command(ssd, SET_COL_ADDR);
    ssd1306_command(ssd, 0);                        // Início da coluna
    ssd1306_command(ssd, ssd->width - 1);           // Fim da coluna
    ssd1306_command(ssd, SET_PAGE_ADDR);
    ssd1306_command(ssd, 0);                        // Início da página
    ssd1306_command(ssd, ssd->pages - 1);           // Fim da página
    i2c_write_blocking(ssd->i2c_port, ssd->address, ssd->ram_buffer, ssd->bufsize, false);
}

// Define o estado de um pixel no buffer
void ssd1306_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value) {
    uint16_t index = (y >> 3) + (x << 3) + 1; // Calcula o índice no buffer
    uint8_t pixel = (y & 0b111);              // Determina o bit do pixel

    if (value)
        ssd->ram_buffer[index] |= (1 << pixel);
    else
        ssd->ram_buffer[index] &= ~(1 << pixel);
}

// Preenche o display com um valor (on/off)
void ssd1306_fill(ssd1306_t *ssd, bool value) {
    for (uint8_t y = 0; y < ssd->height; ++y) {
        for (uint8_t x = 0; x < ssd->width; ++x) {
            ssd1306_pixel(ssd, x, y, value);
        }
    }
}

// Desenha um retângulo no display
void ssd1306_rect(ssd1306_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value, bool fill) {
    for (uint8_t x = left; x < left + width; ++x) {
        ssd1306_pixel(ssd, x, top, value);                  // Linha superior
        ssd1306_pixel(ssd, x, top + height - 1, value);     // Linha inferior
    }

    for (uint8_t y = top; y < top + height; ++y) {
        ssd1306_pixel(ssd, left, y, value);                 // Linha esquerda
        ssd1306_pixel(ssd, left + width - 1, y, value);     // Linha direita
    }

    if (fill) {
        for (uint8_t x = left + 1; x < left + width - 1; ++x) {
            for (uint8_t y = top + 1; y < top + height - 1; ++y) {
                ssd1306_pixel(ssd, x, y, value);
            }
        }
    }
}

// Desenha uma linha usando o algoritmo de Bresenham
void ssd1306_line(ssd1306_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        ssd1306_pixel(ssd, x0, y0, value); // Marca o pixel atual

        if (x0 == x1 && y0 == y1) break;   // Chegou ao destino

        int e2 = err * 2;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

// Desenha uma linha horizontal
void ssd1306_hline(ssd1306_t *ssd, uint8_t x0, uint8_t x1, uint8_t y, bool value) {
    for (uint8_t x = x0; x <= x1; ++x) {
        ssd1306_pixel(ssd, x, y, value);
    }
}

// Desenha uma linha vertical
void ssd1306_vline(ssd1306_t *ssd, uint8_t x, uint8_t y0, uint8_t y1, bool value) {
    for (uint8_t y = y0; y <= y1; ++y) {
        ssd1306_pixel(ssd, x, y, value);
    }
}

// Desenha um caractere individual na posição especificada
void ssd1306_draw_char(ssd1306_t *ssd, char character, uint8_t x, uint8_t y) {
    uint16_t index = 0;

    if (character >= 'A' && character <= 'Z') {
        index = (character - 'A' + 11) * 8; // Letras maiúsculas
    } else if (character >= 'a' && character <= 'z') {
        index = (character - 'a' + 37) * 8; // Letras minúsculas
    } else if (character >= '0' && character <= '9') {
        index = (character - '0' + 1) * 8;  // Números
    }

    for (uint8_t i = 0; i < 8; ++i) {
        uint8_t line = font[index + i];
        for (uint8_t j = 0; j < 8; ++j) {
            ssd1306_pixel(ssd, x + i, y + j, line & (1 << j));
        }
    }
}

// Desenha uma string completa no display
void ssd1306_draw_string(ssd1306_t *ssd, const char *str, uint8_t x, uint8_t y) {
    while (*str) {
        ssd1306_draw_char(ssd, *str++, x, y);
        x += 8; // Avança para a próxima posição

        if (x + 8 >= ssd->width) {
            x = 0;
            y += 8;
        }

        if (y + 8 >= ssd->height) {
            break; // Evita sair da área visível
        }
    }
}