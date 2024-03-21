#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "gamelib.h"

#define NUM_FACCE_DADO 6
// Strutture dati globali per la mappa di gioco
static Zona_segrete *pFirst = NULL;
static Zona_segrete *pLast = NULL;
static Giocatore *giocatori[4];
static Abitante *abitante;
// gioca
void dimezza_dadi_difesa(Giocatore *giocatore);
void stampa_abitante(Abitante *abitante);
void genera_abitante();
int avanza(Giocatore *giocatore,int coso);
int indietreggia(Giocatore *giocatore,int coso);
void stampa_giocatore(Giocatore *giocatore);
void stampa_zona(Zona_segrete *zona);
int apri_porta(Giocatore *giocatore,int coso);
void prendi_tesoro(Giocatore *giocatore);
void scappa(Giocatore *giocatore,Abitante *abitante);
int combatti(Giocatore *giocatore, Abitante *abitante);
int risolveAttacco(Giocatore *giocatore, Abitante *abitante, int attaccante);
void gioca_potere_speciale(Giocatore *giocatore);
int lancia_dado();


// mappa
void inserisci_zona();
void genera_zona();
void genera_mappa();
void genera_porta(Zona_segrete *zona);
void genera_tesori(Zona_segrete *zona);
void genera_tipo(Zona_segrete *zona);
static void stampa_tipo_zona(Tipo_zona tipo);
void prendi_tesoro(Giocatore *giocatore);
void stampa_mappa(Zona_segrete *zona);
void cancella_zona();
void elimina_mappa(Zona_segrete *zona);
int count_zone();

// Funzioni ausiliarie
void menu ();
static int random_int(int min, int max)
{
  return min + rand() % (max - min + 1);
}

int lancia_dado()
{
  return rand() % NUM_FACCE_DADO + 1;
}

