#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#ifdef _LINUX_
#include <string.h>
#else
#include <strings.h>
#endif

#include <bateau.h>

int
main( int nb_arg , char * tab_arg[] )
{

     char nomprog[128] ;
     char fich_bateaux[128] ;
     int noerr = CORRECT ;

     /*--------------------*/

     if( nb_arg != 2)
     {
	  fprintf(stderr, "Usage %s <fichier bateaux>\n",
		  tab_arg[0] );
	  exit(-1);
     }


     strcpy( nomprog      , tab_arg[0] );
     strcpy( fich_bateaux    , tab_arg[1] );

     if( (noerr = bateau_fichier_initialiser( fich_bateaux ) ) )
     {
	  fprintf( stderr , "%s : erreur %d lors de l'initialisation du fichier bateau <%s>\n",
		   nomprog , noerr , fich_bateaux );
	  exit(noerr);
     }
     
     exit(0);

}
