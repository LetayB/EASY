/*
 * Realisation des primitives du TDA bateau  
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include <commun.h>
#include <bateau.h>

/*
 *=============================================================
 * Fonctions de gestion d'un bateau 
 *=============================================================
 */


/* 
 * Ecriture des caracteristiques completes d'un bateau 
 */
extern
int
bateau_ecrire( bateau_t * bateau,
	       coord_t * const coord,
	       const case_t marque,
	       const pid_t pid,
	       const booleen_t actif )
{
     int i ;

     /*----------*/

     for( i=0 ; i<BATEAU_TAILLE ; i++)
	  coord_affecter( &(bateau->corps[i]) , coord[i] );

     bateau->marque = marque ;
     bateau->pid = pid ;
     bateau->actif = actif ;

     return(CORRECT);
}

/* 
 * Affichage de toutes les caracteristiques d'un bateau
 */

extern
void
bateau_afficher( FILE * const stream,
		 const bateau_t bateau )
{

     /*--------------------*/

     /* Affichage position corps du bateau */

     fprintf( stream , "{ corps =" );
     coord_liste_afficher( stream , (coord_t *)bateau.corps , BATEAU_TAILLE);
     fprintf( stream , "\n" );
    
     /* Affichage des autres caracteristiques */

     fprintf( stream , "  marque=%c\n", bateau.marque);
#ifdef _LINUX_
     fprintf( stream , "  pid   =%d\n", bateau.pid);
#else
     fprintf( stream , "  pid   =%ld\n", bateau.pid);
#endif
     fprintf( stream , "  etat  = ");
     if( bateau.actif )
	  fprintf( stream , "ACTIF }\n");
     else
	  fprintf( stream , "MORT }\n");

}

/*
 * Lecture/Ecriture du corps du bateau 
 */

extern 
coord_t *
bateau_corps_lire( bateau_t * const b ) 
{ 
  return( (coord_t *)(b->corps) ) ; 
}
extern 
void 
bateau_corps_ecrire( bateau_t * b , 
		     coord_t * const coord) 
{ 
     int i ;

     /*----------*/

     for( i=0 ; i<BATEAU_TAILLE ; i++)
	  coord_affecter( &(b->corps[i]) , coord[i] );
}

/*
 * Lecture/Ecriture du signe distinctif du bateau 
 */

extern 
case_t
bateau_marque_lire( const bateau_t b ) 
{ 
     return(b.marque) ; 
}
extern 
void 
bateau_marque_ecrire( bateau_t * b , 
		      const case_t m) 
{ 
    b->marque = m ;
}


/*
 * Lecture/Ecriture du pid du processus qui execute le bateau 
 */

extern 
pid_t
bateau_pid_lire( const bateau_t b ) 
{ 
     return(b.pid) ; 
}
extern 
void 
bateau_pid_ecrire( bateau_t * b , 
		   const pid_t pid) 
{ 
    b->pid = pid ;
}

/* 
 * Lecture/Ecriture de l'etat actif 
 */

extern
booleen_t 
bateau_actif_lire( const bateau_t b )
{
     return(b.actif);
}

extern 
void
bateau_actif_ecrire( bateau_t * b,
		     const booleen_t etat )
{
     b->actif = etat ;
}

/*
 *=============================================================
 * Fonctions de gestion d'une liste de bateaux en MC (liste)
 *=============================================================
 */

/*
 * Affichage d'une liste de bateaux 
 */

extern  
void
bateau_liste_afficher( bateau_t * const tab_bateaux,
		       const int nb_bateaux )
{
     int i = 0 ;

     /*--------------------*/

     fprintf( stdout , "\n----- Debut liste bateaux -----\n");

     for(i=0 ; i<nb_bateaux ; i++)
     {
	  bateau_afficher( stdout , tab_bateaux[i] );
	  fprintf( stdout , "\n");
     }
     fprintf( stdout , "----- Fin liste bateaux -----\n\n");
}

