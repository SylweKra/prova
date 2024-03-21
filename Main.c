#include <stdio.h>
#include "gamelib.h"
#include <time.h>
#include <stdlib.h>




                                                                                                                            


int main() {

    int scelta;

    int gioco_impostato = 0; // Flag per tenere traccia se il gioco è stato impostato

    int num_giocatori;

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









    return 0;

}



