#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "ws2818b.pio.h" // Use a biblioteca WS2812 da pico-sdk
#include "matrix.h"

pixel_t leds[LED_COUNT];
PIO np_pio = pio0;
uint sm;

bool critical_state_active = false;
uint64_t last_blink_time = 0;
bool leds_on = false;

const uint8_t MATRIZ_LEDS[5][5] = {
    {24, 23, 22, 21, 20},
    {15, 16, 17, 18, 19},
    {14, 13, 12, 11, 10},
    {5, 6, 7, 8, 9},
    {4, 3, 2, 1, 0}
};

void npInit(uint pin)
{
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0)
    {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true);
    }
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

    for (uint i = 0; i < LED_COUNT; ++i)
    {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }
}

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b, uint8_t intensity)
{
    leds[index].R = (r * intensity) / 255;
    leds[index].G = (g * intensity) / 255;
    leds[index].B = (b * intensity) / 255;
}

void npWrite()
{
    for (uint i = 0; i < LED_COUNT; ++i)
    {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
}

void npClear()
{
    for (uint i = 0; i < LED_COUNT; ++i)
    {
        npSetLED(i, 0, 0, 0, 0);
    }
    npWrite();
}

void displayNumber(int number)
{
    npClear();
    if (number < 0 || number > 9)
        return;

    for (int row = 0; row < 5; row++)
    {
        for (int col = 0; col < 5; col++)
        {
            if (NUMBER_PATTERNS[number][row][col])
            {
                uint8_t led_index = MATRIZ_LEDS[row][col];
                npSetLED(led_index, 0, 0, 255, 80);
                sleep_us(50); // Atraso para evitar problemas de temporização
            }
        }
    }
    npWrite();
}

// Função para piscar a matriz de LEDs em vermelho (não bloqueante)
void blinkMatrixRed(uint8_t intensity) {
    uint64_t current_time = time_us_64() / 1000;  // Tempo atual em milissegundos

    // Verifica se é hora de alternar o estado dos LEDs
    if (current_time - last_blink_time >= 500) {  // 500ms de intervalo
        last_blink_time = current_time;
        leds_on = !leds_on;  // Alterna o estado dos LEDs

        if (leds_on) {
            // Acende todos os LEDs em vermelho com a intensidade especificada
            for (int row = 0; row < 5; row++) {
                for (int col = 0; col < 5; col++) {
                    uint8_t led_index = MATRIZ_LEDS[row][col];
                    npSetLED(led_index, 150, 0, 0, intensity);  // Vermelho com intensidade ajustável
                }
            }
        } else {
            // Apaga todos os LEDs
            npClear();
        }
        npWrite();
    }
}