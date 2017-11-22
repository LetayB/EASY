#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define DEBUG 0

//Fonction qui renvoie 0 si x appartient au tableau de taille n  , 1 sinon
int appartient_pid(pid_t * tab,pid_t x,int n);

//Tri par ordre croissant un tableau t de taille n et renvoie la valeur mediane ( haute pour un tableau pair)
int tri_bulle(int * t,int n);

void main(int argc,char ** argv){

	/* Gestion des arguments */
	if(argc != 4){
		printf("Erreur nombre d'arguments , Usage : %s <N> <commande> <M>",argv[0]);
			exit(-1);
	}

	/* nb_proc et nb_exec sont des longs afin de pouvoir faire un grand nombre d'execution 
	et pour permettre la verification que l'argument est bien rentré avec strtol */
	long nb_proc = strtol(argv[3], NULL, 10); 
	if(errno != EXIT_SUCCESS){	
		printf("Erreur argument 3 n'est pas un long");
		exit(-1);
	}

	long nb_exec = strtol(argv[1], NULL, 10);
	if(errno != EXIT_SUCCESS){	
		printf("Erreur argument 1 n'est pas un long");
		exit(-1);
	}

	char com[256];
	strcpy(com,argc[2]);


	/* Creation des fils */

	/* Tableau contenant le pid des fils , permet de filtrer les petits fils et de connaitre le temps d'exec des fils */
	pid_t * pid_fils = malloc(nb_proc * sizeof(pid_t));

	int i ;


	for(i = 0;i<nb_proc,i++){
		switch(pid = fork()){
			case -1 :
				printf("Erreur fork processus %d",i);
				exit(-1);
				break;
			case 0 : /* Fils */
				timeval temps_debut;
				timeval temps_fin;
				timeval temps_final;
				int somme = 0; // En microsecondes

				if(gettimeofday(temps_debut,NULL) != 0 ){ //On recupere l'heure 
					printf("Erreur getimeofday");
					exit(-1);
				}

				/* execution de K fois la commande */
				int j ;
				for(j = 0;j<nb_exec,j++){
					switch(pid = fork()){
						case -1 :
							printf("Erreur fork processus %d",i);
							exit(-1);
						case 0 :
							execl(com,com,NULL);
							exit(0);
					}
				}
				while(wait(&cr)){
					//On attend la fin de tout les processus
				}
				

				if(gettimeofday(temps_fin,NULL) != 0 ){ //On recupere l'heure 
					printf("Erreur getimeofday");
					exit(-1);
				}
				//On calcule le temps d'execution
				temps_final.tv_sec = temps_fin.tv_sec - temps_debut.tv_sec ;
				temps_final.tv_usec = temps_fin.tv_usec - temps_debut.tv_usec ;

				somme += temps_final.tv_usec + (1000000 * temps_final.tv_sec );
				if(DEBUG)	//Test
					printf("\nDEBUG : Programme execute en %ds:%dus = %d",temps_final.tv_sec,temps_final.tv_usec,somme); 
				if(DEBUG)	//Test
					printf("\nDEBUG :Fin du fils %d",i); 

				exit(somme);//On retourne la somme

			default : /* Pere */
				pid_fils[i] = pid; // On insere le pid du fils dans le tableau 
		}
	}

	int nb_fils_fini = 0;
	int * tab = malloc(nb_proc * sizeof(int)); //Tab contenant les resultats

	while(pid = wait(&cr)){
		if(appartient_pid(pid_fils,pid,nb_proc)){	//On ne traite que les fils , pas les petits fils
			if(DEBUG)
				printf("Fils pid %d valeur retournée : %d",pid,cr);

			tab[nb_fils_fini] = cr;
			nb_fils_fini ++;
		}
		else{//On verifie que le petit fils se soit arreter correctement
			if(cr != 0)
				printf("Petit fils de pid %d interrompu avec l'exit %d",pid,cr);
		}
	}

	int mediane = tri_bulle(tab,nb_proc);
	printf("\nValeur mediane des temps d'execution : %d\n",mediane);

	if(DEBUG){ //Test
		printf("\nDEBUG : Contenu du tableau\n");
		for(i=0;i<nb_proc;i++){
			printf(" %d ",tab[i]);
		}
		printf("\n");
	}
}


int appartient_pid(pid_t * tab,pid_t x,int n){
	int i = 0;
	for(i=0;i<n;i++){
		if(x == tab[i])
			return 0;
	}
	return 1;
}


int tri_bulle(int * t,int n){
	int j = 0;
	int tmp = 0;
	int disorder = 1;
	while(disorder){
		disorder = 0;
		for(j=0;j<n;j++){
			if(t[j]>t[j+1]){
				tmp = t[j+1];
				t[j+1] = t[j];
				t[j] = tmp;
				disorder = 1;
			}
		}
	}
	//On renvoie la valeur mediane
	return(tab[n/2+1]);
}