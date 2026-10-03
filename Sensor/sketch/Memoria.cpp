#include "Memoria.h"

// Endereço para o próximo registro
int currentAddress = EEPROM_HISTORY_START;

// Endereço do último registro feito
int lastAddress = -1;

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// CONFIGURAÇÕES

// Definir a variável para as configurações
Configuration config;

// Array para armazenar anomalias ativas
// 0 - Anomalia de Temperatura
// 1 - Anomalia de Umidade
// 2 - Anomalia de Luminosidade
Anomaly activesAnomalies[3];

// Função para carregar as configurações
void loadConfig() {
  char flag;
  EEPROM.get(EEPROM_FIRST_INICIALIZATION, flag);

  // Se for a primeira inicialização, salva os valores padrões
  if (flag != INITIALIZATED_FLAG) {

    for (int i = 0; i < 3; i++) {
        activesAnomalies[i].type = -1;
        activesAnomalies[i].start = 0;
        activesAnomalies[i].finish = 0;
        activesAnomalies[i].limit = 0;
        activesAnomalies[i].extremeValue = 0;
    }

    currentAddress = EEPROM_HISTORY_START;

    EEPROM.put(EEPROM_NEXT_ADDRESS, currentAddress);
    EEPROM.put(EEPROM_CONFIG_START, config);
    EEPROM.put(EEPROM_ANOMALIES_START, activesAnomalies);

    // Marca que já inicializou
    EEPROM.put(EEPROM_FIRST_INICIALIZATION, INITIALIZATED_FLAG);
  }
  // Se não, carrega os valores salvos
  else {
    EEPROM.get(EEPROM_NEXT_ADDRESS, currentAddress);
    EEPROM.get(EEPROM_ANOMALIES_START, activesAnomalies); 
    EEPROM.get(EEPROM_CONFIG_START, config);
  }
}

