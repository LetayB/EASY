#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


#include <sens.h>
#include <train.h>
#include <trajet.h>
#include <moniteur_voie_unique.h>


#define TAILLE_MARQUES 26
#define ligne_attente() sleep(1)

/* Variables partagées */

trajet_t trajet ;

/* 
 *  Avancement d'un train sur la ligne dans l'un des 2 sens
 *  Fonction destinee a etre executee par un thread
 */


static void ligne_train_deplacer( train_t * train ) 
{

     /*----------*/

     /* 
      * Depart du train 
      */
     if( train->sens == OUEST_EST )
     {
	  
	  /* 
	   * Depart zone OUEST
	   */
	  trajet_inserer( &trajet, (*train), OUEST, train->sens );
	  trajet_afficher( &trajet) ;	  

	  /*
	   * Trajet zone Ouest 
	   */
	  ligne_attente();
	  
	  /* 
	   * Trajet sens unique 
	   */

	  voie_unique_entree_ouest(&trajet) ;

	  trajet_extraire( &trajet, (*train), OUEST , train->sens );	
	  trajet_inserer(  &trajet, (*train), UNIQUE, train->sens );

	  trajet_afficher( &trajet) ;
	  ligne_attente(); 

	  trajet_extraire( &trajet, (*train), UNIQUE, train->sens );	
	  trajet_inserer(  &trajet, (*train), EST   , train->sens );

	  voie_unique_sortie_est(&trajet) ;

	  /*
	   * Trajet zone EST 
	   */
	  trajet_afficher( &trajet) ;
	  ligne_attente();
	  
	  /* 
	   * Arrivee zone EST
	   */
	  trajet_extraire( &trajet, (*train), EST, train->sens );	
	  trajet_afficher( &trajet) ;
 
     }
     else
     {		/* Sens == EST_OUEST */
	  
	  /* 
	   * Depart 
	   */
	  trajet_inserer( &trajet, (*train), EST, train->sens );
	  trajet_afficher( &trajet) ;

	  /*
	   * Trajet zone Est 
	   */

	  ligne_attente();
	  
	  /* 
	   * Trajet sens unique 
	   */
	  voie_unique_entree_est(&trajet) ;

	  trajet_extraire( &trajet, (*train), EST   , train->sens );	
	  trajet_inserer(  &trajet, (*train), UNIQUE, train->sens );

	  trajet_afficher( &trajet) ;
	  ligne_attente();

	  trajet_extraire( &trajet, (*train), UNIQUE, train->sens );	
	  trajet_inserer(  &trajet, (*train), OUEST , train->sens );

	  voie_unique_sortie_ouest(&trajet) ;

	  /*
	   * Trajet zone Ouest 
	   */
	  
	  trajet_afficher( &trajet) ;
	  ligne_attente();

	  /* 
	   * Arrivee
	   */
	  trajet_extraire( &trajet, (*train), OUEST, train->sens );	
	  trajet_afficher( &trajet) ;
     }

     pthread_exit(NULL);
}

/* 
 * Thread principale
 */


int
main(  int nb_arg , char * tab_arg[] )
{
  pthread_t tid_train[MAX_TRAINS] ; /* identite des threads train */
  train_t t[MAX_TRAINS] ;
  int nb_trains = 0 ;
  int max_trains = 0 ;
  int i ;

  static char tab_marques[TAILLE_MARQUES] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'} ;

  /*----------*/

  if( nb_arg != 3 )
  {
       printf( "usage : %s <nb trains> <max train sens sens unique>\n" , tab_arg[0] );
       exit(-1);
  }

  sscanf( tab_arg[1] , "%d" , &nb_trains );
  if( nb_trains > MAX_TRAINS )
  {
       fprintf( stderr, "Erreur %s  : le nombre de trains %d depasse la limite autorisee (%d)\n",
		tab_arg[0] , nb_trains , MAX_TRAINS );
       return(-1);
  }
  sscanf( tab_arg[2] , "%d" , &max_trains );
  if( max_trains > MAX_TRAINS )
  {
       fprintf( stderr, "Erreur %s  : le maximum de trains %d depasse la limite autorisee (%d)\n",
		tab_arg[0] , max_trains , MAX_TRAINS );
       return(-1);
  }
  srandom(getpid());

#ifdef DEBUG
     printf("\n ----- Debut %s : Nb trains %d , Max trains %d -----\n" , 
	    tab_arg[0],
	    nb_trains , 
	    max_trains );
#endif

  /* Initialisation du moniteur */
  voie_unique_initialiser( &trajet , max_trains ) ;

  /* mettre tous les trains en concurrence */
  pthread_setconcurrency(nb_trains);

  /* Creation des threads train */
  for( i=0 ; i<nb_trains ; i++)
  {
       if( random()%2 )
       {
	    train_sens_set( &t[i]  , OUEST_EST );
       }
       else
       {
	    train_sens_set( &t[i]  , EST_OUEST );
       }       
       train_marque_set( &t[i]  , tab_marques[i%TAILLE_MARQUES] ) ;
#ifdef DEBUG
       printf("Lancement du train ");
       train_print(&t[i]);
       printf("\n");
#endif

       pthread_create(&tid_train[i], NULL, (void *)ligne_train_deplacer, (void *)&t[i]);
  }

  /* Attente fin des threads */
  for(i=0 ; i<nb_trains ; i++ )
       pthread_join( tid_train[i] , NULL );

  /* fermeture du moniteur */
  voie_unique_fermer(&trajet);

#ifdef DEBUG
     printf("\n ----- Fin %s  -----\n" , 
	    tab_arg[0] );
#endif

  exit(0);
}
