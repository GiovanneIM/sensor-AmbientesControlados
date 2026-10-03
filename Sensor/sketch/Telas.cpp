#include "Telas.h"

// AUXILIARES
void printTwoDigits(int value) {
  if (value < 10) lcd.print("0");
  lcd.print(value);
}

void printCentered(const char* text, int row) {
  int len = strlen(text);
  int pos = (16 - len) / 2;
  lcd.setCursor(pos, row);
  lcd.print(text);
}

void printDate(DateTime date) {
  int day = date.day();
  int month = date.month();
  int year = date.year();

  // dd/mm/aaaa
  if (config.dateFormat == '0') {
    printTwoDigits(day);
    lcd.print("/");
    printTwoDigits(month);
    lcd.print("/");
    lcd.print(year);
  }
  // mm/dd/aaaa
  else {
    printTwoDigits(month);
    lcd.print("/");
    printTwoDigits(day);
    lcd.print("/");
    lcd.print(year);
  }
}

void printHour(DateTime date) {
  int hour = date.hour();
  int minute = date.minute();
  int second = date.second();

  // 24 Horas
  if (config.hourFormat == '0') {
    lcd.setCursor(4, 1);

    printTwoDigits(hour);
    lcd.print(":");
    printTwoDigits(minute);
    lcd.print(":");
    printTwoDigits(second);
  }
  // 12 Horas
  else if (config.hourFormat == '1') {
    lcd.setCursor(3, 1);

    int displayHour = hour % 12;
    if (displayHour == 0) displayHour = 12; // 12:00 e 00:00

    printTwoDigits(displayHour);
    lcd.print(":");
    printTwoDigits(minute);
    lcd.print(":");
    printTwoDigits(second);

    if (hour < 12) {
      lcd.print(" AM");
    } else {
      lcd.print(" PM");
    }
  }
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// TELAS PRINCIPAIS

void screenInitialization() {
  lcd.backlight();
  buzzer(440, 500);
  showWaitScreen("INICIANDO", 500);
  led(0, 255, 0);
}

void screenTurningOff() {
  led(0, 255, 0);
  buzzer(440, 500);
  showWaitScreen("DESLIGANDO", 500);
  lcd.clear();
  lcd.noBacklight();
  led(0, 0, 0);
}

// DATA E HORA
void screenTime() {
  DateTime date = getTime();

  lcd.clear();

  // Data
  lcd.setCursor(3, 0);
  printDate(date);

  // Hora
  printHour(date);

}

// MONITORAMENTO
void screenMonitoring() {
  lcd.clear();

  char scaleSymbol = (config.tempScale == '0') ? 'C' : 'F';

  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print(TEMPERATURE);
  lcd.write(223);
  lcd.print(scaleSymbol);

  lcd.setCursor(0, 1);
  lcd.write(byte(1));
  lcd.print(HUMIDITY);
  lcd.print("%");

  lcd.setCursor(8, 1);
  lcd.write(byte(2));
  lcd.print(LIGHT);
  lcd.print("%");
}

// Limites
void screenLimits() {
  lcd.clear();

  char scaleSymbol = (config.tempScale == '0') ? 'C' : 'F';

  // TEMPERATURA
  lcd.setCursor(0, 0);
  lcd.write(byte(0));

  lcd.print(config.tempMin / 100.0, 0);
  lcd.write(223);
  lcd.print(scaleSymbol);

  lcd.setCursor(1, 1);
  lcd.print(config.tempMax / 100.0, 0);
  lcd.write(223);
  lcd.print(scaleSymbol);

  // UMIDADE
  lcd.setCursor(7, 0);
  lcd.write(byte(1));

  lcd.print(config.humidMin / 100.0, 0);
  lcd.print("%");

  lcd.setCursor(8, 1);
  lcd.print(config.humidMax / 100.0, 0);
  lcd.print("%");

  // LIGHT
  lcd.setCursor(12, 0);
  lcd.write(byte(2));
  
  lcd.print(config.luminMin / 100.0, 0);
  lcd.print("%");

  lcd.setCursor(13, 1);
  lcd.print(config.luminMax / 100.0, 0);
  lcd.print("%");
}

// ÚLTIMO REGISTRO
void screenLastRecord() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ULTIMO REGISTRO ");

  // SE NÃO HOUVER REGISTROS
  if (lastAddress == -1) return;

  // OBTER HORÁRIO DA ÚLTIMA GRAVAÇÃO
  long timeStamp;
  EEPROM.get(lastAddress, timeStamp);

  // VERIFICANDO SE O DADO OBTIDO É VÁLIDO
  if (timeStamp == 0xFFFFFFFF) return;

  // CONVERTER TIMESTAMP PARA DATETIME
  DateTime lastRecord(timeStamp);

  lcd.setCursor(0, 1);
  lcd.print(lastRecord.day() < 10 ? "0" : ""); // Adiciona zero à esquerda se dia for menor que 10
  lcd.print(lastRecord.day());
  lcd.print("/");
  lcd.print(lastRecord.month() < 10 ? "0" : ""); // Adiciona zero à esquerda se mês for menor que 10
  lcd.print(lastRecord.month());
  lcd.print("/");
  lcd.print(lastRecord.year());
  lcd.print(" ");
  lcd.print(lastRecord.hour() < 10 ? "0" : ""); // Adiciona zero à esquerda se hora for menor que 10
  lcd.print(lastRecord.hour());
  lcd.print(":");
  lcd.print(lastRecord.minute() < 10 ? "0" : ""); // Adiciona zero à esquerda se minuto for menor que 10
  lcd.print(lastRecord.minute());
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// CONFIGURAÇÕES

// MENU DE CONFIGURAÇÕES
void configurationMenu() {
  int i = 0;
  bool redraw = true;
  int idioma = config.language - '0';

  // Acende o led como azul
  led(0, 0, 255);

  while (true) {
    // Desenhar a tela
    if (redraw) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(getText(MSG_CONFIGURACAO, idioma));

      lcd.setCursor(0, 1);
      lcd.print("<");

      char* text = getText(Configuracoes[i], idioma);
      printCentered(text, 1);

      lcd.setCursor(15, 1);
      lcd.print(">");

      redraw = false;
    }

    int botao = readButtons();
    switch (botao) {
      // VOLTAR
      case 0:
        lcd.clear();
        return;

      // CONFIRMAR
      case 1:
        lcd.clear();
        configure(i);
        led(0, 255, 0);
        return;
        break;

      // ANTERIOR
      case 2:
        i = (--i < 0) ? NUM_CONFIGURACOES - 1 : i;
        redraw = true;
        break;

      // PRÓXIMA
      case 3:
        i = ++i % NUM_CONFIGURACOES;
        redraw = true;
        break;
    }
  }
}

