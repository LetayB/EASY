#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include <msg_messages.h>

int 
main( int nb_arg , char * tab_arg[] )
{     
    char nomprog[128] ;
    char nomprog[128] ;
    int id_file , i = 0 ;
    double start , end ;
    struct timeval temps_cur ;
    message_t message;

     /*-----*/

     //Verification du nombre d'arguments
    if( nb_arg !=1 ){
  	    fprintf( stderr , "%s - Emetteur dans la communication par paquet\n\n" , tab_arg[0] );
  	    fprintf( stderr , "usage : %s \n" , tab_arg[0] );
  	    exit(-1);
    }
     strcpy( nomprog , tab_arg[0] );

    /* Recherche de la file de messages */
    if( (file_id = msgget ( CLE_BAL , 0 ) ) == -1 ){
        fprintf( stderr ,"Erreur => Impossible de trouver la file de message");
        exit(-1);
    }

    /*    EMISSION     */

    /* Creation du message a envoyer */
    message.type = MSG_TYPE;

    //Recuperation du temps exacte à la microseconde près au début de l'envoi
    gettimeofday(&temps_cur, NULL);
    //On place ce temps dans un double plus facile à manipuler que les struct timevals
    start = temps_cur.tv_sec+(temps_cur.tv_usec/1000000.0);

    //Remplissage du message avec le caractère ASCCI 0x24 = $
    msg_remplir(msg,0x24);

    //Boucle écrivant MESSAGE_NB fois dans la file de messages
    for(i=0; i<MESSAGES_NB; i++){
        if( msgsnd( file_id, &message, sizeof(corps_t), 0) == -1){
            fprintf( stderr ,"Erreur durant l'envoi du message");
            exit(-1);
        }
    }

    //Recuperation du temps exacte à la microseconde près au début de l'envoi
    gettimeofday(&temps_cur, NULL);
    //On place ce temps dans un double plus facile à manipuler que les struct timevals
    end = temps_cur.tv_sec+(temps_cur.tv_usec/1000000.0);

    printf("\nTemps d'emission  = %.6lf secondes\n", end - start);
    printf("Temps d'emission par message = %.6lf secondes\n", (end - start) / MESSAGES_NB);

    printf("--------   Fin des Transmissions   --------");


    exit(0);
}
