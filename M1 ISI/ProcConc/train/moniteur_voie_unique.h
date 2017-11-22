#ifndef _MONITEUR_VOIE_UNIQUE_H_
#define _MONITEUR_VOIE_UNIQUE_H_

#include <trajet.h>

extern void voie_unique_initialiser( trajet_t * trajet , const train_id_t nb ) ;
extern void voie_unique_fermer( trajet_t * trajet );

extern void voie_unique_entree_ouest( trajet_t * trajet ) ;
extern void voie_unique_sortie_ouest( trajet_t * trajet ) ;
extern void voie_unique_entree_est( trajet_t * trajet ) ;
extern void voie_unique_sortie_est( trajet_t * trajet ) ;

#endif
