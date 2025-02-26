Hebert Costa Vaz Santana
TIC370101235
Grupo 7

# Sistema Embarcado de Monitoramento de Temperatura com Feedback Visual e Sonoro

Este projeto consiste em um sistema embarcado para monitoramento de temperatura, desenvolvido utilizando a placa **BitDogLab** (baseada no microcontrolador **Raspberry Pi Pico W**). O sistema simula a leitura de temperatura através de um joystick, exibe a temperatura atual e o estado da placa em um display LCD, e fornece feedback visual (matriz de LEDs) e sonoro (buzzer) em caso de estado crítico. Além disso, o sistema exibe um gráfico de temperatura ao longo do tempo, permitindo uma análise visual do comportamento térmico.

---

## Link para vídeo demontrativo
Link: https://drive.google.com/file/d/11-o1yJyh-BIDbyVj--Uw2ctQGhRHkrQu/view?usp=sharing

---

## Funcionalidades

- **Leitura de Temperatura Simulada:** O joystick é utilizado para simular a leitura de temperatura, com valores convertidos para uma faixa de 0°C a 100°C.
- **Exibição no Display LCD:** O sistema exibe a temperatura atual, o estado da placa (normal, alerta ou crítico) e um gráfico de temperatura.
- **Feedback Visual e Sonoro:** Em caso de estado crítico, a matriz de LEDs pisca em vermelho e o buzzer emite um sinal sonoro.
- **Alternância de Telas:** O botão A permite alternar entre as telas no display LCD (temperatura, estado, gráfico e informações adicionais).

---

## Componentes Utilizados

- **Placa BitDogLab (Raspberry Pi Pico W):**
  - Microcontrolador RP2040 (dual-core ARM Cortex-M0+).
  - 264 KB de SRAM e suporte para até 16 MB de memória flash externa.
- **Display LCD (128x64 pixels):**
  - Interface I2C para exibição de texto e gráficos.
- **Matriz de LEDs 5x5:**
  - Feedback visual em caso de estado crítico.
- **Buzzer:**
  - Sinal sonoro de alerta em caso de estado crítico.
- **Joystick Analógico:**
  - Simulação da leitura de temperatura.

---

## Como Configurar o Projeto

### Pré-requisitos

- Placa BitDogLab (Raspberry Pi Pico W).
- Display LCD 128x64 (compatível com I2C).
- Matriz de LEDs 5x5.
- Buzzer.
- Joystick analógico.
- Ambiente de desenvolvimento configurado para programação em C (ex: VS Code com extensão Pico SDK).

### Conexões

| Componente       | Pino na BitDogLab |
|------------------|-------------------|
| Display LCD SDA | GPIO 14 (I2C SDA) |
| Display LCD SCL | GPIO 15 (I2C SCL) |
| Matriz de LEDs   | GPIO 7            |
| Buzzer           | GPIO 21           |
| Joystick (Eixo Y)| GPIO 26 (ADC0)    |
| Botão A          | GPIO 5            |

---

## Fluxograma

![Diagrama sem nome drawio](https://github.com/user-attachments/assets/930c83ff-829d-4298-b3b2-fb87eafb9b4c)
          
---

## Estrutura do Código

O código do projeto está organizado em módulos, cada um responsável por uma funcionalidade específica. Essa modularização facilita a manutenção, a depuração e a reutilização do código. Abaixo está a descrição de cada módulo:

---

### **main.c**
O arquivo `main.c` contém o código principal do projeto. Ele é responsável por:
- Inicializar o hardware (display, joystick, matriz de LEDs, buzzer).
- Controlar o fluxo principal do programa, incluindo a leitura da temperatura, a atualização do estado da placa e a exibição das informações no display.
- Gerenciar a alternância entre as telas no display OLED.

---

### **temperature.c / temperature.h**
Esses arquivos contêm as funções relacionadas à leitura e exibição da temperatura. As principais funções são:
- **`read_simulated_temperature()`:** Lê o valor do joystick e converte para uma faixa de temperatura entre 0°C e 100°C.
- **`update_placa_state()`:** Atualiza o estado da placa (normal, alerta ou crítico) com base na temperatura simulada.
- **`display_temperature()`:** Exibe a temperatura atual no display OLED.
- **`update_temperature_history()`:** Atualiza o histórico de temperatura, utilizado para gerar o gráfico.

---

### **display.c / display.h**
Esses arquivos contêm as funções para controle do display OLED. As principais funções são:
- **`init_display()`:** Inicializa o display OLED e configura a comunicação I2C.
- **`display_message()`:** Exibe uma mensagem de duas linhas no display OLED.
- **`display_temperature_graph()`:** Desenha o gráfico de temperatura no display OLED, com base no histórico de leituras.

---

### **matrix.c / matrix.h**
Esses arquivos contêm as funções para controle da matriz de LEDs. As principais funções são:
- **`npInit()`:** Inicializa a matriz de LEDs e configura os pinos GPIO.
- **`npSetLED()`:** Define o estado de um LED específico na matriz.
- **`npWrite()`:** Envia os dados para a matriz de LEDs.
- **`blinkMatrixRedNonBlocking()`:** Pisca a matriz de LEDs em vermelho de forma não bloqueante.

---

### **controle_buzzer.c / controle_buzzer.h**
Esses arquivos contêm as funções para controle do buzzer. As principais funções são:
- **`start_buzzer()`:** Ativa o buzzer por um período de tempo especificado.
- **`controle_buzzer()`:** Controla o buzzer com base no estado da placa (ativa o buzzer em caso de estado crítico).

---

## Licença
Usa, melhora e distribui.
