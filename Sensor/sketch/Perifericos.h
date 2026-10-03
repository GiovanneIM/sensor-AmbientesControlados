#ifndef PERIFERICOS_H
#define PERIFERICOS_H

#include "Config.h"
#include "Icons.h"
#include <LiquidCrystal_I2C.h>  // Biblioteca para LCD I2C
#include <RTClib.h> // Biblioteca para Relógio em Tempo Real
#include <Wire.h>   // Biblioteca para comunicação I2C
#include "DHT.h"    // Biblioteca para o DHT

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PERIFÉRICOS

// DHT22
#define DHTPIN 13
#define DHTTYPE DHT22
extern DHT dht;

// LDR
#define LDRPIN A0

// BOTOES
struct Button {
  int pin;
  int oldValue;
};
#define NUM_BUTTONS 4
extern Button buttons[NUM_BUTTONS];

// LCD
extern LiquidCrystal_I2C lcd;

// RTC
extern RTC_DS1307 RTC;

// BUZZER
#define BUZZERPIN 11

// LED RGB
#define LED_R 10
#define LED_G 9
#define LED_B 3

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// VALORES DOS SENSORES
extern float TEMPERATURE;
extern float HUMIDITY;
extern float LIGHT;

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// FUNÇÕES

// Função para inicializar os periféricos
void initializeComponents();

// Função para obter a data e hora atual
DateTime getTime();

// Função para alterar a data atual
void alterDate(int day, int month, int year);

// Função para alterar a hora atual
void alterTime(int hour, int minute); 

// Função para verificar se o botao power foi pressionado
bool readButtonPower();

// Função para ler os botoes
int readButtons();

// Função para ler os sensores
void readSensors();

// Função para acender o LED
void led(int r, int g, int b);

// Função para produzir um som no buzzer
void buzzer (int frequencia, int duracao);

#endif