static void stampa_tipo_zona(Tipo_zona tipo)
{
  switch (tipo)
  {
  case corridoio:
    printf("Corridoio");
    break;
  case scala:
    printf("Scala");
    break;
  case sala_banchetto:
    printf("Sala banchetto");
    break;
  case magazzino:
    printf("Magazzino");
    break;
  case giardino:
    printf("Giardino");
    break;
  case posto_guardia:
    printf("Posto guardia");
    break;
  case prigione:
    printf("Prigione");
    break;
  case cucina:
    printf("Cucina");
    break;
  case armeria:
    printf("Armeria");
    break;
  case tempio:
    printf("Tempio");
    break;
  default:
    printf("Tipo di zona non valido");
    break;
  }
}
void genera_abitante()
{
  abitante = (Abitante *)malloc(sizeof(Abitante));
  abitante->nome_abitante = rand() % NumNomiAbitanti; // Genera un nome casuale tra quelli disponibili
  abitante->p_vita = rand() % 10 + 5;                 // Genera punti vita casuali tra 1 e 10
  abitante->dadi_difesa = rand() % 3 + 1;             // Genera dadi difesa casuali tra 1 e 4
  abitante->dadi_attacco = rand() % 4 + 1;            // Genera dadi attacco casuali tra 1 e 4
}
void stampa_abitante(Abitante *abitante)
{
  switch (abitante->nome_abitante)
  {
  case Gino:
    printf("Gino\n");
    break;
  case Il_re_dei_sette_nani:
    printf("Il re dei sette nani\n");
    break;
  case Carciofo:
    printf("Carciofo\n");
    break;
  case Tiziano:
    printf("Tiziano\n");
    break;
  default:
    printf("Nome non valido\n");
    break;
  }
  printf("Punti vita: %d\n", abitante->p_vita);
  printf("Dadi difesa: %d\n", abitante->dadi_difesa);
  printf("Dadi attacco: %d\n", abitante->dadi_attacco);
}
// Funzioni richieste
int imposta_gioco()
{
  int num_giocatori;
  printf("Inserisci il numero di giocatori (da 1 a 4): ");
  scanf("%d", &num_giocatori);
  // Allocazione dinamica dell'array dei giocatori
  for (int i = 0; i < num_giocatori; i++)
  {
    giocatori[i] = (Giocatore *)malloc(sizeof(Giocatore));
    printf("Giocatore %d\n", i + 1);
    printf("Inserisci il nome del giocatore: ");
    scanf("%s", giocatori[i]->nome_giocatore);
    printf("Scegli la classe (0: barbaro, 1: nano, 2: elfo, 3: mago): ");
    int scelta_classe;
    scanf("%d", &scelta_classe);
    switch (scelta_classe)
    {
    case 0:
      giocatori[i]->classe = 0;
      giocatori[i]->dadi_attacco = 3;
      giocatori[i]->dadi_difesa = 4;
      giocatori[i]->p_vita = 1;
      giocatori[i]->mente = random_int(1, 2);
      giocatori[i]->potere_speciale = 1;

      break;
    case 1:
      giocatori[i]->classe = 1;
      giocatori[i]->dadi_attacco = 4;
      giocatori[i]->dadi_difesa = 4;
      giocatori[i]->p_vita = 15;
      giocatori[i]->mente = random_int(2, 3);
      giocatori[i]->potere_speciale = 1;
      break;
    case 2:
      giocatori[i]->classe = 2;
      giocatori[i]->dadi_attacco = 3;
      giocatori[i]->dadi_difesa = 3;
      giocatori[i]->p_vita = 10;
      giocatori[i]->mente = random_int(3, 4);
      giocatori[i]->potere_speciale = 1;

      break;
    case 3:
      giocatori[i]->classe = 3;
      giocatori[i]->dadi_attacco = 5;
      giocatori[i]->dadi_difesa = 4;
      giocatori[i]->p_vita = 15;
      giocatori[i]->mente = random_int(4, 5);
      giocatori[i]->potere_speciale = 2;
      break;
    default:
      printf("Classe non valida\n");
      break;
    }
  }

  // Creazione della mappa di gioco
  int creazione_mappa = 0;
  do
  {
    // Menu per la creazione della mappa
    printf("\nMenu di creazione della mappa:\n");
    printf("1) Genera mappa\n");
    printf("2) Inserisci zona\n");
    printf("3) CancellA zona\n");
    printf("4) Stampa mappa\n");
    printf("5) Chiudi mappa\n");
    printf("Scelta: ");
    int scelta;
    while (!scanf("%d", &scelta))
    {
      while (getchar() != '\n')
      {
      }
      printf("invalid");
      printf("\nDIO MERDA\n");
      printf("1) Genera mappa\n");
      printf("2) Inserisci zona\n");
      printf("3) Cancella zona\n");
      printf("4) Stampa mappa\n");
      printf("5) Chiudi mappa\n");
      printf("Scelta: ");
      printf("%p",pFirst);
    }


    switch (scelta)
    {
    case 1:
      if (creazione_mappa == 1)
      {
        break;
      }
        genera_mappa();
      
      
      printf("Generazione della mappa...\n");
      creazione_mappa = 1;

      break;
    case 2:
      printf("Inserimento di una nuova zona...\n");
      inserisci_zona();
      break;
    case 3:
      printf("Cancellazione di una zona...\n");
      cancella_zona();
      break;
    case 4:
      printf("Stampa della mappa...\n");
      stampa_mappa(pFirst);
      break;
    case 5:
      printf("Chiusura della mappa...\n");
      if (!creazione_mappa)
      {
        printf("La mappa non è stata creata\n");
        continue;
      }
      return num_giocatori;
    default:
      printf("Scelta non valida\n");
      break;
    }
  } while (1);

  // Liberazione della memoria allocata per gli array dei giocatori
  free(giocatori);
}

