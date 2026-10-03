#ifndef TELAS_H
#define TELAS_H

#include "Config.h"
#include "Memoria.h"
#include "Perifericos.h"
#include "Idiomas.h"
#include <LiquidCrystal_I2C.h>  // Biblioteca para LCD I2C
#include <RTClib.h> // Biblioteca para Relógio em Tempo Real
#include <EEPROM.h>

void screenInitialization();

void screenTurningOff();

void screenTime();

void screenMonitoring();

void screenLimits();

void screenLastRecord();

void showWaitScreen(char* message, int delayMillis);

void configurationMenu();

void changeLanguage();

void configurationDataMenu();

void changeDate();

void changeDateFormat();

void configurationHourMenu();

void changeHour();

void changeHourFormat();

void configurationTemperature();

void changeTempScale();

void configurationHumidity();

void configurationLuminosity();

void setLimit(char* limitFor);

#endif