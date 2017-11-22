#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef _LINUX_
#include <string.h>
#else
#include <strings.h>
#endif

#include <coord.h>

int
main( int nb_arg , char * tab_arg[] )
{
     
  coord_t c , c2 , c3 ;
     int ligne , colonne  ;
     off_t position ;
     int cmp , diff1 , diff2 , diff3 ;

     /*----------*/

     if( nb_arg != 4 )
     {
	  fprintf( stderr , "\n%s: Test du TDA coord\n\n" , tab_arg[0] );
	  fprintf( stderr , "Usage : %s <ligne> <colonne> <position>\n",
		   tab_arg[0] );
	  exit(-1) ;
     }

     sscanf( tab_arg[1] , "%d" , &ligne ) ;
     sscanf( tab_arg[2] , "%d" , &colonne ) ;
     sscanf( tab_arg[3] , "%ld" , &position ) ;

     /* Initialisation generation nombres pseudo-aleatoires */
     srandom((unsigned int)getpid());

     /* Test ecriture dans une case */
     coord_ecrire( &c,
		   ligne,		
		   colonne,		
		   position );

     coord_ligne_ecrire( &c3 , ligne );
     coord_colonne_ecrire( &c3 , colonne );
     coord_position_ecrire( &c3 , position );


     /* Test affichage d'une case */
     printf("\nAffichage structure case affectee globalement: ");
     coord_afficher( stdout, c ); 
     printf("\n\n");


     printf("\nAffichage structure case affectee attribut par attribut: ");
     coord_afficher( stdout, c3 ); 
     printf("\n\n");

     /* Test lecture d'une case */
     coord_lire( c,		
		 &ligne,	              
		 &colonne,	            
		 &position);	

     printf("\nAffichage des caracteristiques de la case affectee globalement:\n");
     printf("\tligne    = %d\n"  , ligne );
     printf("\tcolonne  = %d\n"  , colonne );
     printf("\tposition = %ld\n" , position );
     printf("\n\n");

     ligne = coord_ligne_lire(c3);
     colonne = coord_colonne_lire(c3);
     position = coord_position_lire(c3);

     printf("\nAffichage des caracteristiques de la case affectee attribut par attribut:\n");
     printf("\tligne    = %d\n"  , ligne );
     printf("\tcolonne  = %d\n"  , colonne );
     printf("\tposition = %ld\n" , position );
     printf("\n\n");

     /* Test comparaison de 2 coordonnees */
     diff1 = (int)(random()%3) ; diff1 = diff1 - 1 ; 
     diff2 = (int)(random()%3) ; diff2 = diff2 - 1 ; 
     diff3 = (int)(random()%3) ; diff3 = diff3 - 1 ; 

     coord_ecrire( &c2 ,
		   ligne+diff1 ,
		   colonne+diff2,
		   position+diff3 ) ;
     
     cmp = coord_comparer( c , c2 );
     coord_afficher( stdout, c ); 
     if( cmp < 0 )
     {
	  printf( " est inferieure a ") ;
     }
     else
     {
	  if( cmp > 0 )
	       printf( " est superieure a ");
	  else
	       printf( " est egale a ");
     }
     coord_afficher( stdout, c2 ); 
     printf( "\n\n");

     /* Test affectation */
     coord_affecter( &c2 , c );
     cmp = coord_comparer( c , c2 );
     coord_afficher( stdout, c ); 
     if( cmp < 0 )
     {
	  printf( " est inferieure a ") ;
     }
     else
     {
	  if( cmp > 0 )
	       printf( " est superieure a ");
	  else
	       printf( " est egale a ");
     }
     coord_afficher( stdout, c2 ); 
     printf( "\n\n");

     exit(0); 
}