void gioca(int num_giocatori)
{

  if (!pFirst)
  {
    printf("Il gioco non è stato impostato correttamente.\n");
    return;
  }

  // Implementazione della fase di gioco
  printf("\t\t\t\t\tIl gioco è iniziato!\n");
  int ordine[num_giocatori];
  for (size_t i = 0; i < num_giocatori; i++)
  {
    ordine[i] = i + 1;
    giocatori[i]->posizione = pFirst;
  }
  // I giocatori sono rappresentati dai numeri 1, 2, 3, 4
  for (int i = num_giocatori - 1; i > 0; i--)
  {
    int j = rand() % (i + 1);
    int temp = ordine[i];
    ordine[i] = ordine[j];
    ordine[j] = temp;
  }

  // Esecuzione dei turni dei giocatori
  int conta = 0;
  for (int i = 0;; i++)
  {
    Giocatore *giocatore_corrente = giocatori[ordine[i % num_giocatori] - 1];
    for (int i = 0; i < num_giocatori; i++)
    {
      if(giocatori[i] == NULL){
        conta++;
      }
      if(conta == num_giocatori){
        menu();
      }

    }
    if(giocatore_corrente == NULL)
      continue;
    
    printf("\nTurno del giocatore %d\n\n", ordine[i % num_giocatori]);
    // Esegui le azioni del giocatore
    
    int scelta;
    int coso = 0;
    
    do
    {
     
      printf("---------------\n\n");
      printf("\nAzioni disponibili:\n");
      printf("1) Avanza\n");
      printf("2) Indietreggia\n");
      printf("3) Stampa giocatore\n");
      printf("4) Stampa zona\n");
      printf("5) Apri porta\n");
      printf("6) Prendi tesoro\n");
      printf("7) Scappa\n");
      printf("8) Combatti\n");
      printf("9) Usa abilità speciale\n");
      printf("10) Passa\n");

      printf("Scegli un'azione: ");
     
      while (!scanf(" %d", &scelta))
      {
        
        while (getchar() != '\n')
        {
        }printf("scelta non valida");
        printf("---------------\n\n");
        
        printf("\nAzioni disponibili:\n");
        printf("1) Avanza\n");
        printf("2) Indietreggia\n");
        printf("3) Stampa giocatore\n");
        printf("4) Stampa zona\n");
        printf("5) Apri porta\n");
        printf("6) Prendi tesoro\n");
        printf("7) Scappa\n");
        printf("8) Combatti\n");
        printf("9) Usa abilità speciale\n");
        printf("10) Passa\n");
        printf("Scegli un'azione: ");
        
      }

      switch (scelta)
      {
      case 1:
        coso = avanza(giocatore_corrente,coso);
        break;
      case 2:
       coso = indietreggia(giocatore_corrente,coso);
        break;
      case 3:
        stampa_giocatore(giocatore_corrente);
        break;
      case 4:
        stampa_zona(giocatore_corrente->posizione);
        if (coso ==1){
          stampa_abitante(abitante);
        }else{}
        break;
      case 5: 
        coso = apri_porta(giocatore_corrente,coso);
        break;
      case 6:
        prendi_tesoro(giocatore_corrente);
        break;
      case 7:
      if(coso == 1){
        scappa(giocatore_corrente,abitante);
        coso = 0;}else{}
        
        break;
      case 8:
      if (coso == 1){
        combatti(giocatore_corrente,abitante);
        if(giocatore_corrente->p_vita <= 0){
          free(giocatore_corrente);
          giocatori[ordine[i % num_giocatori] - 1] =NULL;
          scelta = 10;
        }else{coso = 0;}
       }
        break;
      case 9:
      if (coso == 1 && giocatore_corrente->potere_speciale >=1)
      {
        gioca_potere_speciale(giocatore_corrente);
        coso = 0;
      }else{
         printf("non hai più il potere speciale");
      }
      break;
      default:
        printf("Scelta non valida\n");
        break;
        

      }
     
    } while (scelta != 10 ); //finchè non passa
     
  }
}

int indietreggia(Giocatore *giocatore,int coso)
{
  // Controlla se il giocatore può tornare indietro
  if (giocatore->posizione->zona_precedente != NULL)
  {
    // Torna indietro nella stanza precedente
    giocatore->posizione = giocatore->posizione->zona_precedente;

    printf("Sei tornato nella stanza precedente:\n");
    stampa_zona(giocatore->posizione);

    // 33% di probabilità che appaia un abitante delle segrete
    if (rand() % 3 == 0)
    {
      printf("Nella stanza precedente è apparso un abitante delle segrete!\n");
      genera_abitante(abitante);
      printf("Un pericoloso abitante compare: ");
      stampa_abitante(abitante);
      return coso = 1;
    
    }
  }
  else
  {
    printf("Non puoi tornare più indietro. Sei nella prima stanza delle segrete.\n");
  }
}

