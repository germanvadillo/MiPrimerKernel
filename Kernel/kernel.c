#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

enum vga_color {VGA_COLOR_BLACK = 0, VGA_COLOR_BLUE = 1, VGA_COLOR_GREEN = 2,VGA_COLOR_CYAN = 3,
VGA_COLOR_RED = 4, VGA_COLOR_MAGENTA = 5, VGA_COLOR_BROWN = 6, VGA_COLOR_LIGHT_GREY = 7, VGA_COLOR_DARK_GREY = 8,
VGA_COLOR_LIGHT_BLUE = 9, VGA_COLOR_LIGHT_GREEN = 10, VGA_COLOR_LIGHT_CYAN = 11, VGA_COLOR_LIGHT_RED = 12,
VGA_COLOR_LIGHT_MAGENTA = 13, VGA_COLOR_LIGHT_BROWN = 14, VGA_COLOR_WHITE = 15,
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
volatile uint16_t* terminal_buffer;


uint8_t crear_color(enum vga_color texto, enum vga_color fondo) {
    return (texto | (fondo << 4));
}

uint16_t crear_caracter_vga(unsigned char caracter, uint8_t color){
    return (uint16_t) caracter | (uint16_t) color << 8;
}

void terminal_iniciar(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = crear_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;

    for (size_t y = 0; y < VGA_HEIGHT; y++){
        for (size_t x = 0; x < VGA_WIDTH; x++){
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = crear_caracter_vga(' ', terminal_color);
        }
    }
}

void terminal_borrar(void){
    terminal_row = 0;
    terminal_column = 0;
    for (size_t y = 0; y < VGA_HEIGHT; y++){
        for (size_t x = 0; x < VGA_WIDTH; x++){
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = crear_caracter_vga(' ', terminal_color);
        }
    }
}

void terminal_escribir_entero(int numero) {
    char buffer[12];
    char bufferev[12];
    int i = 0;
    bool es_negativo = false;
    if (numero == 0){
        terminal_escribir("0");
        return;
    }

    if (numero < 0){
        es_negativo = true;
        numero = -numero;
    }

    while (numero > 0){
        int digito = numero % 10;
        buffer[i] = digito + '0';
        i++;
        numero = numero / 10;
    }

    if (es_negativo){
        buffer[i] = '-'
        i++
    }

    
}

void terminal_scrolling(void){
    for (size_t y = 1; y < VGA_HEIGHT; y++){
        for (size_t x = 0;x < VGA_WIDTH; x++){
            const size_t index_actual = y * VGA_WIDTH + x;
            const size_t index_arriba = (y - 1) * VGA_WIDTH + x;

            terminal_buffer[index_arriba] = terminal_buffer[index_actual];
        }
    }

    for (size_t x = 0; x < VGA_WIDTH; x++){
        const size_t index_ultima_linea = (VGA_HEIGHT -1) * VGA_WIDTH + x;
        terminal_buffer[index_ultima_linea] = crear_caracter_vga(' ', terminal_color);
    }
    terminal_row = VGA_HEIGHT - 1;
}

void terminal_escribir(const char* texto){
    size_t i = 0;
    while (texto[i] != '\0'){
        if (texto[i] == '\n'){
            terminal_row++;
            terminal_column = 0;
        }
        else {const size_t index = terminal_row * VGA_WIDTH + terminal_column;
        terminal_buffer[index] = crear_caracter_vga(texto[i], terminal_color);
        terminal_column++;
        if (terminal_column == VGA_WIDTH){
            terminal_column = 0;
            terminal_row++;
        }
    }

    if (terminal_row == VGA_HEIGHT){
        terminal_scrolling();
    }
        i++;
    }
}

void wait(uint32_t ciclos){
    for (uint32_t n = 0; n < ciclos; n++){
        __asm__ volatile("nop");
    }
}

void kernel_main(void){

    terminal_iniciar();
    for (uint32_t i = 0; i < 50; i++){
        terminal_escribir("mensaje de prueba 44444444444444444444444 \n");
        wait(1800000000);
        terminal_escribir("mensaje de prueba 33333333333333333333333 \n");
        wait(1800000000);
        terminal_escribir("mensaje de prueba 55555555555555555555555 \n");
        wait(1800000000);
        terminal_escribir("mensaje de prueba 99999999999999999999999 \n");
        wait(1800000000);
        terminal_escribir("mensaje de prueba 77777777777777777777777 \n");
        wait(1800000000);
        terminal_escribir("mensaje de prueba 66666666666666666666666 \n");
        wait(1800000000);
        terminal_escribir("mensaje de prueba 22222222222222222222222 \n");
        wait(1800000000);
        terminal_escribir("mensaje de prueba 00000000000000000000000 \n");
        wait(1800000000);
    }
}