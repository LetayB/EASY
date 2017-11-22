/* Regler la gestion des semaphore de la piste */


#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <commun.h>
#include <liste.h>
#include <piste.h>

/* Liste des sémaphores des différentes cases */
static int sem_cases[20];

/* Définition des structures pour les opérations P et V */
static struct sembuf Op_P = {0,-1,0};
static struct sembuf Op_V = {0,1,0};

/* Définition des fonctions P et V*/
static void 
P(int semId)
{
  semop(semId, &Op_P, 1);
}
static void 
V(int semId)
{
  semop(semId, &Op_V, 1);
}


/*===============================*
|              Main              |
*===============================*/
int 
main( int nb_arg , char * tab_arg[] )
{     

  int cle_piste ;
  piste_t * piste = NULL ;

  int cle_liste ;
  liste_t * liste = NULL ; 

  char marque ;

  booleen_t fini = FAUX ;
  piste_id_t deplacement = 0 ;
  piste_id_t depart = 0 ;
  piste_id_t arrivee = 0 ;

  
  cell_t cell_cheval ;


  elem_t elem_cheval ;

  int num_tour = 0;

  /* Segment de mémoire partagé contenant la liste des chevaux */
  int shmid_liste;
  /* Segment de mémoire partagé contenant la piste */
  int shmid_piste;

  int sem_liste;

  int i;
  /*Indice du cheval dans la liste */
  int ind_cheval;

  int ind_cheval_deca;



  /*===============================*
  |     Gestion des paramètres     |
  *===============================*/

  if( nb_arg != 4 ){
      fprintf( stderr, "usage : %s <cle de piste> <cle de liste> <marque>\n" , tab_arg[0] );
      exit(-1);
    }
  
  if( sscanf( tab_arg[1] , "%d" , &cle_piste) != 1 ){
      fprintf( stderr, "%s : erreur , mauvaise cle de piste (%s)\n" , 
	       tab_arg[0]  , tab_arg[1] );
      exit(-2);
    }


  if( sscanf( tab_arg[2] , "%d" , &cle_liste) != 1 ){
      fprintf( stderr, "%s : erreur , mauvaise cle de liste (%s)\n" , 
	       tab_arg[0]  , tab_arg[2] );
      exit(-2);
    }

  if( sscanf( tab_arg[3] , "%c" , &marque) != 1 ){
      fprintf( stderr, "%s : erreur , mauvaise marque de cheval (%s)\n" , 
	       tab_arg[0]  , tab_arg[3] );
      exit(-2);
    }

  /*==============================*
  |     Segment memoire liste     |
  *==============================*/
  
  /* Récuperation de l'indentifiant du segment */
  if ((shmid_liste = shmget(cle_liste, LISTE_MAX*sizeof(int),0)) == -1){
    perror("Erreur shmget list");
    exit(-3);
  }
  /* Attachament a une adresse chosiie par le systeme */
  if ((liste = shmat(shmid_liste, 0, 0)) == (int *)-1){
    perror("Erreur shmat list");
    exit(-3);
  }

  /*==============================*
  |     Segment memoire piste     |
  *==============================*/

  /* Récuperation de l'identifiant du segment */
  if ((shmid_piste = shmget(cle_piste, PISTE_LONGUEUR*sizeof(int),0)) == -1){
    perror("Erreur shmget piste ");
    exit(-3);
  }
  /* Attachament a une adresse chosiie par le systeme */
  if ((piste = shmat(shmid_piste, 0, 0)) == (int *)-1){
    perror("Erreur shmat pist");
    exit(-3);
  }

  /*========================*
  |     Sémaphore Liste     |
  *========================*/

  /* Récuperation de l'identifiant du semaphore */
  if ((sem_liste = semget(cle_liste,1,0)) == -1){
    perror("Erreur semget list");
    exit(-3);
  }

  /*========================*
  |     Sémaphores Piste    |
  *========================*/

  for (i = 0; i < 20; ++i){
 
  }
  
  /* Init de l'attente */
  commun_initialiser_attentes() ;

  /* Init de la cellule du cheval pour faire la course */
  cell_pid_affecter( &cell_cheval  , getpid());
  cell_marque_affecter( &cell_cheval , marque );

  /* Init de l'element du cheval pour l'enregistrement */
  elem_cell_affecter(&elem_cheval , cell_cheval ) ;
  elem_etat_affecter(&elem_cheval , EN_COURSE ) ;


  /*=========================*
  |     Sémaphores cheval    |
  *=========================*/

  if (elem_sem_creer(&elem_cheval) == -1)
  {
    perror("Pb création sémaphore du cheval");
    exit(-5);
  }

  /*=========================*
  |     Ajout cheval liste   |
  *=========================*/
  
  /* Accès à la liste */
  P(sem_liste);
  if ((ind = liste_elem_ajouter(liste, elem_cheval)) == 0 ) //voir modif en haut de ce fichier
  {
    printf("Erreur de creation du cheval \"%c\"\n", marque);
  }
  V(sem_liste);





  /*=============*
  |    Course    |
  *=============*/

  printf("Le cheval %c débute la course\n", marque);
  while( ! fini )
    {
      num_tour ++;

      /*Pas d'attente lors du premier tour */
      if(num_tour != 1){
        /* Attente entre 2 coup de de */
        commun_attendre_tour() ;
      }
      
      /* On prend l'accès sur le sémaphore du cheval */
      if (elem_sem_verrouiller(&elem_cheval) == -1){
        perror("Pb elem_cheval prise sem 1");
        exit(-5);
      }

      /* Prise d'accès à la liste */
      P(sem_liste);

      /* Recherche le cheval elem_cheval dans la liste */
      liste_elem_rechercher(&ind_cheval, liste, elem_cheval );



      /*==============================*
      |     Verif si pas decanille    |
      *==============================*/
      if (elem_decanille(liste_elem_lire(liste,ind_cheval))){
        /* La course est fine pour ce cheval */
        fini = VRAI;

        /* Prise de sémaphore de la piste, suppression du cheval, rendu du sémaphore */
        P(sem_piste);
        piste_cell_effacer(piste, ind);
        V(sem_piste);

        /* On vérifie qu'il existe dans la liste */
        if(liste_elem_rechercher(&ind ,liste, elem_cheval) == -1 ){
          printf("Impossible de trouver le cheval \"%c\"\n", marque);
        }

        /* Si il est présent on le supprime */
        if (liste_elem_supprimer(liste, ind) != 0){
          printf("Erreur de suppression du cheval \"%c\"\n", marque);
        }
        printf("suppression du cheval \"%c\"\n", marque);

        V(sem_liste);

        /* Destruction du sémaphore du cheval */
        elem_sem_detruire(&elem_cheval);
        exit(0);
      }
      else{
        V(sem_liste);
      }

      /*=============================*
      |     Deplacement sur piste    |
      *=============================*/

      /* Coup de de */
      deplacement = commun_coup_de_de() ;
      #ifdef _DEBUG_
        printf(" Lancement du De --> %d\n", deplacement );
      #endif

      arrivee = depart+deplacement ;

      if( arrivee >= PISTE_LONGUEUR-1 ){
	      arrivee = PISTE_LONGUEUR-1 ;
	      fini = VRAI ;

        P(tab_cases[depart]);

        commun_attendre_fin_saut();

        /* Suppression du cheval gagnant de la liste */
        P(sem_liste);

        printf( "Le cheval \"%c\" A FRANCHIT LA LIGNE D ARRIVEE\n" , marque );
        /* On vérifie qu'il existe dans la liste */
        if(liste_elem_rechercher(&ind ,liste, elem_cheval) == -1 )
        {
          printf("Impossible de trouver le cheval \"%c\"\n", marque);
        }

        /* Si il est présent on le supprime */
        if (liste_elem_supprimer(liste, ind) != 0)
        {
          printf("Erreur de suppression du cheval \"%c\"\n", marque);
        }

        V(sem_liste);

        /* Suppression du cheval gagnant de la piste */

        piste_cell_effacer(piste, ind_cheval);
        V(tab_cases[depart]);
  	   }

      /* Si un cheval est sur la case d'arrivée on le decanille */
      if (piste_cell_occupee( piste, arrivee))
      {
        /* Récupération de l'indice et du cheval à décaniller  */
        P(sem_liste);
        get_indice_cheval_by_cell(&elem_cheval_to_decanille, &ind_cheval, liste, piste->cell[arrivee]);

        if (elem_sem_verrouiller(&liste->liste[ind_cheval_deca]) == -1){
          perror("Erreur sem cheval_deca : verouillage");
          V(sem_liste);
          exit(-5);
        }
        /* Décanillage du cheval adverse */
        liste_elem_decaniller(liste, ind_cheval);

          
        /* Rendu du sémaphore */
        if (elem_sem_deverrouiller(&elem_cheval_to_decanille) == -1){
          V(sem_liste);
          perror("Erreur sem cheval_deca : deverouillage");
          exit(-5);
        }
        

        V(sem_liste);
      }

      /*============================*
      |     Coeur du deplacement    |
      *============================*/
      piste_cell_effacer(piste ,depart) ;
      piste_cell_affecter(piste ,arrivee ,cell_cheval );


      V(tab_cases[depart]);

      #ifdef _DEBUG_
        printf("Deplacement du cheval \"%c\" de %d a %d\n",
        marque, depart, arrivee );
      #endif

      /* Attente de la fin du saut */
      commun_attendre_fin_saut();

      /* Affichage de la piste */
      piste_afficher_lig( piste );
      depart = arrivee ;
    }
  
  exit(0);
}
