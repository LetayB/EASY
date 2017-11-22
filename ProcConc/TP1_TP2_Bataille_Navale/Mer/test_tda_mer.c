#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef _LINUX_
#include <string.h>
#else
#include <strings.h>
#endif

#include <commun.h>
#include <mer.h>


int
main( int nb_arg , char * tab_arg[] )
{
     
     char nomprog[128] ;

     int fd_mer ;
     char fich_mer[128] ;

     int no_err = CORRECT ;

     coord_t * liste_voisins = (coord_t *)NULL ;
     int nb_voisins = 0 ;

     bateau_t bateau ;
     booleen_t ok = VRAI ;

     int b ;
     
     int nb_lig = 0 ;
     int nb_col = 0 ;

     coord_t cible ;
     booleen_t acquisition ;

     /*----------*/

     if( nb_arg != 4 )
     {
	  fprintf( stderr , "Usage : %s <nom fichier mer> <nb lignes> <nb colonnes>\n",
		   tab_arg[0] );
	  exit(-1) ;
     }

     strcpy( nomprog , tab_arg[0] );
     strcpy( fich_mer , tab_arg[1] ) ;
     sscanf( tab_arg[2] , "%d" , &nb_lig );
     sscanf( tab_arg[3] , "%d" , &nb_col ) ;

     /*
      * Creation mer 
      */

     if( (no_err = mer_initialiser( fich_mer,nb_lig,nb_col)) )
     {
	  fprintf( stderr, "%s : erreur %d dans mer_initialiser\n",
		   nomprog , no_err );
	  exit(no_err) ;
     }

     /* 
      * Ouverture fichier mer
      */

     if( ( fd_mer = open( fich_mer , O_RDWR , 0644 )) == -1 )
     {
	  fprintf( stderr, "%s : Pb open sur %s\n", nomprog , fich_mer);
	  exit(-1);
     }

     printf(" Mer initialisee: \n");

     if( (no_err = mer_afficher(fd_mer) )) 
     {
	  fprintf( stderr, "%s : erreur %d dans mer_afficher\n",
		   nomprog , no_err );
	  exit(no_err) ;
     }
     
     
     srandom(getpid()) ;

     for( b=0 ; b < BATEAU_NB_MAX ; b++ ) 
     {
	 
	  /* 
	   * Test initialisation des bateaux 
	   */

	  bateau_marque_ecrire( &bateau , 'A'+b );
	  bateau_pid_ecrire( &bateau , 100+b );
	  bateau_actif_ecrire( &bateau , VRAI );
	  if( (no_err = mer_bateau_initialiser( fd_mer , 
						&bateau)) )
	  {
	       fprintf( stderr, "%s : erreur %d dans 3mer_bateau_initialiser\n",
			nomprog , no_err );
	       if( no_err == ERREUR )
	       {
		 fprintf( stderr, "\t(mer_bateau_initialiser n'a pas pu placer le bateau \"%c\" dans la mer)\n" ,
			  bateau_marque_lire(bateau) );
	       }
	       exit(no_err) ;
	  }
     
	  if( (no_err = mer_afficher(fd_mer) )) 
	  {
	       fprintf( stderr, "%s : erreur %d dans mer_afficher\n",
			nomprog , no_err );
	       exit(no_err) ;
	  }

	  /* 
	   * Test deplacement des bateaux 
	   */
	  coord_liste_creer(&liste_voisins);

	  if( (no_err = mer_voisins_rechercher( fd_mer,
						bateau,
						&liste_voisins,
						&nb_voisins)) )
	  {
	       fprintf( stderr, "%s : erreur %d dans mer_voisins_rechercher\n",
			nomprog , no_err );
	       exit(no_err) ;
	  }

	  printf( "Liste des voisins :\n");
	  coord_liste_afficher( stdout , liste_voisins , nb_voisins );
	  printf("\n\n");

	  if( (no_err = mer_bateau_deplacer( fd_mer,
					     &bateau,
					     liste_voisins,
					     nb_voisins,
					     &ok )) )
	  {
	       fprintf( stderr, "%s : erreur %d dans mer_bateau_deplacer\n",
			nomprog , no_err );
	       exit(no_err) ;
	  }

	  coord_liste_detruire(&liste_voisins); 
	
	  if( ok )
	  {
	       printf( "Deplacement reussi\n");
	  }
	  else
	  {
	       printf(" Deplacement impossible\n");
	  }
	  

	  if( (no_err = mer_afficher(fd_mer) )) 
	  {
	       fprintf( stderr, "%s : erreur %d dans mer_afficher\n",
			nomprog , no_err );
	       exit(no_err) ;
	  }

	  if( b > 1 )
	  {
	       /* 
		* Test de tir 
		*/

	       if( (no_err = mer_bateau_cible_acquerir( fd_mer,
							bateau,
							&acquisition,
							&cible )) )
	       {
		    fprintf( stderr, "%s : erreur %d dans mer_bateau_cible_acquerir\n",
			     nomprog , no_err );
		    exit(no_err) ;
	       }

	       if( acquisition ) 
	       {
		    printf("Acquisition d'une cible par le bateau \n");
		    bateau_afficher( stdout , bateau );
		    printf( "\n-->La cible choisie est la case ");
		    coord_afficher( stdout , cible );
		    printf( "\n\n");
	       }
	       else
	       {
		    printf("Pas d'acquisition de cible pour le bateau \n");
		    bateau_afficher( stdout , bateau );
		    printf( "\n");
	       }

	       if( (no_err = mer_bateau_cible_tirer( fd_mer,
						     cible)) )
	       {
		    fprintf( stderr, "%s : erreur %d dans mer_bateau_cible_tirer\n",
			     nomprog , no_err );
		    exit(no_err) ;
	       }
	       
	       if( (no_err = mer_afficher(fd_mer) )) 
	       {
		    fprintf( stderr, "%s : erreur %d dans mer_afficher\n",
			     nomprog , no_err );
		    exit(no_err) ;
	       }
	  }
	  
     }

     close(fd_mer); 

     exit(0);

}