// Função para salvar as configurações
void saveConfig() {
  EEPROM.put(EEPROM_CONFIG_START, config);
  EEPROM.put(EEPROM_NEXT_ADDRESS, currentAddress);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// ANOMALIAS

// Atualiza o endereço do próximo registro
void getNextAddress() {
  lastAddress = currentAddress;
  currentAddress += ANOMALY_SIZE;

  // Volta para o começo se atingir o limite
  if (currentAddress >= EEPROM_HISTORY_END) {
    currentAddress = EEPROM_HISTORY_START;
  }
  
  EEPROM.put(EEPROM_NEXT_ADDRESS, currentAddress);
}

// VERIFICA SE UMA NOVA ANOMALIA OU SE É NECESSÁRIO ATUALIZAR ALGUMA
void verifyAnomalies(float sensorValue, float limitMax, float limitMin, int typeAnomaly, int i) {
  int currentValue = int(sensorValue * 100);

  // ACIMA
  if (sensorValue > limitMax) {
    // SE JÁ ESTIVER ATIVA
    if (activesAnomalies[i].type == typeAnomaly) {
      if (currentValue > activesAnomalies[i].extremeValue) {
        activesAnomalies[i].extremeValue = currentValue;
        buzzer(500, 100);
        saveActivitysAnomalies();
      }
    }
    // NOVA ANOMALIA
    else {
      newAnomaly(typeAnomaly, sensorValue, limitMax, i);
    }
  }

  // ABAIXO
  else if (sensorValue < limitMin) {
    // SE JÁ ESTIVER ATIVA
    if (activesAnomalies[i].type == (typeAnomaly + 1)) {
      if (currentValue < activesAnomalies[i].extremeValue) {
        activesAnomalies[i].extremeValue = currentValue;
        buzzer(500, 100);
        saveActivitysAnomalies();
      }
    }
    // NOVA ANOMALIA
    else {
      newAnomaly(typeAnomaly + 1, sensorValue, limitMin, i);
    }
  }
  // DENTRO DO LIMITE
  else {
    if (activesAnomalies[i].type != -1) {
      finishAnomaly(i);
    }
  }
}

// NOVA ANOMALIA
void newAnomaly(int type, float value, float limit, int i) {
  DateTime now = getTime();

  activesAnomalies[i].start = now.unixtime();
  activesAnomalies[i].type = type;
  activesAnomalies[i].limit = int(limit * 100);
  activesAnomalies[i].extremeValue = int(value * 100);
  saveActivitysAnomalies();

  Serial.println("= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =");

  Serial.println("ANOMALIA INICIADA");
  serialDate(now);

  Serial.print(type);
  Serial.print("\t");
  Serial.print(limit);
  Serial.print("\t");
  Serial.println(value);

  Serial.println("= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =");

}

// VERIFICA SE HÁ UMA ANOMALIA EM ANDAMENTO
bool checkAnomalie() {
  for (Anomaly a : activesAnomalies) {
    if (a.type != -1) {
      buzzer(500, 100);
      led(255, 0, 0);
      return true;
    }
  }

  led(0, 255, 0);
  return false;
}

// ENCERRA E SALVA UMA ANOMALIA
void finishAnomaly(int i) {
  // Atualizando a data e  hora de finalização
  DateTime now = getTime();
  DateTime start(activesAnomalies[i].start);

  activesAnomalies[i].finish = now.unixtime();

  // Salvando a anomalia
  EEPROM.put(currentAddress, activesAnomalies[i]);
  getNextAddress();

  Serial.println("= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =");

  Serial.println("ANOMALIA ENCERRADA");
  serialDate(start);
  serialDate(now);

  Serial.print(activesAnomalies[i].type);
  Serial.print("\t");
  Serial.print(activesAnomalies[i].limit / 100.0);
  Serial.print("\t");
  Serial.println(activesAnomalies[i].extremeValue / 100.0);
  Serial.print("Salva em: ");
  Serial.println(lastAddress);

  Serial.println("= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =");

  // printhHistory();

  Serial.println("= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =");


  // Indicando que não há anomalia em andamento
  activesAnomalies[i].type = -1;
  saveActivitysAnomalies();

  buzzer(500, 100);
};

// SALVA AS ANOMALIAS ATIVAS
void saveActivitysAnomalies() {
  EEPROM.put(EEPROM_ANOMALIES_START, activesAnomalies);
}

// IMPRIME O HISTÓRICO
void printhHistory() {
  Serial.println("ANOMALIES HISTORY");

  Serial.print("ENDEREÇO");
  Serial.print("\t");
  Serial.print("START");
  Serial.print("\t");
  Serial.print("FINISH");
  Serial.print("\t");
  Serial.print("TYPE");
  Serial.print("\t");
  Serial.print("LIMIT");
  Serial.print("\t");
  Serial.print("EXTREME VALUE");
  Serial.println();

  for (
    int address = EEPROM_HISTORY_START;
    address < EEPROM_HISTORY_END;
    address += ANOMALY_SIZE
  ) {
    Anomaly anomaly;
    EEPROM.get(address, anomaly);

    // Verificar se os dados são válidos antes de imprimir
    if (anomaly.start == 0xFFFFFFFF) continue;

    Serial.println("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");

    // Converter valores
    DateTime start = DateTime(anomaly.start);
    int type = anomaly.type;
    float limit = anomaly.limit / 100.0;
    float extremeValue = anomaly.extremeValue / 100.0;

    // ENDEREÇO
    Serial.print(address);
    Serial.print("\t");

    // INICIO
    Serial.print(start.timestamp(DateTime::TIMESTAMP_FULL));
    Serial.print("\t");

    // FIM
    if (anomaly.finish != 0xFFFFFFFF) {
      DateTime finish(anomaly.finish);
      Serial.print(finish.timestamp(DateTime::TIMESTAMP_FULL));
    } else {
      Serial.print("Em andamento"); // ou "Ainda em andamento"
    }
    Serial.print("\t");

    // TIPO
    Serial.print(type);
    Serial.print("\t");

    // LIMITE
    Serial.print(limit);
    Serial.print("\t");

    // VALOR EXTREMO
    Serial.print(extremeValue);
    Serial.print("\t");

    Serial.println();
  }
}


// Função para imprimir uma data no Serial
void serialDate(DateTime data) {
  Serial.print(data.day(), DEC);
  Serial.print('/');
  Serial.print(data.month(), DEC);
  Serial.print('/');
  Serial.print(data.year(), DEC);
  Serial.print(" ");
  Serial.print(data.hour(), DEC);
  Serial.print(':');
  Serial.print(data.minute(), DEC);
  Serial.print(':');
  Serial.println(data.second(), DEC);
}