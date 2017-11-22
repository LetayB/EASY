//
// msg_recepteur.c
//
// --> Programme  lisant  MESSAGES_NB messages  de  taille
// MESSAGES_TAILLE dans la file de messages de cle CLE_BAL
//
// Auteur : FROGER Adrien - L3SPI - Grp 2
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#ifdef _LINUX_
#include <string.h>
#else
#include <strings.h>
#endif

#define CLE_BAL 42

#include <msg_messages.h>

int
main( int nb_arg , char * tab_arg[] )
{
    char nomprog[128] ;

    int file_id;

    int i;

    message_t reponse;

    struct timeval temps ;
    double temps_debut;
    double temps_fin;



    /*-----*/



    /* Verification du nombre d'arguments */
    if( nb_arg !=1 )
    {
        fprintf( stderr , "%s - Recepteur dans la communication par paquet\n\n" , tab_arg[0] );
        fprintf( stderr , "usage : %s \n" , tab_arg[0] );
        exit(-1);
    }
    strcpy( nomprog , tab_arg[0] );



    /* Recherche de la file de messages */
    if( (file_id = msgget ( CLE_BAL , 0 ) ) == -1 )
    {
        perror(" Probleme durant la recherche de la file");
        exit(-2);
    }

    printf( "\n---------- Debut reception %s ----------\n" , nomprog ) ;

    /* Recuperation de l'heure actuelle avec precision de l’ordre de la microseconde */
    gettimeofday(&temps, NULL);
    temps_debut = temps.tv_sec+(temps.tv_usec/1000000.0);


    /* Reception des MESSAGES_NB messages dans la file de messages */	
    for(i=0; i<MESSAGES_NB; i++)
    {
        if( msgrcv(file_id, &reponse, sizeof(message_t), MSG_TYPE_RECEPTEUR, 0) == -1)
        {
            perror("Erreur durant la reception du message");
            exit(-3);
        }
    }

    /* Recuperation de l'heure actuelle avec precision de l’ordre de la microseconde */
    gettimeofday(&temps, NULL);
    temps_fin = temps.tv_sec+(temps.tv_usec/1000000.0);

    fprintf(stderr, "\nTemps de reception           =  %.6lf secondes", temps_fin - temps_debut);
    fprintf(stderr, "\nTemps de reception / message =  %.6lf secondes\n", (temps_fin - temps_debut) / MESSAGES_NB);
    fprintf(stderr, "\n----------- Fin reception %s -----------\n\n" , nomprog ) ;

    fprintf(stderr, "=== Affichage message : =======================\n");
    msg_afficher( &reponse );
    fprintf(stderr, "===============================================\n\n");

    exit(0);
}
