#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef _LINUX_
#include <string.h>
#else
#include <strings.h>
#endif

#include <bateau.h>


int
main( int nb_arg , char * tab_arg[] )
{
     char nomprog[256] ;
     char fich_bateaux[128] ;

     bateau_t * bateaux = (bateau_t *)NULL ;
     int nb_bateaux = 0 ;

     int v ; 
     int sig ;

     /*----------*/

     /* 
      * Capture des parametres 
      */

     if( nb_arg != 3 )
     {
	  fprintf( stderr , "\n\n%s : envoie le signal numero <signal> a tous les processus bateaux contenus dans le fichier de reference <fichier des bateaux>\n\n",
		   tab_arg[0] ); 
	  fprintf( stderr , "Usage : %s <fichier des bateaux> <signal>\n", 
		   tab_arg[0] );
	  exit(-1);
     }

     strcpy( nomprog , tab_arg[0] );
     strcpy( fich_bateaux , tab_arg[1] );
     sscanf( tab_arg[2] , "%d" , &sig ) ;

     printf("\n\n\t----- Debut Trafalgar %s -----\n\n" , nomprog);

#ifdef _DEBUG_
     printf(" Envoie du signal numero %d aux bateaux suivants:\n" , sig );
     bateau_fichier_afficher( fich_bateaux );
#endif

     /* 
      *  Chargement de la liste des bateaux 
      */

     bateau_liste_charger( fich_bateaux, &bateaux , &nb_bateaux );
     
     /* 
      * Envoi du signal d'arret aux bateaux
      */

     for( v=0 ; v < nb_bateaux ; v++ )
     {
#ifdef _LINUX_	  
       printf("arret --> %d\n" , bateaux[v].pid);
#else
	  printf("arret --> %ld\n" , bateaux[v].pid);
#endif
	  kill(  bateaux[v].pid , SIGKILL );
     }
          

     printf("\n\n\t----- Fin Trafalgar %s -----\n\n", nomprog);

     exit(0);
}

