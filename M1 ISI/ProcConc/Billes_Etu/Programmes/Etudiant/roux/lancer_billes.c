#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include <ecran.h>

typedef struct {
  aire_t *A;
  ecran_t *ecran;
  bille_t *bille;
  int b;
} pthread_args_t;

booleen_t fini = FAUX ;

static pthread_mutex_t mutex_deplacement = PTHREAD_MUTEX_INITIALIZER;

static void *thread_bille( void *data )
{
  char mess[ECRAN_LG_MESS] ;
  pthread_args_t *p_data = (pthread_args_t*)data;
  aire_t *A = p_data->A;
  ecran_t *ecran = p_data->ecran;
  bille_t *bille = p_data->bille ;
  char txt[20];
  int b = p_data->b;

  sprintf(txt, "J'ai bouge %d", b);

  while( (pthread_mutex_lock( &mutex_deplacement ) == 0) && bille_active(bille) )
  {

    if( ecran_bille_deplacer( ecran , A, bille ) )
      {
        ecran_message_afficher( ecran , txt ) ;
        pthread_mutex_unlock( &mutex_deplacement );
        return(NULL);
      }

    if( bille_active(bille) )
  	  {
        ecran_afficher( ecran , A );
        pthread_mutex_unlock( &mutex_deplacement );
  	    /* Arret sur image de ce mouvement pendant une duree inversement proportionnelle a sa vitesse */
        ecran_bille_temporiser(bille) ;
  	  }
    else
  	  {
  	    /* Desintegration de la bille */
  	    sprintf( mess , "Desintegration de la bille %d" , b ) ;
  	    ecran_message_afficher( ecran , mess ) ;
  	    ecran_bille_desintegrer( ecran , bille ) ;

  	    /* On enleve cete bille de l'aire */
  	    if( aire_bille_detruire( A , &bille ) )
          {
		        sprintf( mess , "Pb lors de la destruction de la bille %d", b ) ;
  	        ecran_message_pause_afficher( ecran , mess ) ;
  	        ecran_stderr_afficher( ecran ) ;
  		    }
        pthread_mutex_unlock( &mutex_deplacement );
        return(NULL);
      }
    }
    pthread_mutex_unlock( &mutex_deplacement );
    return(NULL);
}

static
void arret( int sig )
{
  static int cpt = 0 ;

  cpt++ ;

  switch( cpt )
    {
    case 1 :
      break ;
    case 2 :
      break ;
    case 3 :
      fini = VRAI ;
      break ;
    default :
      break  ;
    }
  return ;
}

