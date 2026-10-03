#include "Perifericos.h"

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PERIFÉRICOS

// BOTOES
Button buttons[] = {
  {4, LOW}, // 0 -> POWER
  {5, LOW}, // 1 -> SETUP
  {6, LOW}, // 2 -> DOWN
  {7, LOW}  // 3 -> UP
};

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT
DHT dht(DHTPIN, DHTTYPE);

// RTC
RTC_DS1307 RTC;

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// VALORES DOS SENSORES
extern float TEMPERATURE = -1.0;
extern float HUMIDITY = -1.0;
extern float LIGHT = -1.0;

// DATA E HORA ATUAIS
DateTime currentTime = RTC.now();

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// FUNÇÕES

// FUNÇÃO PARA INICIALIZAR OS PERIFÉRICOS
void initializeComponents() {
  // BOTOES
  for (Button botao : buttons) {
    pinMode(botao.pin, INPUT_PULLUP);
  }

  // BUZZER
  pinMode(BUZZERPIN, OUTPUT);

  // LED RGB
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  // lcd.backlight();

  // DHT
  dht.begin();

  // RTC
  RTC.begin();

  // LCD
  lcd.init();
  lcd.createChar(0, thermometer);
  lcd.createChar(1, drop);
  lcd.createChar(2, sun);

}

// FUNÇÃO PARA ATUALIZAR A HORA ATUAL
void updateTime() {
  DateTime now = RTC.now();

  // Calculando o deslocamento do fuso horário
  //int offsetSeconds = UTC_OFFSET * 3600;  // Convertendo horas para segundos
  //now = now.unixtime() + offsetSeconds;   // Adicionando o deslocamento ao tempo atual

  // Atualizando a hora atual
  currentTime = DateTime(now);
}

// FUNÇÃO PARA OBTER A HORA ATUAL
DateTime getTime() {
  updateTime();
  return currentTime;
}

// FUNÇÃO PARA ALTERAR A DATA ATUAL
void alterDate(int day, int month, int year) {
  DateTime now = getTime();
  RTC.adjust(DateTime(year, month, day, now.hour(), now.minute(), now.second()));
}

// FUNÇÃO PARA ALTERAR A HORA ATUAL
void alterTime(int hour, int minute) {
  DateTime now = getTime();
  RTC.adjust(DateTime(now.year(), now.month(), now.day(), hour, minute, 0));
}

// FUNÇÃO PARA VERIFICAR SE O BOTÃO PARA LIGAR FOI PRESSIONADO
bool readButtonPower() {
  int currentValue = digitalRead(buttons[0].pin);

  if (currentValue != buttons[0].oldValue) {
    // Debounce
    delay(20);

    currentValue = digitalRead(buttons[0].pin);

    if (currentValue != buttons[0].oldValue) {
      buttons[0].oldValue = currentValue;

      if (currentValue == LOW) {
        return true;
      }
    }
  }

  return false;
}

// FUNÇÃO PARA VERIFICAR SE UM BOTÃO FOI PRESSIONADO
int readButtons() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    int currentValue = digitalRead(buttons[i].pin);

    if (currentValue != buttons[i].oldValue) {
      // Debounce
      delay(20);

      currentValue = digitalRead(buttons[i].pin);

      if (currentValue != buttons[i].oldValue) {
        buttons[i].oldValue = currentValue;

        if (currentValue == LOW) {
          buzzer(1000, 100);
          // Retornar o índice do botão
          return i;
        }
      }
    }
  }

  return -1;
}

// FUNÇÃO PARA LER OS SENSORES
void readSensors() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

  // Ler os valores do DHT
  TEMPERATURE = dht.readTemperature();
  HUMIDITY = dht.readHumidity();

  // Ler o valor do LDR
  int analogValue = analogRead(LDRPIN);              // valor entre 0 e 1023
  LIGHT = (1.0 - (analogValue / 1023.0)) * 100.0;    // Valor em %

  verifyAnomalies(TEMPERATURE, config.tempMax / 100.0, config.tempMin / 100.0, 1, 0);
  verifyAnomalies(HUMIDITY, config.humidMax / 100.0, config.humidMin / 100.0, 3, 1);
  verifyAnomalies(LIGHT, config.luminMax / 100.0, config.luminMin / 100.0, 5, 2);

  // = = = = = = =

  // Cabeçalho
  Serial.println("= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =");
  Serial.println("  VALORES ATUAIS ");
  Serial.println("=======================================");

  // TEMPERATURA
  Serial.print("TEMPERATURA\t");
  Serial.print(TEMPERATURE, 1); // 1 casa decimal
  Serial.print("\t");
  Serial.print(config.tempMin / 100.0, 1);
  Serial.print(" - ");
  Serial.println(config.tempMax / 100.0, 1);

  // HUMIDITY
  Serial.print("HUMIDITY\t");
  Serial.print(HUMIDITY, 1);
  Serial.print("\t");
  Serial.print(config.humidMin / 100.0, 1);
  Serial.print(" - ");
  Serial.println(config.humidMax / 100.0, 1);

  // LIGHT
  Serial.print("LIGHT\t\t"); // duas tabs para alinhar
  Serial.print(LIGHT, 1);
  Serial.print("\t");
  Serial.print(config.luminMin / 100.0, 1);
  Serial.print(" - ");
  Serial.println(config.luminMax / 100.0, 1);

  Serial.println("= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =");
}

// Função para acender o LED RGB
void led(int r, int g, int b) {
  analogWrite(LED_R, r);
  analogWrite(LED_G, g);
  analogWrite(LED_B, b);
}

// Função para produzir um som no buzzer
void buzzer(int frequencia, int duracao) {
  tone(BUZZERPIN, frequencia, duracao);
}
