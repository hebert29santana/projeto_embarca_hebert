#ifndef CONTROLE_BUZZER_H
#define CONTROLE_BUZZER_H

#include "pico/stdlib.h"

#define BUZZER_PIN 21
#define BUZZER_FREQ_HZ 3200

// Variável global para controlar o modo silencioso do buzzer
extern bool buzzer_silenced;
extern uint64_t buzzer_end_time;

void start_buzzer(uint32_t duration_ms);
void controle_buzzer(char tecla);
void toggle_buzzer_silence();

#endif
