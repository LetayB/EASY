/****************************************************

MODIFICATION DE liste_elem_ajouter/2
===> RETOURNE LA POSITION DE L'ELEMENT AJOUTE

extern
int
liste_elem_ajouter( liste_t * liste,
        const elem_t elem )
{
  int pos = 0;
  liste->liste[liste->nb] = elem ;

  liste->nb++ ;

  pos = liste->nb;
  
  return(pos) ;
}

Bug => etat du cheval

*****************************************************/

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

/* Définition des structures pour les opérations P et V */
static struct sembuf Op_P = {0,-1,0};
static struct sembuf Op_V = {0,1,0};

/* Définition des sémaphores des cases, de la piste et de la liste */
static int tab_cases[20];
static int sem_liste, sem_piste;

/* Définition de la fonction d'initialisation des sémaphores */
static void 
init_sem(int semId)
{
  semId = semget(IPC_PRIVATE, 1, 0666);
  if (semId == -1)
  {
    perror("init_sem | semget ");
    exit(-1);
  }
  if (semctl(semId, 0, SETVAL, 1) == -1)
  {
    perror("init_sem | semctl ");
    exit(-1);
  }
}

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

static elem_t
get_indice_cheval_by_cell(elem_t * elem_cheval_to_decanille,int * ind_cheval, liste_t * liste, cell_t cell)
{
  int i;
  *ind_cheval = -1;
  for (i = 0; i < liste_nb_lire(liste) ; ++i)
  {
    if (cell_comparer(elem_cell_lire(liste_elem_lire(liste, i)), cell) == 0)
    {
      *ind_cheval = i;
      
      *elem_cheval_to_decanille = liste_elem_lire(liste, i);
      elem_etat_lire(liste_elem_lire(liste, i));
    }
  }
  return liste_elem_lire(liste, -1);
}