// = = = = = = = = = =

// IDIOMA
void changeLanguage() {
  int i = 0;
  bool redraw = true;
  int idioma = config.language - '0';

  while (true) {
    // Desenha a Tela
    if (redraw) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(getText(MSG_IDIOMA, idioma));

      lcd.setCursor(0, 1);
      lcd.print("<");

      printCentered(Idiomas[i], 1);

      lcd.setCursor(15, 1);
      lcd.print(">");


      redraw = false;
    }

    int botao = readButtons();
    switch (botao) {
      // CANCELAR
      case 0:
        lcd.clear();
        return;

      // CONFIRMAR
      case 1:
        lcd.clear();

        // Atualizar configuração
        config.language = i + '0';
        idioma = i;

        // Salvar mudança
        saveConfig();
        showWaitScreen(getText(MSG_SALVO, idioma), 500);

        return;

      // ANTERIOR
      case 2:
        i = ++i % NUM_IDIOMAS;
        redraw = true;
        break;

      // PRÓXIMA
      case 3:
        i = (--i < 0) ? NUM_IDIOMAS - 1 : i;
        redraw = true;
        break;
    }
  }
}

// = = = = = = = = = =

// CONFIGURAÇÕES DE DATA
void configurationDataMenu() {
  int i = 0;
  bool redraw = true;
  int idioma = config.language - '0';

  while (true) {
    // Desenhar a tela
    if (redraw) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(getText(MSG_DATA, idioma));

      lcd.setCursor(0, 1);
      lcd.print("<");

      char* text = getText(ConfigData[i], idioma);
      printCentered(text, 1);

      lcd.setCursor(15, 1);
      lcd.print(">");

      redraw = false;
    }

    int botao = readButtons();
    switch (botao) {
      // VOLTAR
      case 0:
        lcd.clear();
        return;

      // CONFIRMAR
      case 1:
        lcd.clear();

        switch (i) {
          case 0:
            changeDate();
            break;

          case 1:
            changeDateFormat();
            break;
        };

        return;
        break;

      // ANTERIOR
      case 2:
        i = (--i < 0) ? 2 - 1 : i;
        redraw = true;
        break;

      // PRÓXIMA
      case 3:
        i = ++i % 2;
        redraw = true;
        break;
    }
  }
}