int avanza(Giocatore *giocatore,int coso)
{
  if(coso ==1){printf("non puoi andare avanti prima abbatti il nemico!");return coso =1;}

  // Avanza nella stanza successiva
  if (giocatore->posizione->zona_successiva != NULL)
  {
    // Controlla se c'è una porta nella stanza successiva
    if (giocatore->posizione->zona_successiva->porta != nessuna_porta)
    {}

    // Avanza nella stanza successiva
    giocatore->posizione = giocatore->posizione->zona_successiva;

    printf("Sei avanzato in una nuova zona:\n");
    stampa_zona(giocatore->posizione);

    // 33% di probabilità che appaia un abitante delle segrete
    if (rand() % 3 == 0)
    {
      genera_abitante(abitante);
      printf("Un pericoloso abitante compare: ");
      stampa_abitante(abitante);
      return coso = 1;
    }
  }
  else
  {
    printf("Sei già nell'ultima zona delle segrete.\n");
    // Nell'ultima stanza delle segrete appare sempre un abitante
    printf("Nell'ultima stanza delle segrete è apparso un abitante!\n");
    genera_abitante(abitante);
    printf("Un pericoloso abitante compare: ");
    stampa_abitante(abitante);
    int win = combatti(giocatore,abitante);
     if(win != 0){
      printf("GIOCATORE %s HAI VINTO!\n",giocatore->nome_giocatore); 
      while (getchar() != '\n'){}
      menu();
    }else{
      printf("GIOCATORE %s HAI PERSO!\n",giocatore->nome_giocatore); 
       while (getchar() != '\n'){}
       menu();
    }
    return coso = 1;
  }

}

int apri_porta(Giocatore *giocatore,int coso)
{
  // Controlla il tipo di porta
  switch (giocatore->posizione->porta)
  {
  case porta_normale:
    printf("La porta è normale e può essere aperta senza problemi.\n");
    printf("La porta è stata aperta con successo!\n");
    break;
  case porta_da_scassinare:
    printf("La porta deve essere scassinata.\n");
    // Effettua un tiro di dado per scassinare la porta
    int dado = lancia_dado();
    printf("Risultato del tiro di dado: %d\n", dado);
    // Controlla se il risultato è minore o uguale alla caratteristica mente del giocatore
    if (dado <= giocatore->mente)
    {
      printf("La porta è stata scassinata con successo!\n");
    }
    else
    {
      // Calcola le probabilità di conseguenza negativa
      int probabilita = rand() % 100;
      if (probabilita < 10)
      {
        // 10% di probabilità di riniziare il percorso dalla prima stanza delle segrete
        printf("Hai sbagliato e devi riniziare il percorso dalla prima stanza delle segrete.\n");
        giocatore->posizione = pFirst;
        stampa_zona(giocatore->posizione);
      }
      else if (probabilita < 60)
      {
        // 50% di probabilità di perdere un punto vita
        giocatore->p_vita--;
        printf("Hai perso un punto vita.\n");
      }
      else
      {
        // 40% di probabilità che appaia un abitante delle segrete
        printf("Nella stanza è apparso un abitante delle segrete!\n");
         genera_abitante(abitante);
    printf("Un pericoloso abitante compare: ");
    stampa_abitante(abitante);
    return coso = 1;
    
      }
    }
    break;
  case nessuna_porta:
    printf("Non c'è nessuna porta da aprire.\n");
    break;
  }
}

void stampa_giocatore(Giocatore *giocatori)
{
  printf("\t\t\t---------------\n");
  printf("\t\t\tNome: %s\n", giocatori->nome_giocatore);
  printf("\t\t\t---------------\n");
  printf("\t\t\tClasse: ");
  switch (giocatori->classe)
  {
  case barbaro:
    printf("Barbaro\n");
    break;
  case nano:
    printf("Nano\n");
    break;
  case elfo:
    printf("Elfo\n");
    break;
  case mago:
    printf("Mago\n");
    break;
  default:
    printf("Classe non definita\n");
    break;
  }
  printf("\t\t\t---------------\n");
  printf("\t\t\tPunti Vita: %d\n", giocatori->p_vita);
  printf("\t\t\tDadi Attacco: %d\n", giocatori->dadi_attacco);
  printf("\t\t\tDadi Difesa: %d\n", giocatori->dadi_difesa);
  printf("\t\t\tMente: %d\n", giocatori->mente);
  printf("\t\t\tPotere Speciale: %d\n", giocatori->potere_speciale);
  printf("\t\t\t---------------\n");
}

void dimezza_dadi_difesa(Giocatore *giocatore)
{
  // Calcola il numero di dadi per la difesa
  
 double risultato = floor(giocatore->dadi_difesa / 2.0);
 giocatore ->dadi_difesa =risultato;
}

