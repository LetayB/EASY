//
// msg_emetteur.c
//
// --> Programme creant une file de message et y postant
// MESSAGES_NB fois un message de taille MESSAGES_TAILLE
//
// Auteur : FROGER Adrien - L3SPI - Grp 2
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>

#include <msg_messages.h>

#define CLE_BAL 42

int
main( int nb_arg , char * tab_arg[] )
{
    char nomprog[128] ;
    int file_id = 0;

    int i;

    message_t message;

    struct timeval temps ;
    double temps_debut;
    double temps_fin;


    /*-----*/



    /* Verification du nombre d'arguments */
    if( nb_arg !=1 )
    {
        fprintf( stderr , "%s - Emetteur dans la communication par paquet\n\n" , tab_arg[0] );
        fprintf( stderr , "usage : %s \n" , tab_arg[0] );
        exit(-1);
    }
    strcpy( nomprog , tab_arg[0] );

    /* Creation de la file de messages */
    if( ( file_id = msgget( CLE_BAL, IPC_CREAT | IPC_EXCL | 0666 )) == -1 )
    {
        perror("Probleme durant la creation de la file de messages");
        exit(-2);
    }


    /* Creation du message a envoyer */
    message.type = MSG_TYPE_RECEPTEUR;


    fprintf(stderr,  "\n---------- Debut emission %s ----------\n" , nomprog ) ;

    /* Recuperation de l'heure actuelle avec precision de l’ordre de la microseconde */
    gettimeofday(&temps, NULL);
    temps_debut = temps.tv_sec+(temps.tv_usec/1000000.0);


    /* Remplissage du message avec des Z */
    msg_remplir( &message , 'Z') ;


    /* Envoi de MESSAGES_NB messages dans la file de messages  */
    for(i=0; i<MESSAGES_NB; i++)
    {
        if( msgsnd( file_id, &message, sizeof(corps_t), 0) == -1)
        {
            perror ("Erreur durant l'envoi du message");
            exit(-3);
        }
    }

    /* Recuperation de l'heure actuelle avec precision de l’ordre de la microseconde */
    gettimeofday(&temps, NULL);
    temps_fin = temps.tv_sec+(temps.tv_usec/1000000.0);

    fprintf(stderr, "\nTemps d'emission           =  %.6lf secondes", temps_fin - temps_debut);
    fprintf(stderr, "\nTemps d'emission / message =  %.6lf secondes\n", (temps_fin - temps_debut) / MESSAGES_NB);
    fprintf(stderr, "\n----------- Fin emission %s -----------\n\n" , nomprog ) ;

    exit(0); //Note: Une file de messages, meme privee, doit etre detruite explicitement. 
	     //Elle n'est pas supprimee par un appel de exit() par le processus createur. 
	     //Pour detruire la file : ipcrm -Q 42 (42 est le numero de la file) dans un terminal 
             //OU                    : msgctl(CLE_BAL, IPC_RMID,0) dans le code source
}

