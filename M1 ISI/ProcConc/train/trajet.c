#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <sens.h>
#include <train.h>
#include <trajet.h>

#define max(a,b) (a>b ? a : b )

extern 
void trajet_zone_print( zone_t z )
{
  switch( z )
  {
     case  OUEST : 
       printf( "OUEST") ;
       break ;
     case UNIQUE : 
       printf( "UNIQUE");
       break ;
     case EST : 
       printf( "EST");
       break ;
     default :
       printf("zone inconnu");
       break ;
  }
}

extern 
void trajet_zone_id_print( zone_id_t z )
{
  switch( z )
  {
     case ZONE_ID_OUEST_OE: 
       printf( "zone OUEST sens OUEST-->EST") ;
       break ;
     case ZONE_ID_OUEST_EO: 
       printf( "zone OUEST  sens EST-->OUEST") ;
       break ;
     case ZONE_ID_UNIQUE :  
       printf( "zone SENS UNIQUE") ;
       break ;
     case ZONE_ID_EST_OE: 
       printf( "zone EST sens OUEST-->EST") ;
       break ;
     case ZONE_ID_EST_EO: 
       printf( "zone EST sens EST-->OUEST") ;
       break ;
     default :
       printf("identificateur de zone inconnu");
       break ;
  }
}


/* TDA MONITEUR trajet = sauvegardes des trains sur les 3 parties du trajet */

/* ----- Variables ----- */

/* Procedures internes */

static
void trajet_voie_vide_print()
{
     printf("..........");
}

static
void trajet_voie_blanc_print()
{
     printf("          ");
}

/* ----- Procedures ----- */

extern int
trajet_initialiser( trajet_t * tj )
{
     int z , t ;

     /*----------*/

     /* Initialisation du mutex */
     pthread_mutex_init(&(tj->mutex), NULL);

     /* Initialisation des variables */
     for( z=0 ; z<5 ; z++)
     {
	  tj->cpt[z] = 0 ;
	  for(t=0; t<MAX_TRAINS ; t++)
	       tj->zone[z][t] = TRAIN_VIDE ;
     }

     return(0);
}

extern int
trajet_fermer( trajet_t * t )
{
     /* Initialisation du mutex */
     pthread_mutex_destroy(&(t->mutex));

     return(0);
}

extern void trajet_mapper( trajet_t * const trajet )
{
     int z , t ;

     /* Entree dans le moniteur */
     pthread_mutex_lock(&(trajet->mutex));

     printf("\n\t--------------------\n");

     /* Affichage des 5 zones */
     for( z=0 ; z<5 ; z++ )
     {
	  switch( z )
	  {
	     case 0 :
		  printf( "zone OUEST  sens OUEST_EST\n");
		  break ;
	     case 1 :
		  printf( "zone OUEST  sens EST_OUEST\n");
		  break ;
	     case 2 :
		  printf( "zone UNIQUE\n");
		  break ;
	     case 3 :
		  printf( "zone EST  sens OUEST_EST\n");
		  break ;
	     case 4 :
		  printf( "zone EST  sens EST_OUEST\n");
		  break ;
	  }
	  printf( "Nb trains = %d \n" , trajet->cpt[z] );
	  for(t=0 ; t<MAX_TRAINS ; t++ )
	  {
	       printf(" train[%d] = " , t );
	       train_print( &(trajet->zone[z][t]) );
	       printf("\n");
	  }
     }

     printf("\n\t--------------------\n\n");

     /* Sortie du moniteur */
     pthread_mutex_unlock(&(trajet->mutex));
}