/*
 *  Teste si il n'y au plus un bateau dans le terrain
 */ 

extern
booleen_t
bateau_liste_dernier( bateau_t * const tab_bateaux,	/* liste des bateaux */
		      const int nb_bateaux ,	/* nombre de bateaux dans la liste */
		      int * dernier_bateau )	/* indice du dernier bateau */
{
     int i = 0 ;
     int nb_actifs = 0 ;

     /*----------*/

     for( i=0 ; i<nb_bateaux ; i++ )
     {
	  if( tab_bateaux[i].actif )
	  {
	       (*dernier_bateau) = i ;
	       nb_actifs++ ;
	  }
     }
     
     if( nb_actifs == 1 )
	  return(VRAI) ;
     else
     {
	  (*dernier_bateau) = -1 ;
	  return(FAUX) ;
     }
}

/*
 *  Teste si il n'y a plus de bateaux actifs
 *  Renvoie VRAI si il y a au moins un bateaux actif
 *          FAUX sinon
 */ 

extern
booleen_t
bateau_liste_actifs( bateau_t * const tab_bateaux,	/* liste des bateaux */
		     const int nb_bateaux )	/* nombre de bateaux dans la liste */
{
     int i = 0 ;
     booleen_t actifs = FAUX ;

     /*----------*/

     while( (i<nb_bateaux) && (!actifs) )
     {
	  if( tab_bateaux[i].actif )
	       actifs = VRAI ;
	  i++;
     }
     
     return(actifs) ;
}

/* 
 * Transformation d'un numero de pid en numero de bateau 
 */

extern  
int
bateau_liste_pid2ind( const pid_t pid ,
		      bateau_t * const tab_bateaux,
		      const int nb_bateaux )
{
     booleen_t trouve = FAUX ;
     int i = 0 ;

     /*--------------------*/

     while( (i<nb_bateaux) && (!trouve) )
     {
	  if( tab_bateaux[i].pid == pid ) 
	       trouve = VRAI ;
	  else
	       i++ ;
     }

     if( trouve ) 
	  return(i) ;
     else
	  return(-1) ;
}

/* 
 * Transformation d'un numero de bateau en numero de pid
 */

extern  
pid_t
bateau_liste_ind2pid( const int indice ,
		      bateau_t * const tab_bateaux,
		      const int nb_bateaux )
{

     if( (indice<0) || (indice>nb_bateaux) )
     {
	  fprintf( stderr , "bateau_ind2pid : indice de bateau %d en dehors du tableau des bateaux[0..%d]\n",
		   indice , nb_bateaux );
	  return(-1) ;
     }

     return(tab_bateaux[indice].pid) ;
}

/*
 * Recherche d'une coordonnee dans les corps d'une liste de bateaux
 * Si trouve alors retourbe CORRECT et resultat = indice du bteau dans la liste
 * Sinon retourne PAS_TROUVE et resultat = -1 
 */
extern
int
bateau_liste_coord_chercher( const coord_t coord ,		/* Coordonnee recherchee */
			     bateau_t * const tab_bateaux,	/* Liste des bateaux */
			     const int nb_bateaux, 		/* Nb de bateaux dans la liste */
			     int * ind_bateau )			/* Resultat */
{
     booleen_t trouve = FAUX ;
     int i = 0 ;
     int c = 0 ;

     /*--------------------*/

     (*ind_bateau) = -1 ;

     while( (i<nb_bateaux) && (!trouve) )
     {
	  c=0;
	  while( (c<BATEAU_TAILLE) && (!trouve) )
	  {
	       if( ! coord_comparer( tab_bateaux[i].corps[c] , coord ) )
		    trouve = VRAI ;
	       else
		    c++;
	  }
	  if( ! trouve )
	       i++ ;
     }

     if( trouve ) 
     {
	  (*ind_bateau) = i ;
	  return(CORRECT) ;
     }
     else
     {
	  return(PAS_TROUVE) ;
     }
}

