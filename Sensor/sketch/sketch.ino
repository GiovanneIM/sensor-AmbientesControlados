#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>  // Biblioteca para LCD I2C
#include <RTClib.h> // Biblioteca para Relógio em Tempo Real
#include <Wire.h>   // Biblioteca para comunicação I2C
#include "Idiomas.h"
#include "Config.h"
#include "Memoria.h"
#include "Perifericos.h"
#include "Telas.h"
#include "Funcoes.h"

// EXECUÇÃO  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

// VARIAVEIS
bool ligado = false;
bool paused = false;
int screen = 0;
int numScreens = 4;

// Última leitura dos sensores
unsigned long lastSensorMillis = 0;
// Última troca de tela
unsigned long lastScreenMillis = 0;
// Última interação
unsigned long lastInteractionMillis = 0;


// AO LIGAR O DISPOSITIVO
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  ligado = false;

  initializeComponents();
  loadConfig();
}

// LOOP DE FUNCIONAMENTO
void loop() {
  // SE ESTIVER DESLIGADO
  if (!ligado) {
    if (readButtonPower()) {
      ligado = true;
      screenInitialization();
      exibirTela();
    }
  }
  // SE ESTIVER LIGADO
  else {
    // Verificar se há uma anomalia em andamento
    checkAnomalie();

    unsigned long currentMillis = millis();

    // Serial.println(currentMillis);
    // Serial.println(lastSensorMillis);
    // Serial.println(lastScreenMillis);

    // Ler os sensores e atualizar a tela a cada 3s
    if (currentMillis - lastSensorMillis >= 3000) {
      lastSensorMillis = currentMillis;
      readSensors();
      exibirTela();
    }

    // Trocar tela a cada 5s, se não estiver pausado
    if ((currentMillis - lastScreenMillis >= 5000) && !paused) {
      lastScreenMillis = currentMillis;
      screen = ++screen % numScreens;
    }

    // Despausar a tela depois de 30s sem interação
    if (paused && (currentMillis - lastInteractionMillis >= 30000)) {
      paused = false;
    }

    // Interação do usuário
    int botao = readButtons();
    switch (botao) {
      // DESLIGAR
      case 0:
        screenTurningOff();
        ligado = false;
        break;

      // ABRIR CONFIGURAÇÕES
      case 1:
        configurationMenu();
        paused = false;
        exibirTela();
        break;

      // TELA ANTERIOR
      case 2:
        screen = (screen == 0) ? numScreens - 1 : --screen;
        paused = true;
        lastInteractionMillis = currentMillis;
        exibirTela();
        break;

      // PRÓXIMA TELA
      case 3:
        screen = ++screen % numScreens;
        paused = true;
        lastInteractionMillis = currentMillis;
        exibirTela();
        break;
    }
  }
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

void exibirTela() {
  switch (screen) {
    case 0:
      // DIA E HORA
      screenTime();
      break;

    case 1:
      // MONITORAMENTO
      screenMonitoring();
      break;

    case 2:
      // LIMITES
      screenLimits();
      break;

    case 3:
      // ÚLTIMO REGISTRO
      screenLastRecord();
      break;
  }
}
