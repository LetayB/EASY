#include <com.h>
#include <proc.h>
 
#include <signal.h>
#include <getopt.h>
#include <sys/wait.h>
#include <sys/time.h>
 
 
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
  fprintf( stdout, " Programme de simulation d'un scheduller avec Tourniquet\n" );
  fprintf( stdout , " usage : %s [options] <fichier liste des commandes> <quantum de temps (microsecondes)> \n" , nomprog );
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
  int i ;
  double start , end ;
    struct timeval temps_cur ;
 
  /* Processus */
  proc_t * liste_proc_prets = NULL ; 
  useconds_t quantum = 0 ; 
  pid_t pid;
  pid_t pid_tmp;
 
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
 
 
  if( argc != 2 ) 
    {
      usage(nomprog) ; 
      exit(0); 
    }
 
 
  strcpy( ListeCommandes , argv[0] ) ;
  quantum = atoi(argv[1]) ; 
 
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
 
  for(i = 0;i < nb_commandes; i ++){
 
    switch(pid = fork()){
 
      case -1 :
        if(verbose)
          fprintf( stdout,"Erreur fork processus %d\n",i);
        exit(-1);
      case 0 : //Fils 
      {
      if(verbose)
          fprintf( stdout,"FILS : Processus numÃ©ro : %d , pid : %d crÃ©er\n",i,getpid());
      execv(commandes[i][0],commandes[i]);
 
 
      exit(0);
      }
      default :{ //Pere
        kill(pid,SIGSTOP);
        proc_pid_ecrire(liste_proc_prets+i,pid);
        proc_fini_positionner(liste_proc_prets+i,FAUX);
        if(verbose)
          fprintf( stdout,"PERE : Processus numÃ©ro : %d , pid : %d crÃ©er\n",i,getpid());
        break;
      }
    }
  }
 
  //Recuperation du temps exacte Ã  la microseconde prÃ¨s au dÃ©but de l'envoi
     gettimeofday(&temps_cur, NULL);
     //On place ce temps dans un double plus facile Ã  manipuler que les struct timevals
     start = temps_cur.tv_sec+(temps_cur.tv_usec/1000000.0);
 
  /* 
   * Gestion des processus
   */
 
  if(verbose)
        fprintf( stdout,"Gestion des processus \n");
 
 
  while(!proc_liste_sont_finis(liste_proc_prets,nb_commandes)){
      if(!proc_est_fini(liste_proc_prets[i])){
 
        pid = proc_pid_lire(liste_proc_prets[i]);
        kill(pid,SIGCONT);
        usleep(quantum);
        pid_tmp = waitpid(-1,NULL,WNOHANG);
        if(pid_tmp < 0)
          fprintf( stdout,"Erreur %d",pid_tmp);
        if(pid_tmp == 0){
          kill(pid,SIGSTOP);
        }
        else{ //Le processus est terminÃ©
          proc_fini_vrai(liste_proc_prets+i);
        }
 
      }
 
      i ++;
       if(i >= nb_commandes)
          i = 0;
  }
 
 
   //Recuperation du temps exacte Ã  la microseconde prÃ¨s au dÃ©but de l'envoi
     gettimeofday(&temps_cur, NULL);
     //On place ce temps dans un double plus facile Ã  manipuler que les struct timevals
     end = temps_cur.tv_sec+(temps_cur.tv_usec/1000000.0);
 
 
  if(verbose){
  	printf("\nEtat des processus a la fin du programme :\n");
  	proc_liste_afficher(liste_proc_prets,nb_commandes);
  		printf("\nTemps d'execution  : %f secondes\n", end - start);
  }
 
  printf("\n\n\t===========Fin %s==========\n\n" , nomprog );
 
 return(0) ;
}
 