/*
 *=============================================================
 * Fonctions de gestion d'une liste de bateaux en MS (fichier)
 *=============================================================
 */

/* 
 * Ajout d'un bateau dans un fichier de bateaux 
 */

extern 
int
bateau_fichier_ajouter( char * const fichier_bateaux,	/* Reference fichier bateaux */
			const bateau_t bateau )	/* Bateau a ajouter */
{
     int fd_bateaux;
     char mess[MESS_LONGUEUR] ;
     ssize_t  nb_ecrits ;
     int nb_bateaux ;
     struct flock verrou ;

     /*----------*/

     /* 
      * Ouverture du fichier de sauvegarde des bateaux
      */

     if( ( fd_bateaux = open( fichier_bateaux , O_RDWR , 0666)) == -1 )
     {
	  sprintf( mess, "Pb open sur %s\n", fichier_bateaux);
	  perror(mess);
	  return(ERREUR);
     }

     /* Pose d'un verrou bloquant en ecriture sur tout le fichier */
     verrou.l_type   = F_WRLCK ;
     verrou.l_whence = SEEK_SET ;
     verrou.l_start  = 0 ;
     verrou.l_len    = 0 ;
     verrou.l_pid    = getpid() ; 

     if( fcntl( fd_bateaux , F_SETLKW , &verrou ) == -1 )
     {  
	  sprintf( mess , "bateau_fichier_ajouter : Pb sur pose verrou fichier %s ",
		   fichier_bateaux ) ; 
	  perror(mess) ;
	  return(ERREUR) ;
     }

     /* 
      * Mise a jour du nombre de bateaux 
      */
     
     if( read( fd_bateaux , &nb_bateaux , sizeof(int)) == -1 )
     {
	  perror( "bateau_fichier_ajouter : Pb read sur lecture nombre de bateaux\n");
	  return(ERREUR) ;
     }

#ifdef _DEBUG_ 
     printf("bateau_fichier_ajouter --> nombre de bateaux dans %s avant ajout = %d\n", 
	    fichier_bateaux , nb_bateaux );
#endif

     nb_bateaux++ ;

     if( lseek( fd_bateaux , 0 ,SEEK_SET) == -1 )
     {
	  perror( "bateau_fichier_ajouter : Pb lseek sur deplacement au debut fichier bateaux\n");
	  return(ERREUR) ;
     }

     if( write( fd_bateaux , &nb_bateaux , sizeof(int)) == -1 )
     {
	  perror( "bateau_fichier_ajouter : Pb write sur ecriture nombre de bateaux\n");
	  return(ERREUR) ;
     }

     /* 
      * Ajout d'un bateau en fin de fichier 
      */

     if( lseek( fd_bateaux , 0 , SEEK_END ) == -1 ) 
     { 
  	  perror( "bateau_fichier_ajouter : Pb lseek sur deplacement en fin de fichier bateaux\n"); 
  	  return(ERREUR) ; 
     } 


     if( (nb_ecrits = write( fd_bateaux , &bateau , sizeof(bateau_t)) != sizeof(bateau_t)) )
     {
#ifdef _LINUX_
	  sprintf( mess, "bateau_fichier_ajouter : Pb dans la lecture d'un bateau, %u octets ecrits au lieu de %u\n",
		   nb_ecrits , sizeof(bateau_t) );
#else
	  sprintf( mess, "bateau_fichier_ajouter : Pb dans la lecture d'un bateau, %lu octets ecrits au lieu de %lu\n",
		   nb_ecrits , sizeof(bateau_t) );
#endif

	  perror(mess) ;
	  fprintf( stderr, " Structure de bateau a ecrire = ");
	  bateau_afficher( stderr , bateau );
	  fprintf( stderr , "\n" );
	  return(ERREUR);
     }

     /*
      * Fermeture fichier des bateaux 
      */
     
     /* Levee du verrou sur tout le fichier */
     verrou.l_type   = F_UNLCK ;
     
     if( fcntl( fd_bateaux , F_SETLKW , &verrou ) == -1 )
     {  
	  sprintf( mess , "bateau_fichier_ajouter : Pb sur levee verrou du fichier %s ",
		   fichier_bateaux ) ; 
	  perror(mess) ;
	  return(ERREUR) ;
     }

     close(fd_bateaux) ;
       
#ifdef _DEBUG_ 
     printf(  "bateau_fichier_ajouter --> Ajout effectue dans la liste %s du bateau\n ",
	      fichier_bateaux );
     bateau_afficher( stdout , bateau );
#endif
     
     return(CORRECT) ;
}

