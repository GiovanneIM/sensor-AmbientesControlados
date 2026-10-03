#include "Config.h";

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// LISTAS

// Lista de configurações
Mensagem Configuracoes[] = {
  MSG_IDIOMA,
  MSG_DATA,
  MSG_HORA,
  MSG_TEMPERATURA,
  MSG_UMIDADE,
  MSG_LUMINOSIDADE
};
const int NUM_CONFIGURACOES = sizeof(Configuracoes) / sizeof(Configuracoes[0]);

// Lista de configurações de data
Mensagem ConfigData[] = {
  MSG_DEFINIR_DATA,
  MSG_FORMATO_DATA
};

// Lista de configurações de hora
Mensagem ConfigHora[] = {
  MSG_DEFINIR_HORA,
  MSG_FORMATO_HORA
};

// Lista de configurações de temperatura
Mensagem ConfigTemperatura[] = {
  MSG_LIMITE,
  MSG_ESCALATEMP
};

// Lista de configurações de umidade
Mensagem ConfigUmidade[] = {
  MSG_LIMITE
};

// Lista de configurações de luminosidade
Mensagem ConfigLuminosidade[] = {
  MSG_LIMITE
};

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// FUNÇÕES

// Função para levar do menu até a opção de configuração selecionada
void configure(int i) {
  switch (i) {
    // i = 0 → IDIOMA
    case 0:
      changeLanguage();
      break;
    // i = 1 → DATA
    case 1:
      configurationDataMenu();
      break;
    // i = 2 → HORA
    case 2:
      configurationHourMenu();
      break;
    // i = 3 → TEMPERATURA
    case 3:
      configurationTemperature();
      break;
    // i = 4 → UMIDADE
    case 4:
      configurationHumidity();
      break;
    // i = 5 → LUMINOSIDADE
    case 5:
      configurationLuminosity();
      break;
  }
}