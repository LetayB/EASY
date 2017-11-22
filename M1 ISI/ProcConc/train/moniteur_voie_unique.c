#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include <sens.h>
#include <train.h>
#include <moniteur_voie_unique.h>

/*---------- MONITEUR ----------*/

train_id_t maxSensUnique;
pthread_cond_t cond_entree_ouest;
pthread_cond_t cond_entree_est;
pthread_mutex_t mutex;
int nbSensOuestEst;
int nbSensEstOuest;


extern void voie_unique_initialiser( trajet_t * trajet ,
				     const train_id_t nb ) 
{
     /* Initialisations des variables du moniteur */
	
	pthread_mutex_init(mutex, NULL);
	pthread_cond_init(&cond_entree_est, NULL);
	pthread_cond_init(&cond_entree_ouest, NULL);
	train_id_t maxSensUnique = nb;
	nbSensOuestEst = 0;
	nbSensEstOuest = 0;
}

extern void voie_unique_fermer( trajet_t * trajet )
{
     /* Destructions des variables du moniteur */
	pthread_mutex_lock(mutex);
	if(trajet_fermer(trajet) != 0)
	{
		printf("moniteur_voie_unique.voie_unique_fermer : trajet_fermer\n");
		exit(-1);
	}
	maxSensUnique = 0; //voie fermée
	pthread_cond_destroy(&cond_entree_ouest);
	pthread_cond_destroy(&cond_entree_est);
	pthread_mutex_destroy(&mutex);
	pthread_mutex_unlock(mutex);
}

extern void voie_unique_entree_ouest( trajet_t * trajet ) 
{
     /* Entree d'un train par l'Ouest dans le sens OUEST --> EST  */ 
	pthread_mutex_lock(mutex);
	if (nbSensEstOuest != 0) //On evite les collisions
	{
		pthread_cond_wait(&cond_entree_ouest,mutex);
	}

	nbSensOuestEst = nbSensOuestEst + 1;
	pthread_mutex_unlock(mutex);
}

extern void voie_unique_sortie_est( trajet_t * trajet ) 
{
     /* Sortie d'un train par l'Est dans le sens OUEST --> EST  */
	pthread_mutex_lock(mutex);
	nbSensOuestEst = nbSensOuestEst - 1;
	if (nbSensOuestEst == 0) //si on etait le dernier dans ce sens
	{
		pthread_cond_broadcast(&cond_entree_est);
		pthread_cond_broadcast(&cond_entree_ouest);

	}

	pthread_mutex_unlock(mutex);
}

extern void voie_unique_entree_est( trajet_t * trajet ) 
{
     /* Entree d'un train par l'Est dans le sens OUEST <-- EST  */ 
	pthread_mutex_lock(mutex);
	if (nbSensOuestEst != 0) //On evite les collisions
	{
		pthread_cond_wait(&cond_entree_ouest,mutex);
	}

	nbSensEstOuest = nbSensEstOuest + 1;
	pthread_mutex_unlock(mutex);
}

	pthread_mutex_unlock(mutex);
}

extern void voie_unique_sortie_ouest( trajet_t * trajet ) 
{
     /* Sortie d'un train par l'Ouest dans le sens OUEST <-- EST  */ 
	pthread_mutex_lock(mutex);
	nbSensEstOuest = nbSensEstOuest - 1;
	if (nbSensEstOuest == 0) //si on etait le dernier dans ce sens
	{
		pthread_cond_broadcast(&cond_entree_est);
		pthread_cond_broadcast(&cond_entree_ouest);

	}
	pthread_mutex_unlock(mutex);
}






