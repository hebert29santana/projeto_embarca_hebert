#include "controle_buzzer.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

uint64_t buzzer_end_time = 0;
bool buzzer_silenced = false;


void start_buzzer(uint32_t duration_ms) {
    uint32_t period = 1000000 / BUZZER_FREQ_HZ;  
    uint32_t half_period = period / 2;
    uint32_t end_time = time_us_32() + (duration_ms * 1000);  

    gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    while (time_us_32() < end_time) {
        gpio_put(BUZZER_PIN, 1);
        sleep_us(half_period);
        gpio_put(BUZZER_PIN, 0);
        sleep_us(half_period);
    }

    gpio_put(BUZZER_PIN, 0); 
}

void controle_buzzer(char tecla) {
    if (tecla == '#') {
        start_buzzer(2000); 
    }
}

// Função para alternar o modo silencioso do buzzer
void toggle_buzzer_silence() {
    buzzer_silenced = !buzzer_silenced;
}
