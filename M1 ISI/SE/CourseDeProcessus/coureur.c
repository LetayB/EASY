#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>

#include "messages.h"

int main(int argc, char * argv[])
{
	int i, file_id, file_cle;

	/* Test du nombre d'arguments */
	if( argc != 2 ){
		printf("Usage : <%s> <cle externe>\n",argv[0]);
	}

	file_cle = atoi(argv[1]);
	if( file_cle <= 0 ){
		printf("Erreur => L'argument <cle externe> est invalide.\n");
	}

	/* Recuperation de la file de messages */
	if((file_id = msgget(file_cle , 0))==-1){
		printf("Erreur => Recuperation de la file non reussie\n");
		exit(-1);
	}

	printf("====File : %i=====\n=====Cle : %i====\n",file_id,file_cle);

	//Creation de la requete,contenant le pid du processus
	requete_t requete;
	requete.corps.dossard = getpid();
	requete.type = PC_COURSE;

	//Creation de la reponse
	reponse_t reponse;
	
	messages_initialiser_attente() ;
	

    //Boucle principale
	for(i=0; reponse.corps.etat == EN_COURSE ; i++){
		
		printf("Envoi...\n");

		if( msgsnd(file_id,&requete,sizeof(corps_requete_t),0)==-1){
			printf("Erreur => Envoi du message au serveur échoué\n");
			exit(-1);
		}

		printf("Réception...\n");

		if( msgrcv(file_id,&reponse,sizeof(reponse),requete.corps.dossard,0)==-1){
			printf("Erreur => Reception de la réponse du serveur échoué\n");
			exit(-1);
		}

		messages_afficher_reponse(&reponse);
		
		if( reponse.corps.etat == ARRIVE ){
			printf("\n\n========Ce Processus est Gagnant !!!!========\n");
			exit(0);
		}	
		else if( reponse.corps.etat == DECANILLE ){
			printf("\n\n========Ce Processus est Decanillé !!========\n");
			exit(0);
		}
		else if( reponse.corps.etat != EN_COURSE ){
			printf("Erreur => Status du processus invalide");			
			exit(-1);
		}
		messages_attendre_tour() ;
	}
	exit(0);
}
