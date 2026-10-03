#include "Idiomas.h"

// Lista de idiomas
const char* Idiomas[NUM_IDIOMAS] = {
  "PORTUGUES",
  "ENGLISH",
  "ESPANOL"
};

// PORTUGUÊS
const char* const textosPT[TOTAL_MENSAGENS] = {
  "BEM-VINDO",

  "CONFIGURACAO",

  "IDIOMA",

  "DATA",
  "FORMATO DATA",
  "DEFINIR DATA",

  "HORA",
  "HORA",
  "FORMATO HORA",
  "DEFINIR HORA",

  "TEMPERATURA",
  "UMIDADE",
  "LUMINOSIDADE",

  "ESCALA TEMP",
  "LIMITE",
  "MAXIMO",
  "MINIMO",

  "SALVO"
};

// INGLÊS
const char* const textosENG[TOTAL_MENSAGENS] = {
  "WELCOME",

  "CONFIGURATION",
  "LANGUAGE",

  "DATE",
  "DATE FORMAT",
  "SET DATE",

  "HOUR",
  "TIME",
  "HOUR FORMAT",
  "SET TIME",

  "TEMPERATURE",
  "HUMIDITY",
  "LUMINOSITY",

  "TEMP SCALE",
  "LIMIT",
  "MAXIMUM",
  "MINIMUM",

  "SAVED"
};



// ESPANHOL
const char* const textosESP[TOTAL_MENSAGENS] = {
  "BIENVENIDO",

  "CONFIGURACION",
  "IDIOMA",

  "FECHA",
  "FORMATO FECHA",
  "DEFINIR FECHA",

  "HORA",
  "HORA",
  "FORMATO HORA",
  "DEFINIR HORA",

  "TEMPERATURA",
  "HUMEDAD",
  "LUMINOSIDAD",

  "ESCALA TEMP",
  "LIMITE",
  "MAXIMO",
  "MINIMO",

  "GUARDADO"
};





// Função para pegar um texto em um idioma
const char* getText(Mensagem msg, int i) {
  if (i >= NUM_IDIOMAS) {
    i = 0;
  }

  switch (i) {
    case 0: return textosPT[msg];
    case 1: return textosENG[msg];
    case 2: return textosESP[msg];
    default: return "";
  }
}