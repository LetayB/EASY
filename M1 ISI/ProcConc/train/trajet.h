#ifndef _TRAJET_H_
#define _TRAJET_H_

#include <pthread.h>
#include <sens.h>
#include <train.h>


/* 
 * Definitions des structures
 */

typedef enum zone_s { OUEST=0 , UNIQUE=2, EST=3 } zone_t ;
typedef enum zone_id_s { ZONE_ID_OUEST_OE=0, ZONE_ID_OUEST_EO=1, ZONE_ID_UNIQUE=2, ZONE_ID_EST_OE=3, ZONE_ID_EST_EO=4 } zone_id_t ; 

typedef struct trajet_s
{
     /* Gestion */
     pthread_mutex_t mutex ;		/* semaphore protegeant ce trajet */
     sens_t sens ;			/* sens courant */
     train_id_t max_trains_voie_unique ;	
     train_id_t cpt_trains_voie_unique ; 
     /* Donnees */
     train_id_t cpt[5] ;   
     train_t zone[5][MAX_TRAINS] ;
} trajet_t ;

/* 
 * Definitions des fonctions 
 */

extern void trajet_zone_print( zone_t z ) ;
extern void trajet_zone_id_print( zone_id_t z ) ;

extern int trajet_initialiser( trajet_t * t ) ;

extern void trajet_afficher( trajet_t * const trajet ) ;
extern void trajet_mapper( trajet_t * const trajet ) ;

extern int trajet_inserer( trajet_t * tj ,     /* trajet */      
			   const train_t tn ,  /* train a inserer */
			   const zone_t z,     /* zone */        
			   const sens_t s );         /* sens */   

extern int trajet_extraire( trajet_t * tj ,      /* trajet */           
			    const train_t tn ,   /* train a extraire */   
			    const zone_t z,      /* zone */             
			    const sens_t s ) ;         /* sens */             

extern int trajet_fermer( trajet_t * t );

#endif
