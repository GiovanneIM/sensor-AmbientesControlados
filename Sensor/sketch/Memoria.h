#ifndef MEMORIA_H
#define MEMORIA_H

// MEMORIA.H
// Biblioteca para organizar e controlar a EEPROM, onde serão armazenadas as
// configurações que o usuário fizer e os registros das anomalias

#include "Config.h"
#include "Idiomas.h"
#include "Perifericos.h"
#include <EEPROM.h>
#include <RTClib.h> // Biblioteca para Relógio em Tempo Real

// ORGANIZAÇÃO DA EEPROM:
// 0 ► Indica se é a primeira inicialização
// 1 ► Endereço do próximo registro
// 2 - 25 ► Configurações do usuário
// 26 - 67 ► Controle das anomalias ativas
// 68 - 1023 ► Armazenamento das anomalias

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// CONTROLE

// Controlar se é a primeira inicialização - Primeiro byte
#define EEPROM_FIRST_INICIALIZATION 0
#define INITIALIZATED_FLAG 0x42

// Controlar se é a primeira inicialização - Segundo byte
#define EEPROM_NEXT_ADDRESS 1

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// CONFIGURAÇÕES

// Inicio do espaço da EEPROM destinado a armazenar as configurações
#define EEPROM_CONFIG_START 2

// Struct para armazenar as configurações do usuário
struct __attribute__((packed)) Configuration {
  // SETUP
  char language = '1';    // 1 byte - Português
  char tempScale = '0';    // 1 byte - C
  char dateFormat = '0';  // 1 byte - dd/mm/aaaa
  char hourFormat = '0';  // 1 byte - 24 horas
  unsigned long buzzInterval = 60000;  // 4 bytes - 1 min

  // TRIGGERS
  int tempMin = 1500;      // 2 bytes - 15º
  int tempMax = 2500;      // 2 bytes - 25º
  int humidMin = 3000;     // 2 bytes - 30%
  int humidMax = 5000;     // 2 bytes - 50%
  int luminMin = 0000;     // 2 bytes - 0%
  int luminMax = 3000;     // 2 bytes - 30%

  // CONTROLE
  long turnedOffTime;     // 4 bytes
};
extern Configuration config;

// Tamanho da configuracao - 24 bytes
#define CONFIGURATION_SIZE sizeof(struct Configuration)

// Função para carregar as configurações
void loadConfig();

// Função para salvar as configurações
void saveConfig();

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// ANOMALIAS

// Inicio do espaço da EEPROM destinado a armazenar os registros das anomalias
#define EEPROM_ANOMALIES_START EEPROM_CONFIG_START + CONFIGURATION_SIZE

// Struct para construção de uma anomalia
struct __attribute__((packed)) Anomaly {
  unsigned long start;  // 4 bytes
  unsigned long finish; // 4 bytes
  int type = -1;             // 2 bytes
  int limit;             // 2 bytes
  int extremeValue;     // 2 bytes
};

// Tamanho de cada anomalia - 14 bytes
#define ANOMALY_SIZE sizeof(struct Anomaly)

// Array para armazenar anomalias ativas
// Um espaço para Temperatura, um para Umidade e um para Luz
extern Anomaly activesAnomalies[3];

// Inicio do espaço para o histórico
#define EEPROM_HISTORY_START (EEPROM_ANOMALIES_START + 3 * ANOMALY_SIZE)

// Número máximo de registros
#define ANOMALIES_MAX ((1024 - EEPROM_ANOMALIES_START - 3 * ANOMALY_SIZE) / ANOMALY_SIZE)

// Fim do espaço para anomalias
#define EEPROM_HISTORY_END (EEPROM_ANOMALIES_START + (ANOMALY_SIZE * ANOMALIES_MAX))

// Endereço para o próximo registro
extern int currentAddress;

// Endereço do último registro feito
extern int lastAddress;

// VERIFICA SE HÁ UMA NOVA ANOMALIA OU SE É NECESSÁRIO ATUALIZAR ALGUMA
void verifyAnomalies(float sensorValue, float limitMin, float limitMax, int typeAnomaly, int i);

void saveActivitysAnomalies();

void newAnomaly(int type, float value, float limit, int i);

bool checkAnomalie();

void finishAnomaly(int i);

// Atualiza o endereço atual para registo
void getNextAddress();

// Imprime os registros no monitor serial
void print_log();

void printhHistory();

void serialDate(DateTime data);

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

#endif