// DEFINIR DATA
void changeDate() {
  int i = 0;
  char digit = 0;
  bool redraw = true;
  int idioma = config.language - '0';

  // Obter a data configurada atualmente
  DateTime date = getTime();
  int day = date.day();
  int month = date.month();
  int year = date.year();

  // Data definida pelo usuário
  char data[11];

  if (config.dateFormat == '0') {
    sprintf(data, "%02d/%02d/%04d", day, month, year);
  }
  else {
    sprintf(data, "%02d/%02d/%04d", month, day, year);
  }

  while (true) {
    // Desenha a Tela
    if (redraw) {
      lcd.clear();

      // Exibir a data
      lcd.setCursor(0, 0);
      lcd.print(getText(MSG_DEFINIR_DATA, idioma));
      printCentered(data, 1);

      // Posicionar o cursor
      int len = strlen(data);
      int cursorPos = i + ((16 - len) / 2);
      lcd.setCursor(cursorPos, 1);
      lcd.blink();

      redraw = false;
    }

    int botao = readButtons();
    switch (botao) {
      // CANCELAR
      case 0:
        lcd.clear();
        return;

      // PRÓXIMO DIGITO
      case 1:
        // Avançar para o próximo digito
        i++;
        redraw = true;

        // Pular as barras
        if (i == 2 || i == 5) {
          i++;
        }

        // Se tiver chegado ao final da data
        if (i >= 10) {
          lcd.clear();
          lcd.noBlink();
          Serial.println(data);

          if (config.dateFormat == '0')
            sscanf(data, "%2d/%2d/%4d", &day, &month, &year);
          else
            sscanf(data, "%2d/%2d/%4d", &month, &day, &year);

          alterDate(day, month, year);

          showWaitScreen(getText(MSG_SALVO, idioma), 500);

          return;
        }

        break;

      // AUMENTAR
      case 2:
        digit = data[i] - '0';
        digit = (digit >= 9) ? 0 : digit + 1;
        data[i] = digit + '0';
        redraw = true;
        break;

      // DIMINUIR
      case 3:
        digit = data[i] - '0';
        digit = (digit <= 0) ? 0 : digit - 1;
        data[i] = digit + '0';
        redraw = true;
        break;
    }
  }
}