/*************************************/
/*************** MAIN **************/
/*************************************/
int 
main( int nb_arg , char * tab_arg[] )
{     

  int ind;
  
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

  int ind_cheval;
  elem_t elem_cheval_to_decanille ;


  /******************************/
  /* Vérification des paramètres */
  /******************************/
  if( nb_arg != 4 )
  {
    fprintf( stderr, "usage : %s <cle de piste> <cle de liste> <marque>\n" , tab_arg[0] );
    exit(-1);
  }
  
  if( sscanf( tab_arg[1] , "%d" , &cle_piste) != 1 )
  {
    fprintf( stderr, "%s : erreur , mauvaise cle de piste (%s)\n" , 
      tab_arg[0]  , tab_arg[1] );
    exit(-2);
  }

  if( sscanf( tab_arg[2] , "%d" , &cle_liste) != 1 )
  {
    fprintf( stderr, "%s : erreur , mauvaise cle de liste (%s)\n" , 
      tab_arg[0]  , tab_arg[2] );
    exit(-2);
  }

  if( sscanf( tab_arg[3] , "%c" , &marque) != 1 )
  {
    fprintf( stderr, "%s : erreur , mauvaise marque de cheval (%s)\n" , 
      tab_arg[0]  , tab_arg[3] );
    exit(-2);
  }


  /******************************/
  /* Attachement de la liste */
  /******************************/
  int shmid_liste;
  shmid_liste = shmget(cle_liste, LISTE_MAX*sizeof(int), IPC_CREAT | 0666 );
  if (shmid_liste == -1)
  {
    perror("Problème création de Mémoire partagée | shmget liste | cheval.c");
    exit(-3);
  }
  liste = shmat(shmid_liste, 0, 0);

  //comparaison entre pointeur et integer
  /*if (liste == -1)
  {
    perror("Problème d'accès à la mémoire partagée | shmat liste | cheval.c");
    exit(-4);
  }*/

  /******************************/
  /* Attachement de la piste */
  /******************************/
  int shmid_piste;
  shmid_piste = shmget(cle_piste, PISTE_LONGUEUR*sizeof(int), IPC_CREAT | 0666 );
  if (shmid_piste == -1)
  {
    perror("Problème création de Mémoire partagée | shmget piste | cheval.c");
    exit(-3);
  }
  piste = shmat(shmid_piste, 0, 0);

  //comparaison entre pointeur et integer
  /*if (shmat(shmid_piste, 0, 0) == -1)
  {
    perror("Problème d'accès à la mémoire partagée | shmat piste | cheval.c");
    exit(-4);
  }*/


  /******************************************/
  /* Créations des sémaphores pour les cases */
  /******************************************/
  int i;
  for (i = 0; i < 20; ++i)
  {
    init_sem(tab_cases[i]);
  }

  /******************************************/
  /* Création du sémaphore pour la liste */
  /* Création du sémaphore pour la piste */
  /******************************************/
  init_sem(sem_liste);
  init_sem(sem_piste);



  /******************************************/
  /* Initialisation de l'attente et du cheval */
  /******************************************/
  /* Init de l'attente */
  commun_initialiser_attentes() ;

  /* Init de la cellule du cheval pour faire la course */
  cell_pid_affecter( &cell_cheval  , getpid());
  cell_marque_affecter( &cell_cheval , marque );

  /* Init de l'element du cheval pour l'enregistrement */
  elem_cell_affecter(&elem_cheval , cell_cheval ) ;
  elem_etat_affecter(&elem_cheval , EN_COURSE ) ;


  /******************************************/
  /* Créations du sémaphores pour le cheval */
  /******************************************/
  if (elem_sem_creer(&elem_cheval) == -1)
  {
    perror("Pb création sémaphore du cheval");
    exit(-5);
  }

  /******************************************/
  /* Ajout du cheval a la liste */
  /******************************************/
  P(sem_liste);
  if ((ind = liste_elem_ajouter(liste, elem_cheval)) == 0 ) //voir modif en haut de ce fichier
  {
    printf("Erreur de creation du cheval \"%c\"\n", marque);
  }
  V(sem_liste);







  /******************************************/
  /* Début de la course de petits chevaux  */
  /******************************************/
  int numeroTour = 0;
  printf("JE COMMENCE LA COURSE => \"%c\"\n", marque );
  while( ! fini )
  {

    /* si c'est le premier tour on n'attends pas */
    numeroTour = numeroTour + 1;
    if (numeroTour != 1)
    {
      commun_attendre_tour() ;
    }

    /*********************************************************/
    /* Prise de sémaphore du cheval et de la case actuelle*/
    /*********************************************************/
    if (elem_sem_verrouiller(&elem_cheval) == -1)
    {
      perror("Pb elem_cheval prise sem 1");
      exit(-5);
    }


    /******************************************/
    /* Prise de sémaphore de la liste */
    /******************************************/
    P(sem_liste);

    /* Recherche le cheval elem_cheval dans la liste liste et retourne son indice dans ind */
    liste_elem_rechercher(&ind, liste, elem_cheval );

    /******************************************/
    /* Vérification si décanillé */
    /******************************************/
    if (elem_decanille(liste_elem_lire( liste , ind )))
    {
      /* course finie */
      fini = VRAI;

      /****************************************************************************/
      /* Prise de sémaphore de la piste, suppression du cheval, rendu du sémaphore */
      /****************************************************************************/
      P(sem_piste);
      piste_cell_effacer(piste, ind);
      V(sem_piste);


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
      printf("suppression du cheval \"%c\"\n", marque);

      /********************************************************/
      /* Rendu du sémaphore du cheval et de la piste */
      /********************************************************/
      V(sem_liste);
      if (elem_sem_deverrouiller(&elem_cheval) == -1)
      {
        perror("Pb elem_cheval supp sem");
        exit(-5);
      }

      /* Destruction du sémaphore du cheval */
      elem_sem_detruire(&elem_cheval);
      exit(0);
    }
    /* Si pas décanillé on rend le sémaphore sur la liste pris auparavant*/
    else
    {
      V(sem_liste);
    }


    /******************************************/
    /* Lancement du dé */
    /******************************************/
    deplacement = commun_coup_de_de() ;
    #ifdef _DEBUG_
      printf(" Lancement du De --> %d par \"%c\" \n", deplacement, marque );
    #endif

    /* Calcule de la case d'arrivée */
    arrivee = depart+deplacement ;


    if( arrivee > PISTE_LONGUEUR-1 || arrivee == PISTE_LONGUEUR-1)
    {
      /* Si on arrive sur la ligne ou qu'on la dépasse on gagne */
      arrivee = PISTE_LONGUEUR-1 ;
      fini = VRAI ;

      /*********************************************/
      /* On prends la case d'arrivée et de départ */
      /*********************************************/
      P(tab_cases[depart]);
      P(tab_cases[arrivee]);

      /* Pas de vérification de cheval sur la case d'arrivée car il n'est pas censé y en avoir */

      /* On attend quand même la fin du saut */
      commun_attendre_fin_saut();

      /* On libère la case de départ */
      V(tab_cases[depart]);


      /*********************************************/
      /* Suppression du cheval gagnant de la liste */
      /*********************************************/
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


      /*********************************************/
      /* Suppression du cheval gagnant de la piste */
      /*********************************************/
      P(sem_piste);
      piste_cell_effacer(piste, ind);
      V(sem_piste);

      /* On libère la case d'arrivée */
      V(tab_cases[arrivee]);

    }

    if (depart == arrivee)
    {
      perror("Vous avez déjà gagné, pas la peine de relancer le de");
      exit(-10);
    }


    /*********************************************************************************/
    /* Prise du sémaphore de la case actuelle et de la case d'arrivée et de la piste */
    /*********************************************************************************/
    P(tab_cases[depart]);
    P(tab_cases[arrivee]);
    P(sem_piste);


    /*****************************************************************/
    /* Si un cheval est sur la case d'arrivée on le decanille */
    /*****************************************************************/
    if (piste_cell_occupee( piste, arrivee))
    {

      /******************************************************/
      /* Récupération de l'indice et du cheval à décaniller  */
      /******************************************************/
      P(sem_liste);
      get_indice_cheval_by_cell(&elem_cheval_to_decanille, &ind_cheval, liste, piste->cell[arrivee]);


      /******************************************/
      /* Prise du sémaphore */
      /******************************************/

      if (elem_sem_verrouiller(&liste->liste[ind_cheval]) == -1)
      {
        perror("Pb elem_cheval prise sem 2");
        V(sem_liste);
        exit(-5);
      }
      else
      {  
        /******************************************/
        /* Décanillage du cheval adverse */
        /******************************************/
        liste_elem_decaniller(liste, ind_cheval);

        /******************************************/
        /* Rendu du sémaphore */
        /******************************************/
        if (elem_sem_deverrouiller(&elem_cheval_to_decanille) == -1)
        {
          V(sem_liste);
          perror("Pb elem_cheval supp sem");
          exit(-5);
        }
      }

      V(sem_liste);
    }


    /******************************************/
    /* Deplacement du cheval */
    /******************************************/
    piste_cell_effacer(piste ,depart) ;
    piste_cell_affecter(piste ,arrivee ,cell_cheval );


    /**************************************************/
    /* Libération du sémaphore de la case de départ */
    /**************************************************/
    V(tab_cases[depart]);

    #ifdef _DEBUG_
      printf("Deplacement du cheval \"%c\" de %d a %d\n",
      marque, depart, arrivee );
    #endif

    /******************************************/
    /* Attente de la fin du saut */
    /******************************************/
    commun_attendre_fin_saut();

    /******************************************/
    /* Affichage de la piste */
    /******************************************/
    piste_afficher_lig( piste );
    depart = arrivee ;


    /****************************************************************/
    /* Déverrouillage de la piste, de la case arrivée et du cheval */
    /****************************************************************/
    V(tab_cases[arrivee]);
    V(sem_piste);
    if (elem_sem_deverrouiller(&elem_cheval) == -1)
    {
      perror("Pb elem_cheval liberation sem");
      exit(-5);
    }

  }
  exit(0);
}