/* 
 * Ecriture du pid du bateau amiral dans le fichier des bateaux
 */

extern 
int
bateau_fichier_amiral_ecrire( char * const fichier_bateaux,	/* Reference fichier bateaux */
			      const pid_t pid_amiral )		/* Pid a ecrire */
{
     int fd_bateaux;
     char mess[MESS_LONGUEUR] ;
     struct flock verrou ;

     /*----------*/

     /* 
      * Ouverture fichier de sauvegarde des bateaux
      */

     if( ( fd_bateaux = open( fichier_bateaux , O_RDWR , 0666)) == -1 )
     {
	  sprintf( mess, "Pb open sur %s\n", fichier_bateaux);
	  perror(mess);
	  return(ERREUR);
     }

     /* Pose d'un verrou bloquant en ecriture sur la zone du pid amiral */
     verrou.l_type   = F_WRLCK ;
     verrou.l_whence = SEEK_SET ;
     verrou.l_start  = sizeof(int);
     verrou.l_len    = sizeof(int);
     verrou.l_pid    = getpid() ; 

     if( fcntl( fd_bateaux , F_SETLKW , &verrou ) == -1 )
     {  
	  sprintf( mess , "bateau_fichier_amiral_ecrire : Pb sur pose verrou fichier %s ",
		   fichier_bateaux ) ; 
	  perror(mess) ;
	  return(ERREUR) ;
     }

     /* 
      * Saut du nombre de bateaux 
      */

     if( lseek( fd_bateaux , sizeof(int)  ,SEEK_SET) == -1 )
     {
	  perror( "bateau_fichier_amiral_ecrire : Pb lseek sur saut du nb de bateaux\n");
	  return(ERREUR) ;
     }

     /* 
      * Ecriture du pid du processus bateau amiral
      */

     if( write( fd_bateaux , &pid_amiral , sizeof(pid_t)) == -1 )
     {
	  perror( "bateau_fichier_amiral_ecrire : Pb write sur ecriture pid bateau amiral\n");
	  return(ERREUR) ;
     }

     /*
      * Fermeture fichier des bateaux 
      */

     /* Levee du verrou sur le pid bateau amiral */
     verrou.l_type   = F_UNLCK ;
     
     if( fcntl( fd_bateaux , F_SETLKW , &verrou ) == -1 )
     {  
	  sprintf( mess , "bateau_fichier_amiral_ecrire : Pb sur levee verrou du fichier %s ",
		   fichier_bateaux ) ; 
	  perror(mess) ;
	  return(ERREUR) ;
     }
     close(fd_bateaux) ;

#ifdef _DEBUG_ 
#ifdef _LINUX_
     printf(  "bateau_fichier_amiral_ecrire --> Ecriture du pid bateau amiral %d effectuee dans le fichier des bateaux %s\n",
	      pid_amiral , fichier_bateaux );
#else
     printf(  "bateau_fichier_amiral_ecrire --> Ecriture du pid bateau amiral %ld effectuee dans le fichier des bateaux %s\n",
	      pid_amiral , fichier_bateaux );
#endif
#endif

     return(CORRECT) ;
}


/* 
 * Lecture du pid du bateau amiral dans le fichier des bateaux
 */

