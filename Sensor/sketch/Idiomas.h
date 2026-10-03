#ifndef IDIOMAS_H
#define IDIOMAS_H

#define NUM_IDIOMAS 3

// Lista de idiomas
extern const char* Idiomas[NUM_IDIOMAS];

// Enum para mensagens
enum Mensagem {
  MSG_BEMVINDO,

  MSG_CONFIGURACAO,

  MSG_IDIOMA,
  
  MSG_DATA,
  MSG_FORMATO_DATA,
  MSG_DEFINIR_DATA,

  MSG_HOUR,
  MSG_HORA,
  MSG_FORMATO_HORA,
  MSG_DEFINIR_HORA,

  MSG_TEMPERATURA,
  MSG_UMIDADE,
  MSG_LUMINOSIDADE,


  MSG_ESCALATEMP,
  MSG_LIMITE,
  MSG_MAXIMO,
  MSG_MINIMO,

  MSG_SALVO,
  
  TOTAL_MENSAGENS
};

// Função para obter um texto
const char* getText(Mensagem msg, int i);

#endif
