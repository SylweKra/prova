#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#include <string.h>

#include <stdbool.h>

// Dichiarazioni delle funzioni definite in gamelib.c

int imposta_gioco();

void gioca(int num_gicoatori);

void termina_gioco();





// Definizioni dei tipi di dati utilizzati nella libreria



// Enumerazione delle classi dei giocatori

typedef enum {

    barbaro,

    nano,

    elfo,

    mago

} classe_giocatore;

typedef enum{

  Gino,

  Il_re_dei_sette_nani,

  Carciofo,

  Tiziano,

  NumNomiAbitanti

} NomeAbitante;

// Enumerazione dei tipi di zona

typedef enum {

    corridoio,

    scala,

    sala_banchetto,

    magazzino,

    giardino,

    posto_guardia,

    prigione,

    cucina,

    armeria,

    tempio

} Tipo_zona;



// Enumerazione dei tipi di tesoro

typedef enum {

    nessun_tesoro,

    veleno,

    guarigione,

    doppia_guarigione

} Tipo_tesoro;



// Enumerazione dei tipi di porta

typedef enum {

    nessuna_porta,

    porta_normale,

    porta_da_scassinare

} Tipo_porta;



// Definizione della struttura Giocatore

typedef struct Giocatore {

    char nome_giocatore[50];

    classe_giocatore classe;

    struct Zona_segrete* posizione;

    int p_vita;

    unsigned char dadi_attacco;

    unsigned char dadi_difesa;

    unsigned char mente;

    unsigned char potere_speciale;

} Giocatore;

//struttura abitante

typedef struct Abitante{

  NomeAbitante nome_abitante;

  int p_vita;

  unsigned char dadi_difesa;

  unsigned char dadi_attacco;

}Abitante;

// Definizione della struttura Zona_segrete

typedef struct Zona_segrete {

    struct Zona_segrete* zona_successiva;

    struct Zona_segrete* zona_precedente;

    Tipo_zona tipo;

    Tipo_tesoro tesoro;

    Tipo_porta porta;

} Zona_segrete;

