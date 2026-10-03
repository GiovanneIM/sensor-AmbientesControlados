# Data Logger Climático com Arduino

Este projeto implementa um Data Logger inteligente baseado em Arduino, focado no monitoramento ambiental e agrícola. Ele mede temperatura, umidade e luminosidade, além de calcular métricas avançadas como o Déficit de Pressão de Vapor (VPD) e a Integral de Luz Diária (DLI). O sistema possui alertas visuais e sonoros, armazenamento de configurações e registro de anomalias na memória EEPROM.

## 🚀 Funcionalidades

*   **Monitoramento em Tempo Real:** Leitura de Temperatura, Umidade (via sensor DHT) e Luminosidade (via LDR).
*   **Métricas Agrícolas:** Cálculo automático de VPD (Vapor Pressure Deficit) e DLI (Daily Light Integral).
*   **Interface Multilíngue:** Suporte nativo para Português, Inglês e Espanhol.
*   **Armazenamento Não-Volátil (EEPROM):** Salva as preferências do usuário (idioma, limites, formatos de data/hora) e registra um log histórico de anomalias quando os valores saem das faixas configuradas.
*   **Alertas:** Feedback sonoro através de Buzzer e visual através de um LED RGB.
*   **Relógio em Tempo Real (RTC):** Controle preciso de data e hora usando o módulo DS1307.
*   **Interface Física:** Display LCD 16x2 I2C com ícones customizados e controle por 4 botões de navegação.

## 🛠️ Hardware Necessário

*   1x Placa compatível com Arduino (Uno, Mega, Nano, etc.)
*   1x Sensor de Temperatura e Umidade (DHT11 ou DHT22)
*   1x Módulo RTC DS1307
*   1x Display LCD 16x2 com módulo I2C
*   1x LDR (Sensor de Luz) + Resistor de pull-down
*   1x LED RGB (Catodo ou Anodo comum) + Resistores
*   1x Buzzer
*   4x Push Buttons (Botões)

## 🗂️ Arquitetura do Software

O código foi modularizado para facilitar a manutenção e escalabilidade:

*   **`Perifericos.h/cpp`**: Camada de abstração de hardware (HAL). Gerencia a inicialização dos pinos, display LCD (com ícones customizados), sensor DHT, leitura do RTC e lógica de debounce dos 4 botões (POWER, SETUP, DOWN, UP).
*   **`Memoria.h/cpp`**: Gerencia o uso da EEPROM. Divide a memória em uma área de configuração (salvando limites, idiomas e preferências) e um buffer circular para armazenar o registro contínuo de anomalias (log de eventos extremos).
*   **`Idiomas.h/cpp`**: Dicionário de strings e enumeradores para garantir a fácil troca de idiomas em tempo de execução sem hardcoding de textos pelo resto do sistema.
*   **`Funcoes.h/cpp`**: Contém a lógica de negócio e matemática avançada do projeto, especificamente os algoritmos para cálculo de VPD e DLI.
*   **`Icons.h/cpp`**: Define os bytes arrays para os caracteres customizados do display LCD (Termômetro, Gota e Sol).

## ⚙️ Instalação e Uso Inicial

1. Realize as conexões de hardware conforme definidos (ou a serem definidos) no arquivo principal de configuração (`Config.h`). *Nota: Os botões estão mapeados nos pinos digitais 4, 5, 6 e 7.*
2. Abra a IDE do Arduino, instale as bibliotecas necessárias (RTClib, LiquidCrystal_I2C, DHT sensor library).
3. Compile e faça o upload para a placa.
4. Na primeira execução, o sistema gravará os valores padrão na EEPROM (ex: Temperatura alvo de 15°C a 25°C).
5. Para extrair os dados de log das anomalias, conecte o Arduino ao computador, abra o Monitor Serial e o sistema listará o histórico em um formato separado por tabulações (pronto para Excel/Planilhas).