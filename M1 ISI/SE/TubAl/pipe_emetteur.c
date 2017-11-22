#include <stdio.h>
#include <stdlib.h>	/* exit */
#include <sys/stat.h>	/* mknod */
#include <sys/types.h>	/* open */
#include <fcntl.h>
#include <unistd.h>	/* close , write */
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

     /*----------*/
     
     //Verification des arguments
     if( nb_arg != 1 ){
	    fprintf( stderr , "%s - Emetteur dans la communication par flot\n\n" , tab_arg[0] );
	     fprintf( stderr , "usage : %s \n" , tab_arg[0] );
	    exit(-1);
     }
     strcpy( nomprog , tab_arg[0] );

     //Creation du tube nommé , TUBE_NOM définit dans pipe_message.h
     if(mknod(TUBE_NOM,S_IFIFO | 0666,0)){
          fprintf( stderr ,"Erreur => Impossible de creer le tube nommé %s ***\n",TUBE_NOM);
          exit(-1);
     }
     printf("Creation du tube reussie\n");

     // Ouverture du tube en ecriture seule , nous ne lisons pas dedans
     if((fd_pipe = open(TUBE_NOM,O_WRONLY,0))==-1) {
          fprintf( stderr ,"Erreur => Probleme durant l'ouverture en lecture seul du tube nommé %s ***\n",TUBE_NOM);
          exit(-1);
     } 

     /*    EMISSION     */
     
     //Recuperation du temps exacte à la microseconde près au début de l'envoi
     gettimeofday(&temps_cur, NULL);
     //On place ce temps dans un double plus facile à manipuler que les struct timevals
     start = temps_cur.tv_sec+(temps_cur.tv_usec/1000000.0);

     //Remplissage du message avec le caractère ASCCI 0x24 = $
     pipe_remplir(msg,0x24);

      //Boucle écrivant MESSAGE_NB fois dans le tube nommé
     for(i=0;i<MESSAGES_NB;i++){
        write(fd_pipe,&msg,MESSAGES_TAILLE);
     }

     //Recuperation du temps exacte à la microseconde près au début de l'envoi
     gettimeofday(&temps_cur, NULL);
     //On place ce temps dans un double plus facile à manipuler que les struct timevals
     end = temps_cur.tv_sec+(temps_cur.tv_usec/1000000.0);

     printf("\nTemps d'emission  = %.6lf secondes\n", end - start);
     printf("Temps d'emission par message = %.6lf secondes\n", (end - start) / MESSAGES_NB);

     printf("--------   Fin des Transmissions   --------");

     //Fermeture du tube nommé
     close(fd_pipe);

     exit(0);
}
 
