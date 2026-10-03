#ifndef CONFIG_H
#define CONFIG_H

#include "Idiomas.h"
#include "Telas.h"

//#define UTC_OFFSET -3    // Ajuste de fuso horário para UTC-3

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// LISTAS

// Lista de configurações
extern Mensagem Configuracoes[];
extern const int NUM_CONFIGURACOES;

// Lista de configurações de data
extern Mensagem ConfigData[];

// Lista de configurações de hora
extern Mensagem ConfigHora[];

// Lista de configurações de temperatura
extern Mensagem ConfigTemperatura[];

// Lista de configurações de umidade
extern Mensagem ConfigUmidade[];

// Lista de configurações de luminosidade
extern Mensagem ConfigLuminosidade[];

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// FUNÇÕES

// Função para levar do menu até a opção de configuração selecionada
void configure(int i);

#endif