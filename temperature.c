#include "temperature.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "display.h"
#include "font.h"
#include <stdio.h>
#include <stdlib.h>

// Variáveis globais
int simulated_temperature = 25;  // Temperatura simulada
int temperature_history[HISTORY_SIZE];  // Histórico de temperatura
int history_index = 0;  // Índice do histórico

// Função para atualizar o histórico de temperatura
void update_temperature_history(int temperature) {
    temperature_history[history_index] = temperature;
    history_index = (history_index + 1) % HISTORY_SIZE;
}

// Função para exibir o gráfico de temperatura
void display_temperature_graph(ssd1306_t *ssd) {
    ssd1306_fill(ssd, false);  // Limpa o display

    // Exibe o título "GRAF. TEMP." no topo do display
    ssd1306_draw_string(ssd, "Grafico Temp", 10, 0);

    // Desenha o eixo Y (temperatura)
    ssd1306_vline(ssd, 0, 0, SSD1306_HEIGHT - 1, true);

    // Desenha o gráfico de linha
    for (int i = 1; i < HISTORY_SIZE; i++) {
        int x1 = i - 1;
        int x2 = i;
        int y1 = SSD1306_HEIGHT - 1 - (temperature_history[(history_index + x1) % HISTORY_SIZE] / 2);
        int y2 = SSD1306_HEIGHT - 1 - (temperature_history[(history_index + x2) % HISTORY_SIZE] / 2);
        ssd1306_line(ssd, x1, y1, x2, y2, true);
    }

    ssd1306_send_data(ssd);
}

// Função para ler o eixo Y do joystick e simular a temperatura
int read_simulated_temperature() {
    adc_select_input(0);  // Seleciona o canal ADC0 (eixo Y)
    uint16_t adc_value = adc_read();  // Lê o valor do ADC
    return (adc_value / 40);  // Converte o valor do ADC para uma faixa de 0 a 100
}

// Função para atualizar o estado da placa com base na temperatura
uint8_t update_placa_state(int temperature) {
    if (temperature <= 40) {
        return STATE_NORMAL;  // Estado normal
    } else if (temperature <= 60) {
        return STATE_ALERT;  // Estado de alerta
    } else {
        return STATE_CRITICAL;  // Estado crítico
    }
}

// Função para exibir a temperatura no display
void display_temperature(int temperature) {
    char temp_str[16];
    snprintf(temp_str, sizeof(temp_str), "Temp: %dC", temperature);
    display_message(&display, "Temperatura:", temp_str);
}