extern 
int
bateau_fichier_amiral_lire( char * const fichier_bateaux,	/* Reference fichier bateaux */
			    pid_t * pid_amiral )	/* Pid a lire */
{
     int fd_bateaux;
     char mess[MESS_LONGUEUR] ;
     struct flock verrou ;

     /*----------*/

     /* 
      * Initialisation du resultat 
      */
     (*pid_amiral) = 0 ;

     /* 
      * Ouverture fichier de sauvegarde des bateaux
      */

     if( ( fd_bateaux = open( fichier_bateaux , O_RDONLY , 0666)) == -1 )
     {
	  sprintf( mess, "Pb open sur %s\n", fichier_bateaux);
	  perror(mess);
	  return(ERREUR);
     }

     /* Pose d'un verrou bloquant en lecture sur la zone du pid mer */
     verrou.l_type   = F_RDLCK ;
     verrou.l_whence = SEEK_SET ;
     verrou.l_start  = sizeof(int);
     verrou.l_len    = sizeof(int);
     verrou.l_pid    = getpid() ; 

     if( fcntl( fd_bateaux , F_SETLKW , &verrou ) == -1 )
     {  
	  sprintf( mess , "bateau_fichier_amiral_lire : Pb sur pose verrou fichier %s ",
		   fichier_bateaux ) ; 
	  perror(mess) ;
	  return(ERREUR) ;
     }

     
     /* 
      * Saut du nombre de bateaux 
      */

     if( lseek( fd_bateaux , sizeof(int) ,SEEK_SET) == -1 ) 
     { 
	  perror( "bateau_fichier_ajouter : Pb lseek sur saut du nb de bateaux\n"); 
	  return(ERREUR) ; 
     } 

     /* 
      * Lecture du pid du processus mer
      */

     if( read( fd_bateaux , pid_amiral , sizeof(pid_t)) == -1  )
     {
	  perror( "bateau_amiral_lire : Pb read sur lecture pid mer\n");
	  return(ERREUR) ;
     }

     /*
      * Fermeture fichier des bateaux 
      */

     /* Levee du verrou sur le pid mer */
     verrou.l_type   = F_UNLCK ;
     
     if( fcntl( fd_bateaux , F_SETLKW , &verrou ) == -1 )
     {  
	  sprintf( mess , "bateau_fichier_amiral_lire : Pb sur levee verrou du fichier %s ",
		   fichier_bateaux ) ; 
	  perror(mess) ;
	  return(ERREUR) ;
     }

     close(fd_bateaux) ;

#ifdef _DEBUG_ 
#ifdef _LINUX_
     printf(  "bateau_fichier_amiral_lire --> Lecture du pid mer %d effectuee dans la liste des bateaux %s\n",
	      (*pid_amiral) , fichier_bateaux );
#else
     printf(  "bateau_fichier_amiral_lire --> Lecture du pid mer %ld effectuee dans la liste des bateaux %s\n",
	      (*pid_amiral) , fichier_bateaux );
#endif
#endif

     return(CORRECT) ;
}


/* 
 * Affichage d'un fichier de bateaux                      
 */