// FORMATO DA DATA
void changeDateFormat() {
  int i = 0;
  bool redraw = true;
  int idioma = config.language - '0';

  const char* dateFormats[] = {
    "DD/MM/AAAA",
    "MM/DD/AAAA"
  };

  while (true) {
    // Desenha a Tela
    if (redraw) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(getText(MSG_FORMATO_DATA, idioma));

      lcd.setCursor(0, 1);
      lcd.print("<");

      printCentered(dateFormats[i], 1);

      lcd.setCursor(15, 1);
      lcd.print(">");


      redraw = false;
    }

    // Interação do usuário
    int botao = readButtons();
    switch (botao) {
      // CANCELAR
      case 0:
        lcd.clear();
        return;

      // CONFIRMAR
      case 1:
        lcd.clear();

        // Atualizar configuração
        config.dateFormat = i + '0';

        // Salvar mudança
        saveConfig();
        showWaitScreen(getText(MSG_SALVO, idioma), 500);

        return;

      // ANTERIOR
      case 2:
        i = ++i % 2;
        redraw = true;
        break;

      // PRÓXIMA
      case 3:
        i = (--i < 0) ? 1 : i;
        redraw = true;
        break;
    }
  }
}

// = = = = = = = = = =

// CONFIGURAÇÕES DE HORA
void configurationHourMenu() {
  int i = 0;
  bool redraw = true;
  int idioma = config.language - '0';

  while (true) {
    // Desenhar a tela
    if (redraw) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(getText(MSG_HORA, idioma));

      lcd.setCursor(0, 1);
      lcd.print("<");

      char* text = getText(ConfigHora[i], idioma);
      printCentered(text, 1);

      lcd.setCursor(15, 1);
      lcd.print(">");

      redraw = false;
    }

    int botao = readButtons();
    switch (botao) {
      // VOLTAR
      case 0:
        lcd.clear();
        return;

      // CONFIRMAR
      case 1:
        lcd.clear();

        switch (i) {
          case 0:
            changeHour();
            break;

          case 1:
            changeHourFormat();
            break;
        };

        return;
        break;

      // ANTERIOR
      case 2:
        i = (--i < 0) ? 2 - 1 : i;
        redraw = true;
        break;

      // PRÓXIMA
      case 3:
        i = ++i % 2;
        redraw = true;
        break;
    }
  }
}

// DEFINIR HORA
void changeHour() {
  int i = 0;
  char digit = 0;
  bool redraw = true;
  int idioma = config.language - '0';

  // Obter a hora configurada atualmente
  DateTime time = getTime();
  int hour = time.hour();
  int minute = time.minute();

  // Data definida pelo usuário
  char hora[6];
  char* ampm = "";

  // 24 Horas
  if (config.hourFormat == '0') {
    sprintf(hora, "%02d:%02d", hour, minute);
  }
  // 12 Horas
  else {
    hour = (hour % 12 == 0) ? 12 : hour % 12;
    sprintf(hora, "%02d:%02d", hour, minute);
    ampm = (hour >= 12) ? "PM" : "AM";
  }

  while (true) {
    // Desenha a Tela
    if (redraw) {
      lcd.clear();

      // Exibir a hora
      lcd.setCursor(0, 0);
      lcd.print(getText(MSG_DEFINIR_HORA, idioma));
      printCentered(hora, 1);
      lcd.print(" ");
      lcd.print(ampm);

      // Posicionar o cursor
      int len = strlen(hora);
      int cursorPos = i + ((16 - len) / 2);

      if (i < 5) {
        lcd.setCursor(cursorPos, 1);
        lcd.blink();
      }
      else {
        lcd.setCursor(cursorPos + 1, 1);
        lcd.blink();
        lcd.setCursor(cursorPos + 2, 1);
        lcd.blink();
      }


      redraw = false;
    }

    int botao = readButtons();
    switch (botao) {
      // CANCELAR
      case 0:
        lcd.clear();
        return;

      // PRÓXIMO DIGITO
      case 1:
        lcd.clear();
        lcd.noBlink();
        redraw = true;

        // Avançar para a próxima etapa da configuração
        i++;

        // Pular os dois pontos
        if (i == 2) i++;

        // Se tiver chegado ao final da hora e o padrão for de 12h
        // Indicar que a configuração foi encerrada
        if (i == 5 && config.hourFormat != '1') i++;

        // Fim da configuração
        if (i == 6) {
          Serial.print(hora);
          Serial.println(ampm);

          sscanf(hora, "%2d:%2d", &hour, &minute);

          if (ampm == "pm") hour += 12;

          alterTime(hour, minute);

          showWaitScreen(getText(MSG_SALVO, idioma), 500);

          return;
        }

        break;

      // AUMENTAR
      case 2:
        if (i == 5 && config.hourFormat == '1') {
          ampm = (strcmp(ampm, "AM") == 0) ? "PM" : "AM";
        }
        else {
          digit = hora[i] - '0';
          digit = (digit >= 9) ? 0 : digit + 1;
          hora[i] = digit + '0';
        }
        redraw = true;
        break;

      // DIMINUIR
      case 3:
        if (i == 5 && config.hourFormat == '1') {
          ampm = (strcmp(ampm, "AM") == 0) ? "PM" : "AM";
        }
        else {
          digit = hora[i] - '0';
          digit = (digit <= 0) ? 9 : digit - 1;
          hora[i] = digit + '0';
        }
        redraw = true;
        break;
    }
  }
}