extern void trajet_afficher( trajet_t * const trajet )
{
     int i ;
     int max_cpt ;

     /*--------------------*/
     
     /* Entree dans le moniteur */
     pthread_mutex_lock(&(trajet->mutex));

     printf("\n\t--------------------\n");

     /* 
      * 1ere ligne : Affichage OUEST VERS EST 
      *---------------------------------------
      */
     
     if( trajet->cpt[ZONE_ID_OUEST_OE] + trajet->cpt[ZONE_ID_OUEST_EO]  == 0 )
     {
	  /* Affichage voie minimale si pas de train */
	  trajet_voie_vide_print();
	  
     }
     else
     {
	  /* Affichage zone ouest sens A vers B */
	  for(i=0 ; i<trajet->cpt[ZONE_ID_OUEST_OE] ; i++ )
	  {
	       train_print( &(trajet->zone[ZONE_ID_OUEST_OE][i]) );
	  }
	  for(i=trajet->cpt[ZONE_ID_OUEST_OE] ; i<trajet->cpt[ZONE_ID_OUEST_EO] ; i++ )
	  {	  
	       train_vide_print();
	  }
     }

     /* Affichage blanc du sens unique */
     if( trajet->cpt[ZONE_ID_UNIQUE] == 0 )
     {
	  trajet_voie_blanc_print();
     }
     else
     {
	  for(i=0 ; i<trajet->cpt[ZONE_ID_UNIQUE] ; i++ )
	  {
	       train_blanc_print();
	  }
     }

     /* Affichage zone est sens A vers B */
     if( trajet->cpt[ZONE_ID_EST_OE] + trajet->cpt[ZONE_ID_EST_EO] == 0 )
     {
	  trajet_voie_vide_print();
     }
     else
     {
	  for(i=0 ; i<trajet->cpt[ZONE_ID_EST_OE] ; i++ )
	  {
	       train_print( &(trajet->zone[ZONE_ID_EST_OE][i]) );
	  }
	  for(i=trajet->cpt[ZONE_ID_EST_OE] ; i<trajet->cpt[ZONE_ID_EST_EO] ; i++ )
	  {	  
	       train_vide_print();
	  }     
     }
     printf("\n");

     /* 
      * 2ieme ligne : Affichage SENS UNIQUE
      *--------------------------------------
      */
     
     /* Affichage blanc zone ouest */
     if( trajet->cpt[ZONE_ID_OUEST_OE]+trajet->cpt[ZONE_ID_OUEST_EO] == 0 )
     {
       trajet_voie_blanc_print();
     }
     else
     {
       max_cpt = max( trajet->cpt[ZONE_ID_OUEST_OE] , trajet->cpt[ZONE_ID_OUEST_EO] );
       for(i=0 ; i< max_cpt ; i++ )
       {
	 train_blanc_print();	  
       }
     }

     /* Affichage du sens unique */
     if( trajet->cpt[ZONE_ID_UNIQUE] == 0 )
     {
       trajet_voie_vide_print();
     }
     else
     {
	  for(i=0 ; i<trajet->cpt[ZONE_ID_UNIQUE] ; i++ )
	  {
	       train_print( &(trajet->zone[ZONE_ID_UNIQUE][i]));
	  }
     }

     /* Affichage blanc zone est */
     if( trajet->cpt[ZONE_ID_EST_OE]+trajet->cpt[ZONE_ID_EST_EO] == 0 )
     {
	  trajet_voie_blanc_print();
     }
     else
     {
       max_cpt = max( trajet->cpt[ZONE_ID_EST_OE] , trajet->cpt[ZONE_ID_EST_EO] );

       for(i=0 ; i< max_cpt ; i++ )
       {
	 train_blanc_print();
       }
     }
     printf("\n");

     /* 
      * 3ieme ligne : Affichage EST VERS OUEST
      *-----------------------------------------
      */
     
     /* Affichage zone ouest sens B vers A */
     if( trajet->cpt[ZONE_ID_OUEST_EO]+trajet->cpt[ZONE_ID_OUEST_OE] == 0 )
     {
	  trajet_voie_vide_print();
     }
     else
     {
	  for(i=0 ; i<trajet->cpt[ZONE_ID_OUEST_EO] ; i++ )
	  {
	       train_print( &(trajet->zone[ZONE_ID_OUEST_EO][i]) );
	  }
	  for(i=trajet->cpt[ZONE_ID_OUEST_EO] ; i<trajet->cpt[ZONE_ID_OUEST_OE] ; i++ )
	  {	  
	       train_vide_print();
	  }
     }

     /* Affichage blanc du sens unique */
     if( trajet->cpt[ZONE_ID_UNIQUE] == 0 )
     {
	  trajet_voie_blanc_print();
     }
     else
     {
	  for(i=0 ; i<trajet->cpt[ZONE_ID_UNIQUE] ; i++ )
	  {
	       train_blanc_print();
	  }
     }

     /* Affichage zone est sens B vers A */
     if( trajet->cpt[ZONE_ID_EST_EO]+trajet->cpt[ZONE_ID_EST_OE] == 0 )
     {
	  trajet_voie_vide_print();
     }
     else
     {
	  for(i=0 ; i<trajet->cpt[ZONE_ID_EST_EO] ; i++ )
	  {
	       train_print( &(trajet->zone[ZONE_ID_EST_EO][i]) );
	  }
	  for(i=trajet->cpt[ZONE_ID_EST_EO] ; i<trajet->cpt[ZONE_ID_EST_OE] ; i++ )
	  {	  
	       train_vide_print();
	  }
     }
     printf("\n\t--------------------\n\n");

     /*
      * Verif collisions 
      */
     if( trajet->cpt[ZONE_ID_UNIQUE] > 1 )
     {
	  sens_t sens_prem = train_sens_get(&(trajet->zone[ZONE_ID_UNIQUE][0])) ;

	  for(i=1 ; i<trajet->cpt[ZONE_ID_UNIQUE] ; i++ )
	  {
	       if( sens_prem != train_sens_get(&(trajet->zone[ZONE_ID_UNIQUE][i])) )
	       {
		    printf("\n\t--- !!! COLLISION !!! ---\n\n");
		    /* sortie du processus donc arret de toutes les threads */
		    exit(-1);
	       }
	  }
     }

#ifdef DEBUG
     printf("\n\t--- OK ---\n\n");
#endif

     /* Sortie du moniteur */
     pthread_mutex_unlock(&(trajet->mutex));
}


