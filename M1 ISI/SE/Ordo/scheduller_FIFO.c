#include <com.h>
#include <proc.h>
 
#include <signal.h>
#include <getopt.h>
 
 
/* ------------------------------ */
/* ----- long options array ----- */
/* ------------------------------ */
 
static struct option longopts[] =
{
  {"verbose", no_argument, NULL, (int)'v'},
  {"help", no_argument, NULL, (int)'h'},
  {0, 0, 0, 0}
};
 
 
/*
 * Usage 
 */
static 
void usage( char * nomprog )
{
  fprintf( stdout, " Programme de simulation d'un scheduller avec Premier Arrivee Prmier Servi\n" );
  fprintf( stdout , " usage : %s [options] <fichier liste des commandes> \n" , nomprog );
  fprintf( stdout , "Options:\n"); 
  fprintf( stdout , "  -v, --verbose                 trace de l'execution acitvee\n");
  fprintf( stdout , "  -h, --help                    affiche cette aide\n");
}
 
 
int
main( int argc , char * argv[] ) 
{
  /* --- Variables --- */
  err_t cr = OK ;
  char nomprog[MAX_STRING] ; 
 
  /* Processus */
  proc_t * liste_proc_prets = NULL ; 
 
 
  /* Commandes */
  char ListeCommandes[MAX_STRING] ; /* Nom du fichier des commandes */  
  char *** commandes = NULL ; /* Liste des commandes */
  int nb_commandes = 0 ; 
 
  /* Options */
  int opt ;
  booleen_t verbose = FAUX ; 
 
  /* --- Gestion parametres + options --- */
  strcpy( nomprog , argv[0] ) ;
  while ((opt = getopt_long(argc, argv, "vh", longopts, NULL)) != -1 )
    {
      switch(opt)
	{
	case 'v': /* --verbose */
	  printf("verbose\n");
	  verbose = VRAI ;
	  break;
 
	case 'h': /* --help */
	  usage(nomprog) ;
	  exit(0);
	  break;
 
	case '?':
	  usage(nomprog) ;
	  exit(0);
	  break;
 
	default:
	  usage(nomprog) ;
	  exit(0);
	  break;
	}
    }
  argc -= optind ;
  argv += optind ;
 
  /* argc == nb de parametres sans le nom de la commande */
  /* argv[0] --> 1er parametre */ 
 
 
  if( argc != 1 ) 
    {
      usage(nomprog) ; 
      exit(0); 
    }
 
 
  strcpy( ListeCommandes , argv[0] ) ;
 
  /* --- Programme --- */
 
  printf("\n\n\n\t===========Debut %s==========\n\n" , nomprog );
 
  /* 
   * Chargement des commandes 
   */
 
  if( ( cr = com_fichier_charger( &commandes , /* tableau commandes a remplir */
				  &nb_commandes , 
				  ListeCommandes ) ) )
    {
      printf( "Pb sur chargement liste de commandes, cr = %d\n" , cr ) ; 
      exit(-1) ; 
    }
 
  if( verbose ) 
    com_liste_afficher(commandes ,  nb_commandes )  ; 
 
 
  /* 
   * Lancement des processus 
   */
 
  /* Reservation memoire tableau des pid */
  liste_proc_prets = malloc( sizeof(proc_t) * nb_commandes ) ;
 
  /* 
   * Creation des processus 
   */

   pid_t pid;
 	for(i = 0;i < nb_commandes; i += 1){
 
		switch(pid = fork()){
 
			case -1 :
				printf("Erreur fork processus %d",i);
				exit(-1);
			case 0 : //Fils 
			{

			execv(commandes[i][0],commandes[i]);
			

			exit(0);
			}
			default :{ //Pere
				kill(pid,SIGSTOP);
				liste_proc_prets[i].pid = pid;
				liste_proc_prets[i].fini = VRAI;
				break;
			}
		}
	}
  
  /* 
   * Gestion des processus
   */
 
  /*****************************/
  /*          A FAIRE          */
  /*****************************/
 
  printf("\n\n\t===========Fin %s==========\n\n" , nomprog );
 
 return(0) ;
}
 