extern
int
bateau_fichier_afficher( char * const fichier_bateaux )	
{
     int fd_bateaux;
     int nb_bateaux ;
     pid_t pid_amiral ;

     bateau_t bateau ;
     char mess[MESS_LONGUEUR] ;
     ssize_t  nb_lus ;
     int v ;

     struct flock verrou ;

     /*----------*/

     /* 
      * Ouverture fichier de sauvegarde des bateaux
      */

     if( ( fd_bateaux = open( fichier_bateaux , O_RDONLY, 0666)) == -1 )
     {
	  sprintf( mess, "Pb open sur %s\n", fichier_bateaux);
	  perror(mess);
	  return(ERREUR);
     }

     /* Pose d'un verrou bloquant en lecture sur tout le fichier */
     verrou.l_type   = F_RDLCK ;
     verrou.l_whence = SEEK_SET ;
     verrou.l_start  = 0;
     verrou.l_len    = 0;
     verrou.l_pid    = getpid() ; 

     if( fcntl( fd_bateaux , F_SETLKW , &verrou ) == -1 )
     {  
	  sprintf( mess , "bateau_fichier_afficher: Pb sur pose verrou fichier %s ",
		   fichier_bateaux ) ; 
	  perror(mess) ;
	  return(ERREUR) ;
     }


     /* 
      * Lecture du nombre de bateaux 
      */
     
     if( read( fd_bateaux , &nb_bateaux , sizeof(int)) == -1 )
     {
	  perror( "bateau_fichier_afficher : Pb read sur lecture nombre de bateaux\n");
	  return(ERREUR) ;
     }

     /*
      * Lecture du pid du processus mer 
      */
     
     if( read( fd_bateaux , &pid_amiral , sizeof(pid_t)) == -1 )
     {
	  perror( "bateau_fichier_afficher : Pb read sur lecture nombre de bateaux\n");
	  return(ERREUR) ;
     }
#ifdef _LINUX_
    printf( "NB BATEAUX = %d , PID MER = %d\n",
	     nb_bateaux , pid_amiral );
#else
     printf( "NB BATEAUX = %d , PID MER = %ld\n",
	     nb_bateaux , pid_amiral );
#endif
     /* 
      * Affichage corps de la liste 
      */

     for( v=0 ; v < nb_bateaux ; v++ )
     {
	  /* Lecture d'un bateau */
	  if( (nb_lus = read(fd_bateaux , &bateau  , sizeof(bateau_t))) != sizeof(bateau_t) )
	  {
#ifdef _LINUX_
	       sprintf( mess, "bateau_fichier_afficher : Pb dans la lecture d'un bateau, %u octets lus au lieu de %u\n",
			nb_lus , sizeof(bateau_t) );
#else
	       sprintf( mess, "bateau_fichier_afficher : Pb dans la lecture d'un bateau, %lu octets lus au lieu de %lu\n",
			nb_lus , sizeof(bateau_t) );
#endif
	       perror(mess) ;
	       fprintf( stderr, " Structure de bateau lue = ");
	       bateau_afficher( stderr , bateau );
	       fprintf( stderr , "\n" );
	       return(ERREUR);
	  }

	  bateau_afficher( stdout , bateau );
	  fprintf( stdout , "\n" );
     }
	  
     /*
      * Fermeture fichier des bateaux 
      */

     /* Levee du verrou sur tout le fichier */
     verrou.l_type   = F_UNLCK ;
     
     if( fcntl( fd_bateaux , F_SETLKW , &verrou ) == -1 )
     {  
	  sprintf( mess , "bateau_fichier_afficher : Pb sur levee verrou du fichier %s ",
		   fichier_bateaux ) ; 
	  perror(mess) ;
	  return(ERREUR) ;
     }

     close(fd_bateaux) ;

     return(CORRECT) ;
}


/*
 * Initialisation d'un fichier de bateaux 
 */

extern 
int
bateau_fichier_initialiser( char * const fich_bateaux )	/* Reference du fichier des bateaux */
{
     int fd_bateaux ;
     char mess[MESS_LONGUEUR] ;
     int nb_bateaux = 0 ;
     pid_t pid_amiral = 0 ;

     /*--------------------*/

     /* 
      * Ouverture fichier de sauvegarde des bateaux
      */

     if( ( fd_bateaux = open( fich_bateaux , O_WRONLY | O_CREAT | O_TRUNC , 0666)) == -1 )
     {
	  sprintf( mess, "bateau_fichier_initialiser : Pb open sur %s", fich_bateaux);
	  perror(mess);
	  return(ERREUR);
     }

     /* 
      * Ecriture de la ligne d'entete
      */

     if( write( fd_bateaux , &nb_bateaux , sizeof(int)) == -1 )
     {
	  perror( "Pb write sur ecriture nb_bateaux");
	  exit(-1);
     }

     if( write( fd_bateaux , &pid_amiral , sizeof(pid_t)) == -1 )
     {
	  perror( "Pb write sur ecriture pid amiral");
	  exit(-1) ;
     }

     /*
      * Fermeture fichier des bateaux 
      */

     close(fd_bateaux) ;
     
     return(CORRECT);

}