// FORMATO DA HORA
void changeHourFormat() {
  int i = 0;
  bool redraw = true;
  int idioma = config.language - '0';

  const char* dateFormats[] = {
    "24",
    "12"
  };

  while (true) {
    // Desenha a Tela
    if (redraw) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(getText(MSG_FORMATO_HORA, idioma));

      lcd.setCursor(0, 1);
      lcd.print("<");

      lcd.setCursor(4, 1);
      lcd.print(dateFormats[i]);
      lcd.print(" ");
      lcd.print(getText(MSG_HOUR, idioma));
      lcd.print("S");

      lcd.setCursor(15, 1);
      lcd.print(">");


      redraw = false;
    }

    // Interação do usuário
    int botao = readButtons();
    switch (botao) {
      // CANCELAR
      case 0:
        lcd.clear();
        return;

      // CONFIRMAR
      case 1:
        lcd.clear();

        // Atualizar configuração
        config.hourFormat = i + '0';

        // Salvar mudança
        saveConfig();
        showWaitScreen(getText(MSG_SALVO, idioma), 500);

        return;

      // ANTERIOR
      case 2:
        i = ++i % 2;
        redraw = true;
        break;

      // PRÓXIMA
      case 3:
        i = (--i < 0) ? 1 : i;
        redraw = true;
        break;
    }
  }
}

// = = = = = = = = = =

// CONFIGURAÇÕES DE TEMPERATURA
void configurationTemperature() {
  int i = 0;
  bool redraw = true;
  int idioma = config.language - '0';

  while (true) {
    // Desenhar a tela
    if (redraw) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(getText(MSG_TEMPERATURA, idioma));

      lcd.setCursor(0, 1);
      lcd.print("<");

      char* text = getText(ConfigTemperatura[i], idioma);
      printCentered(text, 1);

      lcd.setCursor(15, 1);
      lcd.print(">");

      redraw = false;
    }

    // Interação do usuário
    int botao = readButtons();
    switch (botao) {
      // VOLTAR
      case 0:
        lcd.clear();
        return;

      // CONFIRMAR
      case 1:
        lcd.clear();

        switch (i) {
          // LIMITE
          case 0:
            setLimit("TEMPERATURE");
            break;
          // ESCALA
          case 1:
            changeTempScale();
            break;
        };

        return;
        break;

      // ANTERIOR
      case 2:
        i = (--i < 0) ? 2 - 1 : i;
        redraw = true;
        break;

      // PRÓXIMA
      case 3:
        i = ++i % 2;
        redraw = true;
        break;
    }
  }
}

