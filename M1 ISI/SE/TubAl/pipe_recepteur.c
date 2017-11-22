#include <stdio.h>
#include <unistd.h>	
#include <stdlib.h>	
#include <sys/types.h>	
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#ifdef _LINUX_
#include <string.h>
#else
#include <strings.h>
#endif
#include <pipe_messages.h>

int
main( int nb_arg , char * tab_arg[])
{
     char nomprog[128] ;
     int fd_pipe , i = 0;
     double start , end ;
     struct timeval temps_cur ;
     char msg[MESSAGES_TAILLE ];

     /*-----*/

     //Verification des arguments
     if( nb_arg != 1 ){
	  fprintf( stderr , "%s - Recepteur dans la communication par flot\n\n" , tab_arg[0] );
	  fprintf( stderr , "usage : %s \n" , tab_arg[0] );
	  exit(-1);
     }
     strcpy( nomprog , tab_arg[0] );


     // Ouverture du tube en lecture seule , nous ne lisons pas dedans
     if((fd_pipe = open(TUBE_NOM,O_RDONLY,0))==-1) {
        fprintf( stderr ,"Erreur => Probleme durant l'ouverture en lecture seul du tube nommé %s ***\n",TUBE_NOM);
        exit(-1);
     } 

     /*    RECEPTION     */
     
     //Recuperation du temps exacte à la microseconde près au début de l'envoi
     gettimeofday(&temps_cur, NULL);
     //On place ce temps dans un double plus facile à manipuler que les struct timevals
     start = temps_cur.tv_sec+(temps_cur.tv_usec/1000000.0);

     //Remplissage du message avec le caractère ASCCI 0x24 = $
     pipe_remplir(msg,0x24);

      //Boucle lisant MESSAGE_NB fois dans le tube nommé
     for(i=0;i<MESSAGES_NB;i++){
        read(fd_pipe,&msg,MESSAGES_TAILLE);
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

     //Fermeture du tube nommé
     close(fd_pipe);
     //Suppresion du tube nommé
     remove(TUBE_NOM);

     exit( 0 );
}
