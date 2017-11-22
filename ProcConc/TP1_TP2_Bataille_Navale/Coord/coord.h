#ifndef _COORD_H_
#define _COORD_H_

#include <commun.h> 

/* 
 *===================================================
 * Realisation du TDA coord (coordonnees d'une case) 
 *=================================================== 
 */

typedef struct coord_s
{
     int l ;		/* numero ligne   */
     int c ;		/* numero colonne */
     off_t pos ;	/* position dans le fichier */
} coord_t ;

/* Sous-type direction */

typedef enum { NORD , EST , SUD , OUEST } direction_t ;

/* Sous-type orientation */

typedef enum { VERTICAL , HORIZONTAL , OBLIQUE } orientation_t ;

/*
 * Fonctions get/set des attributs d'une coordonnee
 *=================================================
 */

extern int coord_ligne_lire( const coord_t c );
extern void coord_ligne_ecrire( coord_t * c , const int ligne );

extern int coord_colonne_lire( const coord_t c );
extern void coord_colonne_ecrire( coord_t * c , const int colonne );

extern off_t coord_position_lire( const coord_t c );
extern void coord_position_ecrire( coord_t * c , const off_t position );

/*
 *===========================================
 * Realisations des primitives des sous-types
 *===========================================
 */

extern char * coord_ori2string( const orientation_t );

extern char * coord_dir2string( const direction_t ) ;

/* 
 *===================================================
 * Primitives du TDA coord 
 *=================================================== 
 */

/* 
 * Affichage des coordonnees d'une case 
 */

extern void coord_afficher( FILE * const,    /* Canal d'affichage */ 
			    const coord_t ); /* Case a afficher */

/* 
 * Ecriture des coordonnees d'une case 
 */

extern void coord_ecrire( coord_t * ,		/* Case a affecter */ 
			  const int,		/* # ligne */
			  const int,		/* # colonne */
			  const off_t );	/* Position dans fichier */

/* 
 * Lecture des coordonnees d'une case 
 */

extern void coord_lire( const coord_t,		/* Case a lire          */ 
			int * ligne,		/* # ligne a affecter   */              
			int * colonne,		/* # colonne a affecter */            
			off_t * position );	/* Position a affecter  */


/* 
 * Comparaison de 2 coordonnees 
 * retour < 0 si c1 < c2 
 *        = 0 si c1 = c2 
 *        > 0 si c1 > c2 
 */

extern int coord_comparer( const coord_t,	/* coord c1 */
			   const coord_t) ;	/* coord c2 */

/* 
 * Affectation des coordonnees d'une case 
 */

extern void coord_affecter( coord_t * ,		/* coord cible */
			    const coord_t  ) ;	/* coord source */


/* 
 * Creation d'une liste de cases
 */

extern void coord_liste_creer( coord_t **  ) ; /* liste de coord. */

/* 
 * Destruction d'une liste de cases
 */

extern void  coord_liste_detruire( coord_t ** liste ) ;

/* 
 * Ajout d'un element dans une liste de cases
 */

extern void coord_liste_ajouter( coord_t ** liste ,	/* liste des cases */
				 int * nb_elems,	/* nb de cases avant/apres */
				 const coord_t elem );	/* element a ajouter */


/* 
 * Recherche d'un element dans une liste de cases
 * selon les criteres donnes. 
 * Si criteres.pos == -1 alors recherche sur les [y,x]
 * Sinon recherche sur la position
 * renvoie VRAI si l'element est trouve 
 *         FAUX si l'element n'est pas dans la liste
 */

extern booleen_t coord_liste_rechercher( coord_t * const,	/* liste des cases */
					 const int,		/* nb de cases */
					 const coord_t,		/* criteres de recherche */
					 int *  );		/* indice de l'element trouve */

/* 
 * Affichage d'une liste de cases
 */

extern void coord_liste_afficher( FILE * const,		/* canal d'affichage */
				  coord_t * const,	/* liste des cases */
				  const int  );		/* nb de cases */
/* 
 * Determination de l'orientation de 2 coordonnees
 */

extern orientation_t coord_orientation_donner( const coord_t,  /* 1ere coord  */
					       const coord_t); /* 2ieme coord */

#endif