// ESCALA DE TEMPERATURA
void changeTempScale() {
  int i = 0;
  bool redraw = true;
  int idioma = config.language - '0';

  const char* tempScales[] = {
    "Celsius",
    "Fahrenheit"
  };

  while (true) {
    // Desenha a Tela
    if (redraw) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(getText(MSG_ESCALATEMP, idioma));

      lcd.setCursor(0, 1);
      lcd.print("<");

      printCentered(tempScales[i], 1);

      lcd.setCursor(15, 1);
      lcd.print(">");


      redraw = false;
    }

    // Interação do usuário
    int botao = readButtons();
    switch (botao) {
      // CANCELAR
      case 0:
        lcd.clear();
        return;

      // CONFIRMAR
      case 1:
        lcd.clear();

        // Atualizar configuração
        config.tempScale = i + '0';

        // Salvar mudança
        saveConfig();
        showWaitScreen(getText(MSG_SALVO, idioma), 500);

        return;

      // ANTERIOR
      case 2:
        i = ++i % 2;
        redraw = true;
        break;

      // PRÓXIMA
      case 3:
        i = (--i < 0) ? 1 : i;
        redraw = true;
        break;
    }
  }
}

// = = = = = = = = = =

// CONFIGURAÇÕES DE UMIDADE
void configurationHumidity() {
  int i = 0;
  bool redraw = true;
  int idioma = config.language - '0';

  while (true) {
    // Desenhar a tela
    if (redraw) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(getText(MSG_UMIDADE, idioma));

      lcd.setCursor(0, 1);
      lcd.print("<");

      char* text = getText(ConfigUmidade[i], idioma);
      printCentered(text, 1);

      lcd.setCursor(15, 1);
      lcd.print(">");

      redraw = false;
    }

    // Interação do usuário
    int botao = readButtons();
    switch (botao) {
      // VOLTAR
      case 0:
        lcd.clear();
        return;

      // CONFIRMAR
      case 1:
        lcd.clear();

        switch (i) {
          // LIMITE
          case 0:
            setLimit("HUMIDITY");
            break;
        };

        return;
        break;

      // ANTERIOR
      case 2:
        i = (--i < 0) ? 1 - 1 : i;
        redraw = true;
        break;

      // PRÓXIMA
      case 3:
        i = ++i % 1;
        redraw = true;
        break;
    }
  }
}

// = = = = = = = = = =

// CONFIGURAÇÕES DE LUMINOSIDADE
void configurationLuminosity() {
  int i = 0;
  bool redraw = true;
  int idioma = config.language - '0';

  while (true) {
    // Desenhar a tela
    if (redraw) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(getText(MSG_LUMINOSIDADE, idioma));

      lcd.setCursor(0, 1);
      lcd.print("<");

      char* text = getText(ConfigLuminosidade[i], idioma);
      printCentered(text, 1);

      lcd.setCursor(15, 1);
      lcd.print(">");

      redraw = false;
    }

    // Interação do usuário
    int botao = readButtons();
    switch (botao) {
      // VOLTAR
      case 0:
        lcd.clear();
        return;

      // CONFIRMAR
      case 1:
        lcd.clear();

        switch (i) {
          // LIMITE
          case 0:
            setLimit("LIGHT");
            break;
        };

        return;
        break;

      // ANTERIOR
      case 2:
        i = (--i < 0) ? 1 - 1 : i;
        redraw = true;
        break;

      // PRÓXIMA
      case 3:
        i = ++i % 1;
        redraw = true;
        break;
    }
  }
}

// = = = = = = = = = =