/*
 *===================================================================
 * Fonctions de conversion entre une liste de bateaux en MC et en MS
 *===================================================================
 */


/* 
 * Chargement d'une liste de bateaux a partir d'un fichier de sauvegarde
 * Format du fichier : 
 * <nombre de bateaux> <pid bateau amiral>
 * .....
 * <marque> <pid>                      
 * ......
 */

extern
int
bateau_liste_charger( char * const fichier_bateaux,	/* Fichier de sauvegarde des bateaux */
		      bateau_t ** liste_bateaux,	/* liste des bateaux */
		      int * nb_bateaux )	/* Nombre de bateaux dans la liste */
{
     static booleen_t first = VRAI ;
     int fd_bateaux;
     bateau_t bateau ;
     char mess[MESS_LONGUEUR] ;
     ssize_t  nb_lus ;
     int v ;

     /*----------*/

     /* 
      * Ouverture fichier de sauvegarde des bateaux
      */

     if( ( fd_bateaux = open( fichier_bateaux , O_RDONLY, 0666)) == -1 )
     {
	  sprintf( mess, "Pb open sur %s\n", fichier_bateaux);
	  perror(mess);
	  return(ERREUR);
     }

     /* 
      * Lecture du nombre de bateaux 
      */
     
     if( read( fd_bateaux , nb_bateaux , sizeof(int)) == -1 )
     {
	  perror( "bateau_liste_charger : Pb read sur lecture nombre de bateaux\n");
	  return(ERREUR) ;
     }

     /*
      * Saut du pid du processus bateau_amiral 
      */
     
     if( lseek( fd_bateaux , sizeof(pid_t) , SEEK_CUR) == -1 )
     {
	  perror( "bateau_liste_charger : Pb lseek sur saut du pid mer\n");
	  return(ERREUR) ;
     }
     
     /* 
      * Creation liste des bateaux
      */

     for( v=0 ; v < (*nb_bateaux) ; v++ )
     {
	  /* Lecture d'un bateau */
	  if( (nb_lus = read(fd_bateaux , &bateau , sizeof(bateau_t))) != sizeof(bateau_t) )
	  {
#ifdef _LINUX_
	       sprintf( mess, "bateau_liste_charger : Pb dans la lecture d'un bateau, %u octets lus au lieu de %u\n",
			nb_lus , sizeof(bateau_t) );
#else
	       sprintf( mess, "bateau_liste_charger : Pb dans la lecture d'un bateau, %lu octets lus au lieu de %lu\n",
			nb_lus , sizeof(bateau_t) );
#endif

	       perror(mess) ;
	       fprintf( stderr, " Structure de bateau lue = ");
	       bateau_afficher( stderr , bateau );
	       fprintf( stderr , "\n" );
	       return(ERREUR);
	  }

	  /* Chargement du bateau dans la liste */
	  if( first )
	  {
	       (*liste_bateaux)= (bateau_t *)malloc(sizeof(bateau_t));
	       first = FAUX ;
	  }
	  else
	  {
	       (*liste_bateaux) = (bateau_t *)realloc((*liste_bateaux),sizeof(bateau_t)*((*nb_bateaux)+1));
	  }

	  (*liste_bateaux)[v] = bateau ;
     }
	  

     /*
      * Fermeture fichier des bateaux 
      */
     close(fd_bateaux) ;

#ifdef _DEBUG_ 
     printf("bateau_liste_charger --> Chargement des %d bateaux du fichier %s dans la liste d'@ %0lx effectue\n",
	    (*nb_bateaux), fichier_bateaux , (unsigned long)(*liste_bateaux) );
#endif

     return(CORRECT) ;
}