int
main( int argc , char * argv[] )
{
  aire_t * A  = NULL ;
  err_t cr = OK ;
  ecran_t * ecran  = NULL ;
  char mess[ECRAN_LG_MESS] ;

  signal( SIGINT , arret ) ;

  printf("\n\n\n\t===========Debut %s==========\n\n" , argv[0] );

  if( argc != 4 )
    {
      printf( " Programme de test sur l'affichage de l'aire avec plusieurs billes\n" );
      printf( " usage : %s <Hauteur de l'aire> <Largeur de l'aire> <nb billes>\n" , argv[0] );
      exit(0);
    }

  int Hauteur  = atoi( argv[1] ) ;
  int Largeur  = atoi( argv[2] ) ;
  int NbBilles = atoi( argv[3] ) ;
  printf( " \nTest sur une aire de %dX%d et %d billes\n" , Hauteur , Largeur , NbBilles );

  srand( getpid() ) ;

  /* Creation et affichage de l'aire */
  printf("Creation de l'aire %dX%d\n" , Hauteur , Largeur ) ;
  if( ( A = aire_creer( Hauteur , Largeur ) ) == AIRE_NULL )
    {
      printf("Pb creation aire\n");
      exit(-1) ;
    }

  aire_afficher(A) ;

  /* Creation et affichage de l'ecran */
  if( ( ecran = ecran_creer(A) ) == ECRAN_NULL )
    {
      printf("Pb creation ecran\n");
      exit(-1) ;
    }

  ecran_afficher(  ecran , A )  ;
  ecran_message_afficher( ecran , "Envoyez un signal pour continuer" ) ;
  pause() ;

  /** Si il y as trop de billes, on diminue ce nombre */
  int max_billes = Hauteur*Largeur;
  if(NbBilles > (max_billes-1)){
    NbBilles = max_billes-1;
  }

  /* Creation des billes */
  sprintf( mess , "Creation des %d billes\n" , NbBilles) ;

  ecran_message_pause_afficher( ecran , mess );

  bille_t ** tab_billes = (bille_t **)malloc( sizeof(bille_t *) * NbBilles ) ;

  int b ;
  for( b=0 ; b<NbBilles ; b++ )
    {
      if( ( tab_billes[b] = bille_creer( direction_random() ,
					 rand()%BILLE_VITESSE_MAX+10 ,
					 COORD_NULL ,
					 '*' ) ) == BILLE_NULL )
	{
	  sprintf( mess , "Pb creation bille bille %d\n" , b );
	  ecran_message_pause_afficher( ecran , mess  ) ;
	  exit(-1) ;
	}
    }

  /* Positionnements et affichages des billes */
  ecran_message_pause_afficher( ecran , "Positionnement des billes sur l'aire" ) ;
  for( b=0 ; b<NbBilles ; b++ )
    {
      sprintf( mess , "Positionnement de la bille %d\n" , b ) ;
      ecran_message_afficher( ecran , mess ) ;
      if( ( cr = aire_bille_positionner( A , tab_billes[b] ) ) )
	{
	  sprintf( mess , "Pb lors du positionnement de la bille %d" , b ) ;
	  ecran_message_pause_afficher( ecran , mess ) ;
	  erreur_afficher(cr) ;
	  ecran_stderr_afficher(ecran) ;
	  exit(-1) ;
	}
      sprintf( mess , "Positionnement de la bille numero %d en [%d,%d]"  ,
	       b , coord_lig_lire(bille_coord_lire(tab_billes[b])) , coord_col_lire(bille_coord_lire(tab_billes[b])) ) ;
      ecran_message_pause_afficher( ecran ,  mess ) ;
    }

  ecran_message_pause_afficher( ecran , "Poses des billes sur l'ecran" ) ;
  for( b=0 ; b<NbBilles ; b++ )
    {
      sprintf( mess , "Pose de la bille %d\n" , b ) ;
      ecran_message_afficher( ecran , mess ) ;
      if( ( cr = ecran_bille_poser( ecran , A , tab_billes[b] ) ) )
	{
	  sprintf( mess , "Pb lors de la pose de la bille %d" , b ) ;
	  ecran_message_pause_afficher( ecran , mess ) ;
	  erreur_afficher(cr) ;
	  ecran_stderr_afficher(ecran) ;
	  exit(-1) ;
	}
    }

  ecran_message_afficher( ecran , "Envoyez un signal pour continuer" ) ;
  pause() ;

  // /* Deplacements des billes l'une apres l'autre */
  // ecran_message_pause_afficher( ecran , "Test deplacement des billes, (Deplacements jusqu'a un signal)" ) ;
  //
  // liste_t * liste_billes = LISTE_NULL ;
  // liste_id_t nb_billes = 0 ;
  // bille_t * bille = BILLE_NULL ;
  // while( ! aire_vide(A) )
  //   {
  //     liste_billes = aire_billes_lire(A) ;
  //     nb_billes = liste_nb_lire(liste_billes) ;
  //
  //     for( b=0 ; b<nb_billes ; b++ )
	// {
	//   /* Deplacement d'une bille */
	//   bille = liste_elem_lire( liste_billes , b ) ;
  //
	//   if( ecran_bille_deplacer( ecran , A, bille ) )
	//     {
	//       sprintf( mess ,  "Pb lors du deplacement de la bille %d", b ) ;
	//       ecran_message_pause_afficher( ecran , mess ) ;
	//       ecran_stderr_afficher( ecran ) ;
	//       goto fin ;
	//     }
  //
	//   if( bille_active(bille) )
	//     {
	//       /* Arret sur image de ce mouvement pendant une duree inversement proportionnelle a sa vitesse */
	//       ecran_bille_temporiser(bille) ;
	//     }
	//   else
	//     {
	//       /* Desintegration de la bille */
	//       sprintf( mess , "Desintegration de la bille %d" , b ) ;
	//       ecran_message_afficher( ecran , mess ) ;
	//       ecran_bille_desintegrer( ecran , bille ) ;
  //
	//       /* On enleve cete bille de l'aire */
	//       if( aire_bille_detruire( A , &bille ) )
	// 	{
	// 	  sprintf( mess , "Pb lors de la destruction de la bille %d", b ) ;
	// 	  ecran_message_pause_afficher( ecran , mess ) ;
	// 	  ecran_stderr_afficher( ecran ) ;
	// 	  goto fin ;
	// 	}
	//     }
	// }
  //   }

    /* Lancer les billes avec des pthreads */
  ecran_message_pause_afficher( ecran , "Test de creation des threads" ) ;

  liste_t * liste_billes = LISTE_NULL ;
  liste_id_t nb_billes = 0 ;
  bille_t * bille = BILLE_NULL ;

  liste_billes = aire_billes_lire(A) ;
  nb_billes = liste_nb_lire(liste_billes) ;

  pthread_t tid_billes[50];// = (pthread_t*)malloc(sizeof(pthread_t)*nb_billes);
  pthread_args_t p_data;

  p_data.A = A;
  p_data.ecran = ecran;

  pthread_mutex_lock( &mutex_deplacement );
  for(b=0; b<nb_billes; b++)
  {
	  bille = liste_elem_lire( liste_billes , b ) ;
    p_data.bille = bille;
    p_data.b = b;
    pthread_create(&tid_billes[b], NULL, (void*)thread_bille, (void*)&p_data);
    usleep(1000);
  }
  pthread_mutex_unlock( &mutex_deplacement );

  while( ! aire_vide(A) )
    {
      liste_billes = aire_billes_lire(A) ;
      nb_billes = liste_nb_lire(liste_billes) ;

      pthread_mutex_lock( &mutex_deplacement );
      ecran_afficher( ecran , A );
      pthread_mutex_unlock( &mutex_deplacement );

      //sleep(50.0);
    }

  ecran_message_pause_afficher( ecran , "Arret" ) ;

  ecran_message_pause_afficher( ecran , "Attente de la fin des threads" ) ;
  for(b=0; b<nb_billes; b++)
  {
    pthread_join(tid_billes[b], NULL);
  }

  //free(tid_billes);

  ecran_message_pause_afficher( ecran , "Destruction de la structure ecran" ) ;
  ecran_detruire(&ecran)  ;

  printf("\nDestruction aire\n" ) ;
  aire_detruire( &A)  ;

  printf("\n\n\t===========Fin %s==========\n\n" , argv[0] );

  return(0) ;
}