// LIMITE
void setLimit(char* limitFor) {
  bool redraw = true;
  int idioma = config.language - '0';
  int i = 0;

  // Posição dos valores no LCD
  int posMin = 4;
  int posMax = 13;

  // Valores atuais
  int actualMin;
  int actualMax;

  // Novos valores
  char newMin[4];
  char newMax[4];

  // Obter o valor do limite minimo atual
  if (strcmp(limitFor, "TEMPERATURE") == 0) {
    actualMin = config.tempMin;
    actualMax = config.tempMax;
  } else if (strcmp(limitFor, "HUMIDITY") == 0) {
    actualMin = config.humidMin;
    actualMax = config.humidMax;
  } else if (strcmp(limitFor, "LIGHT") == 0) {
    actualMin = config.luminMin;
    actualMax = config.luminMax;
  }

  // Copiar valores reais para os novos
  newMin[0] = (actualMin < 0) ? '-' : '+';
  newMin[1] = (actualMin / 100 / 10) + '0';
  newMin[2] = (actualMin % 10) + '0';
  newMin[3] = '\0';

  newMax[0] = (actualMax < 0) ? '-' : '+';
  newMax[1] = (actualMax / 100 / 10) + '0';
  newMax[2] = (actualMax % 10) + '0';
  newMax[3] = '\0';

  while (true) {
    // Desenha a Tela
    if (redraw) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(getText(MSG_LIMITE, idioma));

      // Exibir o valores
      lcd.setCursor(0, 1);
      lcd.print("MIN:");
      lcd.print(newMin);

      lcd.setCursor(9, 1);
      lcd.print("MAX:");
      lcd.print(newMax);

      // Posicionar o cursor
      int pos = (i < 3) ? posMin + i : posMax + (i - 3);
      lcd.setCursor(pos, 1);
      lcd.blink();

      redraw = false;
    }

    int botao = readButtons();
    switch (botao) {
      // CANCELAR
      case 0:
        lcd.clear();
        return;

      // PRÓXIMO DIGITO
      case 1:
        lcd.clear();
        lcd.noBlink();
        redraw = true;

        // Avançar para o próximo digito
        i++;

        // Se tiver chegado ao final da configuração
        if (i >= 6) {
          Serial.println(newMin);
          Serial.println(newMax);
          // CONTINUAR DEPOIS


          // Obter o valor do limite minimo atual
          if (strcmp(limitFor, "TEMPERATURE") == 0) {
            config.tempMin = atoi(newMin);
            config.tempMax = atoi(newMax);
          } else if (strcmp(limitFor, "HUMIDITY") == 0) {
            config.humidMax = atoi(newMin);
            config.humidMin = atoi(newMax);
          } else if (strcmp(limitFor, "LIGHT") == 0) {
            config.luminMin = atoi(newMin);
            config.luminMax = atoi(newMax);
          }

          return;
        }

        break;

      // AUMENTAR
      case 2:
        if ( i == 0 ) {
          newMin[0] = (newMin[0] == '+') ? '-' : '+';
        }
        else if (i < 3) {
          int digit = newMin[i] - '0';
          digit = (digit + 1 > 9) ? 0 : digit + 1;
          newMin[i] = digit + '0';
        }
        else if ( i == 3 ) {
          newMax[0] = (newMax[0] == '+') ? '-' : '+';
        }
        else {
          int digit = newMax[i - 3] - '0';
          digit = (digit + 1 > 9) ? 0 : digit + 1;
          newMax[i - 3] = digit + '0';
        }

        redraw = true;
        break;

      // DIMINUIR
      case 3:
        if ( i == 0 ) {
          newMin[0] = (newMin[0] == '+') ? '-' : '+';
        }
        else if (i < 3) {
          int digit = newMin[i] - '0';
          digit = (digit - 1 < 0) ? 9 : digit - 1;
          newMin[i] = digit + '0';
        }
        else if ( i == 3 ) {
          newMax[0] = (newMax[0] == '+') ? '-' : '+';
        }
        else {
          int digit = newMax[i - 3] - '0';
          digit = (digit - 1 < 0) ? 9 : digit - 1;
          newMax[i - 3] = digit + '0';
        }

        redraw = true;
        break;
    }
  }
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

// ESPERA
void showWaitScreen(char* message, int delayMillis) {
  lcd.clear();

  // Acende o led como amarelo
  led(255, 255, 0);

  int pos = (16 - strlen(message)) / 2;
  if (strlen(message) > 16) pos = 0;

  lcd.setCursor(pos, 0);
  lcd.print(message);

  lcd.setCursor(2, 1);
  lcd.print("[..........]");

  lcd.setCursor(3, 1);
  for (byte i = 0; i < 10; i++) {
    delay(delayMillis);
    lcd.print("=");
  }

  lcd.clear();
}