static int
trajet_train_rechercher( trajet_t * const tj,	/* trajet */
			 train_t tn ,	/* train a rechercher */
			 const zone_id_t z,     /* zone de recherche */
			 train_id_t * train_id ) /* indice resultat */
{
  int i = 0 ;
  boolean_t trouve = B_FALSE ;
     
  /*----------*/

  (*train_id) = -1 ;

  while( (i<tj->cpt[z]) && (!trouve) )
  {
    if( ! train_compare( &tn , &(tj->zone[z][i]) ) )
      trouve = B_TRUE ;
    else
      i++ ;
  }

  if( trouve )
  {
    (*train_id) = i ;
     return(0);
  }
  else
    return(-1);
}

static int 
trajet_zone_decaler( trajet_t * const tj,        /* trajet */
		     const zone_id_t z,          /* zone a decaler */
		     const train_id_t id_debut ) /* indice debut decalage */
{
  int i ;
  /*----------*/

  for(i=id_debut; i<tj->cpt[z]-1 ; i++ )
    tj->zone[z][i] = tj->zone[z][i+1] ;
  return(0);
}

extern int
trajet_inserer( trajet_t * tj ,     /* trajet */      
		const train_t tn ,  /* train a inserer */
		const zone_t z,	    /* zone */        
		const sens_t s )          /* sens */        
{
  sens_t s_l = s ;

  /*----------*/

  /* Entree dans le moniteur */
  pthread_mutex_lock(&(tj->mutex));
  
  if( z == UNIQUE ) s_l = OUEST_EST ;
 
  tj->zone[z+s_l][tj->cpt[z+s_l]] = tn ;
  tj->cpt[z+s_l]++ ;
     
  /* Sortie du moniteur */
  pthread_mutex_unlock(&(tj->mutex));
  return(0) ;
}

extern int
trajet_extraire( trajet_t * tj ,      /* trajet */           
		 const train_t tn ,   /* train a extraire */   
		 const zone_t z,      /* zone */             
		 const sens_t s )           /* sens */             
{
  train_id_t train_id ;
  int cr ;
  train_t tn_l = tn ;
  sens_t s_l = s ;

  /*---------------------*/

  /* Entree dans le moniteur */
  pthread_mutex_lock(&(tj->mutex));

  if( z == UNIQUE ) s_l = OUEST_EST ;

  if( (cr = trajet_train_rechercher( tj , tn_l , z+s_l, &train_id )) )
  {
	  printf( "Erreur trajet_extraire: le train  ");
	  train_print( &tn_l ) ;
	  printf( " inconnu dans la zone ");
	  trajet_zone_print( z );
	  printf( " dans le sens ");
	  sens_print( s_l );
	  printf( "\n" );

	  pthread_mutex_unlock(&(tj->mutex));
	  return(cr);
  }

  trajet_zone_decaler( tj, z+s_l, train_id) ;
  tj->cpt[z+s_l]-- ;

  /* Sortie du moniteur */
  pthread_mutex_unlock(&(tj->mutex));
  return(0) ;
}
