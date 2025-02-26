#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "pico/stdlib.h"
#include "display.h"

// Definições
#define HISTORY_SIZE 128

// Definições do estado da placa
#define STATE_NORMAL 0
#define STATE_ALERT 1
#define STATE_CRITICAL 2

// Variáveis globais
extern int simulated_temperature;
extern int temperature_history[HISTORY_SIZE];
extern int history_index;

// Funções
void update_temperature_history(int temperature);
void display_temperature_graph(ssd1306_t *ssd);
int read_simulated_temperature();
uint8_t update_placa_state(int temperature);
void display_temperature(int temperature);

#endif // TEMPERATURE_H