/*
    Hebert Costa Vaz Santana
    TIC370101235
    Grupo 7

    Projeto Embarcatech
    Sistema de monitoramento de placas de sistema embarcado.
    Temperatura
    Estado de alerta
    Tensão
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "font.h"
#include "display.h"
#include "matrix.h"
#include "controle_buzzer.h"
#include "temperature.h"

// Definições de pinos
#define JOYSTICK_X_PIN 26  // Pino do eixo X do joystick (ADC0)
#define JOYSTICK_Y_PIN 27  // Pino do eixo Y do joystick (ADC1)
#define BUTTON_A_PIN 5     // Pino do botão A
#define LED_GREEN_PIN 11   // Pino do LED verde
#define LED_YELLOW_PIN 12  // Pino do LED amarelo
#define LED_RED_PIN 13     // Pino do LED vermelho

// Variáveis globais
uint8_t current_screen = 1;  // Tela atual (1: Temperatura, 2: Estado, 3: Info, 4: Gráfico Temperatura)
ssd1306_t display;

// Função para inicializar o hardware
void hardware_init() {
    // Inicializa o display OLED
    init_display(&display);

    // Inicializa o ADC para o joystick
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    // Inicializa o botão A
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    // Inicializa os LEDs
    gpio_init(LED_GREEN_PIN);
    gpio_init(LED_YELLOW_PIN);
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    gpio_set_dir(LED_YELLOW_PIN, GPIO_OUT);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);

    // Inicializa a matriz de LEDs
    npInit(LED_PIN);

    // Inicializa o buzzer
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
}

// Função para controlar os LEDs com base no estado da placa
void control_leds(uint8_t state) {
    switch (state) {
        case STATE_NORMAL:
            gpio_put(LED_GREEN_PIN, 1);
            gpio_put(LED_YELLOW_PIN, 0);
            gpio_put(LED_RED_PIN, 0);
            break;
        case STATE_ALERT:
            gpio_put(LED_GREEN_PIN, 0);
            gpio_put(LED_YELLOW_PIN, 1);
            gpio_put(LED_RED_PIN, 0);
            break;
        case STATE_CRITICAL:
            gpio_put(LED_GREEN_PIN, 0);
            gpio_put(LED_YELLOW_PIN, 0);
            gpio_put(LED_RED_PIN, 1);
            break;
    }
}

// Função para exibir o estado da placa no display
void display_placa_state(uint8_t state) {
    switch (state) {
        case STATE_NORMAL:
            display_message(&display, "Estado:", "Normal");
            break;
        case STATE_ALERT:
            display_message(&display, "Estado:", "Alerta");
            break;
        case STATE_CRITICAL:
            display_message(&display, "Estado:", "Critico");
            break;
    }
}

// Função para exibir informações adicionais no display
void display_additional_info() {
    display_message(&display, "Info:", "Tensao: 3.3V");
}

// Função principal
int main() {
    hardware_init();  // Inicializa o hardware

    while (true) {
        // Lê a temperatura simulada do joystick
        simulated_temperature = read_simulated_temperature();

        // Atualiza o histórico de temperatura
        update_temperature_history(simulated_temperature);

        // Atualiza o estado da placa
        uint8_t placa_state = update_placa_state(simulated_temperature);

        // Controla os LEDs com base no estado
        control_leds(placa_state);

        // Verifica se o botão A foi pressionado para alternar as telas
        if (!gpio_get(BUTTON_A_PIN)) {
            current_screen++;
            if (current_screen > 4) {
                current_screen = 1;
            }
            sleep_ms(300);  // Debounce do botão
        }

        // Exibe a tela atual no display
        switch (current_screen) {
            case 1:
                display_temperature(simulated_temperature);
                break;
            case 2:
                display_placa_state(placa_state);
                break;
            case 3:
                display_additional_info();
                break;
            case 4:
                display_temperature_graph(&display);
                break;
        }

        // Se o estado for crítico, pisca a matriz de LEDs e ativa o buzzer (se não estiver silenciado)
        if (placa_state == STATE_CRITICAL) {
            if (!critical_state_active) {
                critical_state_active = true;  // Entra no estado crítico
                last_blink_time = time_us_64() / 1000;  // Inicia o timer do piscar
                if (!buzzer_silenced) {
                    start_buzzer(2000);  // Ativa o buzzer por 2 segundos (se não estiver silenciado)
                    buzzer_end_time = time_us_64() / 1000 + 2000;  // Define o tempo de término do buzzer
                }
            }

            // Pisca a matriz de LEDs (não bloqueante)
            blinkMatrixRed(255);
        } else {
            critical_state_active = false;  // Sai do estado crítico
            npClear();  // Garante que a matriz de LEDs esteja apagada
        }

        // Verifica se o buzzer ainda está ativo
        if (buzzer_end_time > 0 && time_us_64() / 1000 >= buzzer_end_time) {
            buzzer_end_time = 0;  // Desativa o buzzer
        }

        sleep_ms(100);  // Pequeno delay para evitar flicker
    }
}