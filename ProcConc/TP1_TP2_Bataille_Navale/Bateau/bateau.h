#ifndef _BATEAU_H_
#define _BATEAU_H_

#include <unistd.h>

#include <commun.h>
#include <coord.h>

/*
 *=============================================================
 * Constantes du TDA bateau
 *=============================================================
 */

#define BATEAU_NB_MAX  100
#define BATEAU_TAILLE  4
#define BATEAU_SEUIL_BOUCLIER 400 //ancienne valeur : 10
#define BATEAU_MAX_ENERGIE 500

/*
 *=============================================================
 * Realisation du TDA bateau 
 *=============================================================
 */

typedef struct bateau_s
{
     coord_t   corps[BATEAU_TAILLE] ;
     case_t    marque ;
     pid_t     pid ;
     booleen_t actif ;
} bateau_t ;


/*
 *=============================================================
 * Primitives du TDA bateau 
 *=============================================================
 */

/*
 * Fonctions get/set (lecture/ecriture) des attributs d'un bateau
 *================================================================
 */

/*
 * Lecture/Ecriture du corps du bateau 
 */

extern coord_t * bateau_corps_lire( bateau_t * const )  ;
extern void bateau_corps_ecrire( bateau_t * , coord_t * const ) ;

/*
 * Lecture/Ecriture du signe distinctif du bateau 
 */

extern case_t bateau_marque_lire( const bateau_t )  ;
extern void bateau_marque_ecrire( bateau_t * , const case_t ) ;

/*
 * Lecture/Ecriture du pid du processus qui execute le bateau 
 */

extern pid_t bateau_pid_lire( const bateau_t ) ;
extern void bateau_pid_ecrire( bateau_t * , const pid_t ) ;

/* 
 * Lecture/Ecriture de l'etat actif 
 */

extern booleen_t bateau_actif_lire( const bateau_t );
extern void  bateau_actif_ecrire( bateau_t * , const booleen_t );

/*
 * Fonctions de gestion d'un bateau 
 *=============================================================
 */

/* 
 * Ecriture des caracteristiques d'un bateau 
 */
extern int bateau_ecrire( bateau_t *,		/* Bateau */
			  coord_t * const,	/* Coordonnees du corps */
			  const case_t,		/* Marque */
			  const pid_t ,		/* Pid */
			  const booleen_t);	/* Actif ? */

/* 
 * Affichage d'un bateau
 */

extern void bateau_afficher( FILE * const,	/* Canal d'affichage */
			     const bateau_t );	/* Bateau a afficher */

/*
 * Fonctions de gestion d'une liste de bateaux en MC (liste)
 *=============================================================
 */

/* 
 * Creation d'une liste de bateaux 
 */

extern
int 
bateau_liste_creer( bateau_t ** );

/*
 * Destruction d'une liste de bateaux
 */
extern 
int
bateau_liste_detruire( bateau_t ** );

/*
 * Affichage de la liste des bateaux 
 */

extern  
void
bateau_liste_afficher( bateau_t * const,	/* liste des bateaux */
		       const int) ;	/* nombre de bateaux dans la liste */ 

/*
 *  Teste si il y a au plus un bateau dans le terrain
 */ 

extern
booleen_t
bateau_liste_dernier( bateau_t * const,	/* liste des bateaux */
		      const int,		/* nombre de bateaux dans la liste */ 
		      int * );		/* indice du dernier bateau */

/*
 *  Teste si il n'y a plus de bateaux actifs
 *  Renvoie VRAI si il y a au moins un bateaux actif
 *          FAUX sinon
 */ 

extern
booleen_t
bateau_liste_actifs( bateau_t * const,	/* liste des bateaux */
		     const int );	/* nombre de bateaux dans la liste */



/* 
 * Transformation d'un numero de pid en numero de bateau 
 */

extern  
int
bateau_liste_pid2ind( const pid_t,		/* Pid */
		      bateau_t * const,		/* Liste des bateaux */
		      const int ) ;		/* Nombre de bateaux dans la liste */

/* 
 * Transformation d'un numero de bateau en numero de pid
 */

extern  
pid_t
bateau_liste_ind2pid( const int,		/* Indice du bateau dans la liste */
		      bateau_t * const,	/* Liste des bateaux */
		      const int );		/* Nombre de bateraux dans la liste */


/*
 * Recherche d'une coordonnee dans les corps d'une liste de bateaux
 * Si trouve alors retourbe CORRECT et resultat = indice du bteau dans la liste
 * Sinon retourne PAS_TROUVE et resultat = -1 
 */
extern
int
bateau_liste_coord_chercher( const coord_t,	/* Coordonnee recherchee */
			     bateau_t * const,	/* Liste des bateaux */
			     const int, 	/* Nb de bateaux dans la liste */
			     int * );		/* Resultat */
/*
 * Fonctions de gestion d'une liste de bateaux en MS (fichier)
 *=============================================================
 */

/* 
 * Ajout d'un bateau dans un fichier de bateaux 
 */

extern 
int
bateau_fichier_ajouter( char * const,	/* Reference fichier bateaux */
			const bateau_t );	/* Bateau a ajouter */


/* 
 * Ecriture du pid de la mer dans le fichier des bateaux
 */

extern 
int
bateau_fichier_amiral_ecrire( char * const,	/* Reference fichier bateaux */
			      const pid_t);	/* Pid a ecrire */

/* 
 * Lecture du pid de la mer dans le fichier des bateaux
 */

extern 
int
bateau_fichier_amiral_lire( char * const,	/* Reference fichier bateaux */
			    pid_t * );	/* Pid a lire */

/* 
 * Affichage d'un fichier de bateaux                      
 */

extern
int
bateau_fichier_afficher( char * const )	; /* Reference fichier bateaux */


/*
 * Initialisation d'un fichier de bateaux 
 */

extern 
int
bateau_fichier_initialiser( char * const ) ;	/* Reference du fichier des bateaux */ 


/*
 *===================================================================
 * Fonctions de conversion entre une liste de bateaux en MC et en MS
 *===================================================================
 */

/* 
 * Chargement d'une liste de bateaux a partir d'un fichier de sauvegarde
 * Format du fichier : 
 * <nombre N de bateaux> 
 * <structure bateau 1>
 * <structure bateau 2>
 * ........
 * <structure bateau N>
 *
 */

extern
int
bateau_liste_charger( char * const,	/* Reference fichier de sauvegarde des bateaux */
		      bateau_t **,	/* Liste des bateaux */
		      int * );		/* Nombre de bateaux dans la liste */


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
bateau_liste_sauver( char * const,	/* Fichier de sauvegarde des bateaux */
		     bateau_t * const,	/* liste des bateaux */
		     const int,		/* Nombre de bateaux dans la liste */
		     const pid_t);	/* Pid du btaeau amiral */

#endif


