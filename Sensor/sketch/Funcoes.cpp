#include "Funcoes.h"

float DLI() {
  DateTime date = getTime();
  float hora = date.hour();

  float c = 0.0036;

  float DLI = LIGHT * c * hora;

  return DLI;
}

float VPD () {
  float umidadeRelativa = dht.readHumidity();
  float temperatura = dht.readTemperature();

  // Verifica se a leitura do sensor falhou
  if (isnan(HUMIDITY) || isnan(TEMPERATURE)) return -1.0;

  // Pressão de Vapor de Saturação (SVP) em kPa
  float SVP = 0.61078 * exp((17.27 * TEMPERATURE) / (TEMPERATURE + 237.3));

  // Pressão de Vapor Atual (AVP) e VPD em kPa
  float VPD = SVP * (1.0 - (HUMIDITY / 100.0));

  return VPD;
}