#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include <string.h>


#include <msg_messages.h>

int 
main( int nb_arg , char * tab_arg[] )
{     
    char nomprog[128] ;
    int id_file , i = 0 ;
    double start , end ;
    struct timeval temps_cur ;
    message_t reponse;

    /*-----*/

    //Verification du nombre d'arguments
    if( nb_arg !=1 ){
	   fprintf( stderr , "%s - Recepteur dans la communication par paquet\n\n" , tab_arg[0] );
       fprintf( stderr , "usage : %s \n" , tab_arg[0] );
	   exit(-1);
    }
    strcpy( nomprog , tab_arg[0] );

    //Creation de la file de message
    if( ( file_id = msgget(CLE_BAL ,IPC_CREAT | IPC_EXCL | 0666 ))== -1){
     	fprintf( stderr ,"Erreur => Impossible de creer la file de messages");
        exit(-1);
    }

    /*    RECEPTION     */
     
    //Recuperation du temps exacte à la microseconde près au début de l'envoi
    gettimeofday(&temps_cur, NULL);
    //On place ce temps dans un double plus facile à manipuler que les struct timevals
    start = temps_cur.tv_sec+(temps_cur.tv_usec/1000000.0);

    //Boucle lisant MESSAGE_NB fois dans la file de message
    for(i=0; i<MESSAGES_NB; i++){
        if( msgrcv(file_id, &reponse, sizeof(message_t), MSG_TYPE, 0) == -1){
            fprintf( stderr ,"Erreur durant la reception du message");
            exit(-1);
        }
    }

    //Recuperation du temps exacte à la microseconde près au début de l'envoi
    gettimeofday(&temps_cur, NULL);
    //On place ce temps dans un double plus facile à manipuler que les struct timevals
    end = temps_cur.tv_sec+(temps_cur.tv_usec/1000000.0);

    printf("\nTemps de reception  = %f secondes\n", end - start);
    printf("Temps de reception par message = %f secondes\n", (end - start) / MESSAGES_NB);

    printf("---------   Message   ---------\n");
    pipe_afficher(msg);
    printf("\n");

    //Destruction de la file de message
    msgctl(CLE_BAL, IPC_RMID,0);

    exit(0);
}
