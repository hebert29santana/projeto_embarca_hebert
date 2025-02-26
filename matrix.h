#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

#define LED_PIN 7
#define LED_COUNT 25

// Variáveis para controlar o estado crítico
extern bool critical_state_active;
extern uint64_t last_blink_time;
extern bool leds_on;

typedef struct pixel_t {
    uint8_t G, R, B;
} pixel_t;

extern const uint8_t MATRIZ_LEDS[5][5];
extern const uint8_t NUMBER_PATTERNS[10][5][5];

void npInit(uint pin);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b, uint8_t intensity);
void npWrite();
void npClear();
void displayNumber(int number);
void blinkMatrixRedNonBlocking(uint8_t intensity);  // Nova função para piscar a matriz em vermelho

#endif // MATRIX_H