void scappa(Giocatore *giocatore,Abitante *abitante)
{
  if(giocatore -> dadi_difesa<2){
    printf("non hai abbastanza dadi per saltare");
  }
  int fattore = giocatore->mente;
  if (lancia_dado() < fattore)
  {
    printf("il giocatore è riuscito a scappare!\n");
    // Controlla se il giocatore può tornare indietro
    if (giocatore->posizione->zona_precedente != NULL)
    {
      // Torna indietro nella stanza precedente
      giocatore->posizione = giocatore->posizione->zona_precedente;
      printf("Sei tornato nella stanza precedente:\n");
      stampa_zona(giocatore->posizione);
    }
  }
  else
  {
    dimezza_dadi_difesa(giocatore);
    printf("non sei riuscito a scappare\n");
    printf("i tuoi dadi difesa sono stati dimezzati  Dadi Difesa:%d\n",giocatore->dadi_difesa);
    printf("l'abitante attacca\n");
    
    int scudo = 0;
  printf("il giocatore si difende\n");
    for (int i = 0; (i < giocatore->dadi_difesa && scudo != 1) ; i++)
  {
    
    switch (lancia_dado())
    {
    case 1:
    case 2:
    case 3:
        printf("esce teschio!\n");
         printf("sei stato colpito\n");
         giocatore ->p_vita--;
         break;
    case 4:
    case 5:
        printf("esce ScudoBianco \n");
        printf("hai parato il colpo \n");
        scudo++;
        break;
    case 6:    
        printf("esce ScudoNero\n"); 
        printf("sei stato colpito\n");
        giocatore ->p_vita--;
        break;
      
    }
  }
   
  }
}
void gioca_potere_speciale(Giocatore *giocatore)
{
  if (giocatore->potere_speciale >= 1)
  {
    printf("Il giocatore usa il potere speciale!\n");
    printf("L'abitante  viene sconfitto istantaneamente.\n");
  // Azzeramento dei punti vita dell'abitante
  abitante->p_vita = 0;
  giocatore->potere_speciale = 0;
  }else{
    printf("non hai più il potere speciale");
  }
  
  
}


