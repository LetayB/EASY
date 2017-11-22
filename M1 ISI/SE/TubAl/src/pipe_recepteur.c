// 
// pipe_recepteur.c
//
// --> Programme lisant MESSAGES_NB messages de taille
// MESSAGES_TAILLE dans le tube nomme TUBE_NOMME
//
// Auteur : FROGER Adrien - L3SPI - Grp 2
//

#include <stdio.h>
#include <unistd.h>	/* close, read, write */
#include <stdlib.h>	/* exit */
#include <sys/types.h>	/*open */
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#ifdef _LINUX_
#include <string.h>
#else
#include <strings.h>
#endif
#include <pipe_messages.h>

#define TUBE_NOMME "tube_tp_5"

int main( int nb_arg , char * tab_arg[])
{
    char nomprog[128] ;
    char message[MESSAGES_TAILLE] ;

    int fd_tube;
    int i;

    struct timeval temps ;
    double temps_debut;
    double temps_fin;

    /*-----*/

    /* Verification du nombre d'arguments */
    if( nb_arg != 1 )
    {
        fprintf( stderr , "%s - Recepteur dans la communication par flot\n\n" , tab_arg[0] );
        fprintf( stderr , "usage : %s \n" , tab_arg[0] );
        exit(-1);
    }
    strcpy( nomprog , tab_arg[0] );

    /* Ouverture du tube nomme en lecture seule */
    if( (fd_tube = open( TUBE_NOMME, O_RDONLY, 0 )) == -1 )
    {
        perror("Probleme durant l'ouverture du tube nomme");
        exit(-1);
    }

    fprintf(stderr, "\n---------- Debut reception %s ----------\n" , nomprog ) ;

    /* Recuperation de l'heure actuelle avec precision de l’ordre de la microseconde */
    gettimeofday(&temps, NULL);
    temps_debut = temps.tv_sec+(temps.tv_usec/1000000.0);

    /* Lecture des MESSAGES_NB messages dans le tube nomme */	
    for(i=0; i<MESSAGES_NB; i++)
    {
        read( fd_tube, &message, MESSAGES_TAILLE);
    }

    /* Recuperation de l'heure actuelle avec precision de l’ordre de la microseconde */
    gettimeofday(&temps, NULL);
    temps_fin = temps.tv_sec+(temps.tv_usec/1000000.0);


    fprintf(stderr, "\nTemps de reception           =  %.6lf secondes", temps_fin - temps_debut);
    fprintf(stderr, "\nTemps de reception / message =  %.6lf secondes\n", (temps_fin - temps_debut) / MESSAGES_NB);
    fprintf(stderr, "\n----------- Fin reception %s -----------\n\n" , nomprog ) ;

    fprintf(stderr, "=== Affichage message : =======================\n");
    pipe_afficher( message );
    fprintf(stderr, "===============================================\n\n");

    /* Fermeture du tube nomme */
    close( fd_tube );

    /* Suppression du tube nomme */
    remove( TUBE_NOMME );

    exit( 0 );
}