/* 
 * Sauvegarde d'une liste de bateaux dans un fichier de sauvegarde
 * Format du fichier : 
 * <nombre de bateaux> <pid bateau amiral>
 * .....
 * <marque> <pid>                      
 * ......
 */

extern
int
bateau_liste_sauver( char * const fichier_bateaux,	/* Fichier de sauvegarde des bateaux */
		     bateau_t * const liste_bateaux,	/* liste des bateaux */
		     const int nb_bateaux,	/* Nombre de bateaux dans la liste */
		     const pid_t pid_amiral ) /* Pid du btaeau amiral */
{
     int fd_bateaux;
     bateau_t bateau ;
     char mess[MESS_LONGUEUR] ;
     ssize_t  nb_ecrits ;
     int v ;

     int nbbateaux = nb_bateaux ;
     pid_t pidamiral = pid_amiral ;

     /*----------*/

     /* 
      * Ouverture fichier de sauvegarde des bateaux
      */

     if( ( fd_bateaux = open( fichier_bateaux , O_CREAT | O_WRONLY, 0666)) == -1 )
     {
	  sprintf( mess, "Pb open sur %s\n", fichier_bateaux);
	  perror(mess);
	  return(ERREUR);
     }

     /* 
      * Ecriture du nombre de bateaux 
      */
     
     if( write( fd_bateaux , &nbbateaux , sizeof(int)) == -1 )
     {
	  perror( "bateau_liste_sauver : Pb write sur ecriture nombre de bateaux\n");
	  return(ERREUR) ;
     }

     /*
      * Ecriture du pid du processus bateau_amiral 
      */
     
     if( write( fd_bateaux , &pidamiral , sizeof(pid_t) ) == -1 )
     {
	  perror( "bateau_liste_sauver : Pb write sur ecriture du pid amiral\n");
	  return(ERREUR) ;
     }
     
     /* 
      * Ecriture de la liste des bateaux
      */

     for( v=0 ; v < nb_bateaux ; v++ )
     {
	  /* Lecture d'un bateau */
	  bateau = liste_bateaux[v];
	  if( (nb_ecrits = write(fd_bateaux , &bateau , sizeof(bateau_t))) != sizeof(bateau_t) )
	  {
#ifdef _LINUX_
	       sprintf( mess, "bateau_liste_sauver : Pb dans l ecriture d'un bateau, %u octets ecrits au lieu de %u\n",
			nb_ecrits , sizeof(bateau_t) );
#else
	       sprintf( mess, "bateau_liste_sauver : Pb dans l ecriture d'un bateau, %lu octets ecrits au lieu de %lu\n",
			nb_ecrits , sizeof(bateau_t) );
#endif

	       perror(mess) ;
	       fprintf( stderr, " Structure de bateau ecrit = ");
	       bateau_afficher( stderr , bateau );
	       fprintf( stderr , "\n" );
	       return(ERREUR);
	  }
     }

     /*
      * Fermeture fichier des bateaux 
      */
     close(fd_bateaux) ;

#ifdef _DEBUG_ 
     printf("bateau_liste_sauver --> Sauvegarde la liste d'@ %0lx des %d bateaux dans le fichier %s  effectuee\n",
	    (unsigned long)liste_bateaux , nb_bateaux, fichier_bateaux );
#endif

     return(CORRECT) ;
}

/* 
 * Creation d'une liste de bateaux 
 */

extern
int 
bateau_liste_creer( bateau_t ** liste )
{
     (*liste) = (bateau_t *)NULL ;
     return(CORRECT);
}

/*
 * Destruction d'une liste de bateaux
 */
extern 
int
bateau_liste_detruire( bateau_t ** liste )
{
     free( (*liste) );
     (*liste) = (bateau_t *)NULL ;
     return(CORRECT) ;
}