void stampa_mappa(Zona_segrete *zona)
{
  int i = 1;
  Zona_segrete *pTemp;
  pTemp = pFirst;
  while (pTemp != NULL)
  {
    printf("-----------%d------------\n", i);
    printf("------------------------\n");
    stampa_zona(pTemp);
    printf("------------------------\n");

    pTemp = pTemp->zona_successiva;
    i++;
  } 
}
void stampa_zona(Zona_segrete *zona)
{
  printf("Tipo di Zona: ");
  stampa_tipo_zona(zona->tipo);
  printf("\n");
  printf("Presenza di tesoro: %s\n", zona->tesoro == nessun_tesoro ? "No" : "Sì");
  printf("Presenza di porta: %s\n", zona->porta == nessuna_porta ? "No" : "Sì");
}
void genera_mappa()
{
  for (int i = 0; i < 15; i++)
  {
    genera_zona();
  }
}
void genera_zona(){
  
 Zona_segrete *zona = malloc(sizeof(Zona_segrete));

  if (zona == NULL) {
    printf("error allocaing memory");
    return;
  }
  zona->zona_precedente = NULL;
  zona->zona_successiva = NULL;

  if (pFirst == NULL)
  {
    pFirst = zona;

  }
  else
  {
    
      pLast->zona_successiva = zona;
      zona->zona_precedente = pLast;
    
  
  }
    pLast = zona;
  genera_porta(zona);
  genera_tesori(zona);
  genera_tipo(zona);


}
void prendi_tesoro(Giocatore *giocatore)
{
  // Ottieni un riferimento alla stanza in cui si trova il giocatore
  Zona_segrete *stanza_corrente = giocatore->posizione;

  // Controlla se c'è un tesoro nella stanza
  if (stanza_corrente->tesoro != nessun_tesoro)
  {
    // Aggiorna lo stato del giocatore in base al tipo di tesoro
    switch (stanza_corrente->tesoro)
    {
    case veleno:
      printf("Hai trovato del veleno! Perdi due punti vita.\n");
      giocatore->p_vita -= 2;
      break;
    case guarigione:
      printf("Hai trovato un'pozione di guarigione! Guadagni un punto vita.\n");
      giocatore->p_vita += 1;
      break;
    case doppia_guarigione:
      printf("Hai trovato una pozione di guarigione potenziata! Guadagni due punti vita.\n");
      giocatore->p_vita += 2;
      break;
    default:
      printf("Tipo di tesoro non valido.\n");
      break;
    }

    // Riporta lo stato del tesoro nella stanza a "nessun tesoro"
    stanza_corrente->tesoro = nessun_tesoro;
  }
  else
  {
    printf("Non c'è tesoro da prendere in questa stanza.\n");
  }
}
void genera_tipo(Zona_segrete *zona)
{
  int prob = rand() % 100;

  if (prob < 10)
  {
    zona->tipo = armeria;
  }
  else if (prob < 20)
  {
    zona->tipo = magazzino;
  }
  else if (prob < 25)
  {
    zona->tipo = tempio;
  }
  else if (prob < 30)
  {
    zona->tipo = cucina;
  }
  else if (prob < 35)
  {
    zona->tipo = posto_guardia;
  }
  else if (prob < 40)
  {
    zona->tipo = sala_banchetto;
  }
  else if (prob < 60)
  {
    zona->tipo = scala;
  }
  else if (prob < 15)
  {
    zona->tipo = prigione;
  }
  else if (prob < 35)
  {
  }
}
void genera_tesori(Zona_segrete *zona)
{
  int prob = rand() % 100;
  if (prob < 40)
  {
    zona->tesoro = guarigione;
  }
  else if (prob < 45)
  {
    zona->tesoro = veleno;
  }
  else if (prob < 10)
  { 
    zona->tesoro = doppia_guarigione;
  }
}
void genera_porta(Zona_segrete *zona)
{
  int prob = rand() % 100;
  if (prob < 33)
  {
    zona->porta = porta_normale;
  }
  else if (prob < 60)
  {
    zona->porta = porta_da_scassinare;
  }
  else if (prob < 33)
  {
    zona->porta = nessuna_porta;
  }
}
void cancella_zona()
{
  int posizione;
  printf("in che posizione\n");
  while (!scanf("%d", &posizione))
  {
    while (getchar() != '\n')
    {
    }
    printf("invalid\n");
    printf("in che posizione\n");
  }
  if (posizione < 0 || posizione >= count_zone())
  {
    printf("errore posizione non valida\n");
    return;
  }
  if (pFirst == NULL)
  {
    printf("non c'è nulla da cancellare la lista è vuota\n");
    return;
  }
  // Cancellazione in testa
  if (posizione == 1)
  {
    Zona_segrete *temp = pFirst;
    pFirst = pFirst->zona_successiva;
    if (pFirst != NULL)
    {
      pFirst->zona_precedente = NULL;
    }
    free(temp);
    return;
  }
  // Cancellazione in coda
  if (posizione == count_zone())
  {
    Zona_segrete *temp = pLast;
    pLast = pLast->zona_precedente;
    pLast->zona_successiva = NULL;
    free(temp);
    return;
  }
}
void inserisci_zona()
{
  int posizione;
  printf("in che posizione\n");
  while (!scanf("%d", &posizione))
  {
    while (getchar() != '\n')
    {
    }
    printf("invalid\n");
    printf("in che posizione\n");
  }

  // Controllo se la posizione specificata è valida
  if (posizione < 0)
  {
    printf("Posizione non valida.\n");
    return;
  }
  Zona_segrete *zona = malloc(sizeof(Zona_segrete));
  genera_porta(zona);
  genera_tesori(zona);
  genera_tipo(zona);

  // Inserimento in testa
  if (posizione == 1)
  {
    zona->zona_successiva = pFirst;
    pFirst->zona_precedente = zona;
    pFirst = zona;
    printf("fatto!\n");
    return;
  }

  // Inserimento in coda
  if (posizione >= count_zone())
  {
    zona->zona_precedente = pLast;
    zona->zona_successiva = NULL;
    pLast->zona_successiva = zona;
    pLast = zona;
    return;
  }

  // Inserimento in posizione intermedia
  Zona_segrete *temp = pFirst;
  int count = 1;
  while (temp != NULL && count < posizione)
  {
    temp = temp->zona_successiva;
    count++;
  }
  if (temp == NULL)
  {
    printf("Errore nell'inserimento della zona.\n");
    return;
  }
  zona->zona_successiva = temp;
  zona->zona_precedente = temp->zona_precedente;
  temp->zona_precedente->zona_successiva = zona;
  temp->zona_precedente = zona;
  printf("------------------------\n");
  stampa_zona(zona);
  printf("------------------------\n");
}
// Funzione ausiliaria per contare il numero di zone segrete nella lista
int count_zone()
{
  int count = 0;
  Zona_segrete *temp = pFirst;
  while (temp != NULL)
  {
    count++;
    temp = temp->zona_successiva;
  }
  return count;
}


