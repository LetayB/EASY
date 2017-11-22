#include <com.h>
 
static
err_t com_fd_ligne_charger( char *** commande , /* tableau commande a remplir */
			     FILE * fd ) 	      /* Descripteur fichier */
{
  err_t cr = OK ; 
  char msgerr[MAX_STRING] ; 
  char ligne[MAX_STRING] ; 
  char * p_ligne = NULL; 
  char * p = NULL ; 
  int i = 0 ; 
 
  /* Initialisation du resultat */
  (*commande) = NULL ; 
 
  /* Lecture d'une commande  */
  if( fgets( ligne , MAX_STRING , fd ) == NULL ) 
    {
      if( feof(fd) ) 
	return(ERR_LIGNE_VIDE) ;
 
 
      sprintf( msgerr , "com_fichier_ligne_charger: pb lecture ligne sur %d caracteres\n" , MAX_STRING ) ;
      perror(msgerr) ;
      return(ERR_FICHIER) ; 
    }
 
 
  /* Suppression \n a la fin de la ligne */
  if((p = strrchr(ligne, '\n')) != NULL)
    *p = '\0';
 
  /* Test ligne vide */
  if( strlen(ligne) == 0 ) 
    return(ERR_LIGNE_VIDE) ; 
 
  /* Compte le nombre d'elements dans la commande */
  p_ligne = strdup( ligne ) ;
  for( p = strtok(p_ligne, " ") , i=0 ; p != NULL ; p = strtok(NULL, " ") , i++ ) ;
 
  /* 
   * Allocation memoire tableau stockage de la commande 
   */
  if( ( (*commande) = malloc( sizeof(char *) * (i+1) ) ) == NULL )
    {
      sprintf( msgerr , "com_fichier_ligne_charger: pb allocation memoire pour tableau commande (%lu octets demandes)\n" , 
	       (unsigned long)(sizeof(char *)*i) ) ;
      perror(msgerr) ;
      return(ERR_MEMOIRE) ; 
    } 
 
  /* Stockage dans un tableau */
  free(p_ligne) ; 
  p_ligne = strdup( ligne ); 
  p = strtok(p_ligne, " ");
  i=0 ; 
  while(p != NULL)
   {
     /* Reservation memoire 1 argument de la commande */
     (*commande)[i] = malloc(sizeof(char) * (1+strlen(p)));
     strcpy((*commande)[i], p);
 
     /* Argument suivant */
     p = strtok(NULL, " ");
     i++;
   }  
 
  /* Fin du tableau par NULL */
  (*commande)[i] = NULL ;
 
  free(p_ligne) ; 
 
 return(cr) ;
}
 
 
extern
err_t com_fichier_charger( char **** commandes , /* tableau commandes a remplir */
			    int * nb_commandes , 
			    char * fichier_liste_com )  /* Nom du fichier */
 
{
  err_t cr = OK ; 
  char msgerr[MAX_STRING] ; 
  FILE * fd_liste_com = NULL ; 
  int cpt_com = 0 ; 
 
  /* Initialisations resultats */
  (*commandes) = NULL ;
  (*nb_commandes) = 0 ; 
 
  /* Ouverture liste des commandes */
  if( ( fd_liste_com = fopen( fichier_liste_com , "r" ) ) == NULL )
    {
      sprintf( msgerr , "Pb ouverture fichier %s\n" , fichier_liste_com ) ; 
      perror( msgerr ) ; 
      return(ERR_FICHIER) ; 
    }
 
 while( ! feof( fd_liste_com ) )
   { 
     if(cpt_com == 0 )
       {
	 /* 1ere commande */
	 (*commandes) = malloc( sizeof( char **) ) ;
       }
     else
       {
	 /* Commandes suivantes */
	 (*commandes) = realloc( (*commandes) , sizeof( char **) * (cpt_com+1) ) ;
       }
 
     cr = com_fd_ligne_charger( *(commandes)+cpt_com, fd_liste_com ) ;
 
     switch( cr ) 
       {
       case OK : 
	 cpt_com++ ;
	 break ;
       case ERR_LIGNE_VIDE : 
	 cr = OK ;
	 break ; 
       default :
	 return(cr) ; 
       }
   }
 
 fclose(fd_liste_com) ;
 (*nb_commandes) = cpt_com ; 
 
 return(cr) ;
}
 
extern
void com_afficher(char ** commande ) /* tableau des arguments d'une commande se finissant par NULL */
{
  int i = 0 ; 
  while( commande[i] != NULL )
    {
      printf( "%s " , commande[i] ) ; 
      i++ ; 
    }
  fflush(stdout) ; 
}
 
extern
void com_liste_afficher(char *** commandes , /* tableau des commandes */
			 const int nb_commandes ) /* Nb de commandes */
{
  int i = 0 ; 
 
  for( i=0 ; i<nb_commandes ; i++ ) 
    {
      printf( "Commande %d: " , i ) ;
      com_afficher( commandes[i] ) ; 
      printf("\n") ;
    }
}
 