int combatti(Giocatore *giocatore, Abitante *abitante)
{

  while (giocatore->p_vita > 0 && abitante->p_vita > 0)
  {
    // Fase di Iniziativa
    int tiroGiocatore = lancia_dado();
    int tiroAbitante = lancia_dado();
    printf("Tiro giocatore: %d, Tiro abitante: %d\n", tiroGiocatore, tiroAbitante);

    int attaccante;
    if (tiroGiocatore >= tiroAbitante)
    {
      attaccante = 0; // Il giocatore attacca per primo
    
      printf("----------------------------------\n");
      printf("+ Il giocatore attacca per primo +\n");
      printf("----------------------------------\n");
      while (getchar() != '\n'){}
    }
    else
    {
      attaccante = 1; // L'abitante attacca per primo
      
      printf("\t\t\t\t\t--------------------------------\n");
      printf("\t\t\t\t\t+ L'abitante attacca per primo +\n");
      printf("\t\t\t\t\t--------------------------------\n");
      while (getchar() != '\n'){}
    }


    int dannoInflitto = 0;
    // Calcolo del danno
    dannoInflitto = risolveAttacco(giocatore, abitante, attaccante);

    if (attaccante == 0)
    {
      abitante->p_vita -= dannoInflitto;
      printf("\t\t\t\t\t--------------------------------------\n");
      printf("\t\t\t\t\t+l'abitante subisce %d danni. HP: %d +\n", dannoInflitto, abitante->p_vita);
      printf("\t\t\t\t\t--------------------------------------\n");
      while (getchar() != '\n'){}
    }
    else
    {
      giocatore->p_vita -= dannoInflitto;
      printf("-------------------------------------\n");
      printf("Il giocatore subisce %d danni. HP: %d\n", dannoInflitto, giocatore->p_vita);
      printf("-------------------------------------\n");
      while (getchar() != '\n'){}
    }
  }

  // Conclusione del combattimento
  if (giocatore->p_vita >= 1)
  {
    printf("------------------------\n");
    printf("+ Il giocatore vince!  +\n");
    printf("------------------------\n");
    return 1;
  }
  else
  {printf("\t\t\t\t\t------------------------\n");
    printf("\t\t\t\t\t+ L'abitante vince! + \n");
    printf("\t\t\t\t\t------------------------\n");

    
printf("GIOCATORE %s HAI PERSO!\n",giocatore->nome_giocatore); 
    
                                                                                                                                                       
    return 0;
  }

  
}

int risolveAttacco(Giocatore *giocatore, Abitante *abitante, int attaccante)
{
  int numTeschi = 0;
  int numScudiBianchi = 0;
  int numScudiNeri = 0;
  int danno = 0;
  // Conta i simboli sui dadi dell'attaccante

  int numDadiAtt = attaccante == 0 ? giocatore->dadi_attacco : abitante->dadi_attacco;
  int numDadiDiff = attaccante == 1 ? giocatore->dadi_difesa : abitante->dadi_difesa;
  for (int i = 0; i < numDadiAtt; i++)
  {

    switch (lancia_dado())
    {
    case 1:
    case 2:
    case 3:
      numTeschi++;
      if (attaccante == 1)
      {
        printf("\t\t\t\t\tesce teschio!\n");
      }
      else
      {
        printf("esce teschio!\n");
      }
      break;
    case 4:
    case 5:
      if (attaccante == 1)
      {printf("\t\t\t\t\tesce ScudoBianco attacco fallito\n");}
      else
      {printf("esce ScudoBianco attacco fallito \n"); }
      break;
    case 6:
      if (attaccante == 1)
      {
        printf("\t\t\t\t\tesce ScudoNero attacco fallito\n");
       
      }
      else
      {
        printf("esce ScudoNero attacco fallito \n");
        
      }
      break;
    }
  }
  // Conta i simboli sui dadi del difensore
  for (int i = 0; i < numDadiDiff; i++)
  {
    if (numTeschi == 0)
    {
      break;
    }
    switch (lancia_dado())
    {
    case 1:
    case 2:
    case 3:
      break;
    case 4:
    case 5:
      if (attaccante == 1)
      {
        numScudiBianchi++;
         printf("------------------------\n");
    printf("+ Il giocatore si difende!  +\n");
    printf("------------------------\n");
        printf("esce ScudoBianco\n");
      }
      else
      printf("\t\t\t\t\t----------------------------\n");
    printf("\t\t\t\t\t+ L'abitante non si difende  +\n");
    printf("\t\t\t\t\t----------------------------\n");
      {printf("\t\t\t\t\tesce ScudoBianco\n");}
      
      
      
      break;
    case 6:
      if (attaccante == 0)
      {
    printf("\t\t\t\t\t------------------------\n");
    printf("\t\t\t\t\t+ L'abitante si difende  +\n");
    printf("\t\t\t\t\t------------------------\n");
        printf("\t\t\t\t\tesce ScudoNero \n");
        numScudiNeri++;
      }
      
      if (attaccante == 1)
      {
         printf("----------------------------\n");
    printf("+ Il Giocatore non si difende  +\n");
    printf("----------------------------\n");
        printf("esce ScudoNero \n");
      }
      break;
    }
  }
  printf("\t\t\t\t\t------------------------\n");
  // Calcola il danno inflitto dopo aver considerato la difesa
  if (attaccante == 1)
  {
    danno = numTeschi - numScudiBianchi;
  }
  else
  {
    danno = numTeschi - numScudiNeri;
  }
  return danno > 0 ? danno : 0;
}



void menu (){
   printf("RESET COMPLETO\n");
    int scelta;
    int gioco_impostato = 0; // Flag per tenere traccia se il gioco è stato impostato
    int num_giocatori;
    elimina_mappa(pFirst);
    for (int i = 0; i < 4; i++)
    {
      free(giocatori[i]);
    }
    
    do {

printf("\n\n.d8888.  .o88b.  .d8b.  db       .d88b.   d888b  d8b   db  .d8b.          .d88b.  db    db d88888b .d8888. d888888b \n");
printf("88'  YP d8P  Y8 d8' `8b 88      .8P  Y8. 88' Y8b 888o  88 d8' `8b        .8P  Y8. 88    88 88'     88'  YP `~~88~~' \n");
printf("  `Y8b. 8b      88~~~88 88      88    88 88  ooo 88 V8o88 88~~~88 C8888D 88    88 88    88 88~~~~~   `Y8b.    88 \n");   
printf("db   8D Y8b  d8 88   88 88booo. `8b  d8' 88. ~8~ 88  V888 88   88        `8P  d8' 88b  d88 88.     db   8D    88   \n ");
printf("8888Y'  `Y88P' YP   YP Y88888P  `Y88P'   Y888P  VP   V8P YP   YP         `Y88'Y8 ~Y8888P' Y88888P `8888Y'    YP  \n");

        printf("\n\t\t\t\t\t\tMenu di scelta:\n\n");

        printf("\t\t\t\t\t1) Imposta gioco\n");

        printf("\t\t\t\t\t2) Gioca\n");

        printf("\t\t\t\t\t3) Termina gioco\n");

        printf("\t\t\t\t\tScelta: ");

        if (scanf("%d", &scelta)){

          switch (scelta) {

              case 1:

                  // Impostare il gioco solo se non è già stato impostato

                  if (!gioco_impostato) {

                      num_giocatori=imposta_gioco();

                      gioco_impostato = 1;

                    

                  } else {

                      printf("Il gioco è già stato impostato.\n");

                  }

                  break;

              case 2:

                  // Giocare solo se il gioco è stato impostato

                  if (gioco_impostato) {

                      gioca(num_giocatori);

                  } else {

                      printf("Prima di giocare, impostare il gioco.\n");

                  }

                  break;

              case 3:

                  // Terminare il gioco

                  

                      termina_gioco();

                  break;

              default:

                  printf("Comando non valido. Si prega di inserire 1, 2 o 3.\n");

          }

        }

        else{

          while(getchar()!='\n'){}printf("invalid");

        }

      } while (scelta != 3);









    return;

}

void elimina_mappa(Zona_segrete *zona){
  Zona_segrete *temp;

    // Ciclo per scorrere la lista
    while (zona != NULL) {
      // Memorizzazione del prossimo nodo
      temp = zona;
       zona = zona->zona_successiva;

      // Deallocazione del nodo corrente
      free(temp);
     // zona->zona_precedente =NULL;
      //zona->zona_successiva =NULL;
      //zona = NULL;
      // Assegnazione del prossimo nodo come nodo corrente
     

    }
    pFirst =NULL;
    pLast =NULL;
   
    
}
void termina_gioco(){
  printf("|----------------------------------------|\n");
  printf("\t\tArrivederci grazie per aver giocato\n");
   printf("|----------------------------------------|\n");
  exit(0);
}