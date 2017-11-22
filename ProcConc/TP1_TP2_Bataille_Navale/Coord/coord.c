#include <stdio.h>
#include <stdlib.h>

#include <coord.h>

/*
 * Fonctions get/set des attributs d'une coordonnee
 *=================================================
 */

extern 
int coord_ligne_lire( const coord_t c )
{
  return(c.l) ;
}
extern 
void coord_ligne_ecrire( coord_t * c , const int ligne )
{
  c->l = ligne ;
}

extern 
int coord_colonne_lire( const coord_t c )
{
  return(c.c) ;
}
extern 
void coord_colonne_ecrire( coord_t * c , const int colonne )
{
  c->c = colonne ;
}
extern 
off_t coord_position_lire( const coord_t c )
{
  return(c.pos) ;
}
extern 
void coord_position_ecrire( coord_t * c , const off_t position )
{
  c->pos = position  ;
}

/*
 *===========================================
 * Realisations des primitives des sous-types
 *===========================================
 */

extern
char *
coord_ori2string( const orientation_t ori )
{
     switch( ori )
     {
	case VERTICAL : 
	     return("VERTICALE");
	     break;
	case HORIZONTAL :
	     return("HORIZONTALE");
	     break;
	case OBLIQUE :
	     return("OBLIQUE");
	     break ;
	default :
	     return("inconnue");
	     break;
     }
}


extern
char *
coord_dir2string( const direction_t dir )
{
     switch( dir )
     {
	case NORD :
	     return("NORD");
	     break ;
	case EST :
	     return("EST");
	     break ;
	case SUD :
	     return("SUD");
	     break ;
	case OUEST :
	     return("OUEST");
	     break ;
	default :
	     return("inconnue");
	     break ;
     }
}
/*
 *===========================================
 * Realisations des primitives du TDA coord
 *===========================================
 */

/* 
 * Affichage des coordonnees d'une case 
 */

extern
void 
coord_afficher( FILE * const stream ,
		const coord_t coord )
{
     fprintf( stream , "[%d,%d](%ld)" , 
	      coord.l , 
	      coord.c ,
	      coord.pos );
}


/* 
 * Ecriture des coordonnees d'une case 
 */

extern
void 
coord_ecrire( coord_t * coord , 
	      const int ligne,
	      const int colonne,
	      const off_t position )
{
     coord->l = ligne ;
     coord->c = colonne ;
     coord->pos = position ;
}

/* 
 * Lecture des coordonnees d'une case 
 */

extern
void 
coord_lire( const coord_t coord , 
	    int * ligne,
	    int * colonne,
	    off_t * position )
{
     (*ligne)    = coord.l ;
     (*colonne)  = coord.c ;
     (*position) = coord.pos ;
}

/* 
 * Comparaison de 2 coordonnees 
 * retour < 0 si c1 < c2 
 *        = 0 si c1 = c2 
 *        > 0 si c1 > c2 
 */

extern 
int
coord_comparer( const coord_t c1,
		const coord_t c2 )
{
     int cmp_l = c1.l - c2.l ;
     int cmp_c = c1.c - c2.c ;
     int cmp_pos = (int)(c1.pos - c2.pos) ;

     /*----------*/

     if( cmp_l )
	  return(cmp_l);
     else
     {
	  if( cmp_c )
	       return(cmp_c);
	  else
	       return(cmp_pos) ;
     }		    
}

/* 
 * Affectation des coordonnees d'une case 
 */

extern
void 
coord_affecter( coord_t * coord_cible,
		const coord_t coord_source )
{
     coord_cible->l = coord_source.l ;
     coord_cible->c = coord_source.c ;
     coord_cible->pos = coord_source.pos ;
}

/* 
 * Creation d'une liste de cases
 */

extern
void
coord_liste_creer( coord_t ** liste ) 
{
     (*liste) = (coord_t *)NULL ;
}

/* 
 * Destruction d'une liste de cases
 */

extern
void 
coord_liste_detruire( coord_t ** liste )
{
     free( (*liste) );
     (*liste) = (coord_t *)NULL ;
}

/* 
 * Ajout d'un element dans une liste de cases
 */

extern 
void
coord_liste_ajouter( coord_t ** liste ,		/* liste des cases */
		     int * nb_elems,		/* nb de cases avant/apres */
		     const coord_t elem )	/* element a ajouter */
{
		     
     if( (*nb_elems) == 0 )
     {
	  (*liste)= (coord_t *)malloc(sizeof(coord_t));
     }
     else
     {
	  (*liste) = (coord_t *)realloc((*liste),sizeof(coord_t)*((*nb_elems)+1));
     }

     coord_affecter( &((*liste)[(*nb_elems)]) , elem ) ;

     (*nb_elems)++ ;
}

/* 
 * Recherche d'un element dans une liste de cases
 * selon les criteres donnes. 
 * Si criteres.pos == -1 alors recherche sur les [y,x]
 * Sinon recherche sur la position
 * renvoie VRAI si l'element est trouve 
 *         FAUX si l'element n'est pas dans la liste
 */

extern 
booleen_t
coord_liste_rechercher( coord_t * const liste ,	/* liste des cases */
			const int nb_elems,	/* nb de cases */
			const coord_t criteres,	/* criteres de recherche */
			int * indice )		/* indice de l'element trouve */
{
	
     booleen_t trouve = FAUX ;
     int i = 0 ;
     booleen_t critere_position = ( criteres.pos == -1 ? FAUX : VRAI ) ;

     /*----------*/
     
     (*indice) = -1 ;

     while( (i<nb_elems) && (!trouve) )
     {
	  if( critere_position ) 
	  {
	       if( criteres.pos == liste[i].pos )
		    trouve = VRAI ;
	  }
	  else
	  {
	       if( ( criteres.l == liste[i].l ) && ( criteres.c == liste[i].c ) )
		    trouve = VRAI ;
	  }

	  if(!trouve) 
	       i++;
     }

     if( trouve )
	  (*indice) = i ;

     return(trouve);
}

/* 
 * Affichage d'une liste de cases
 */

extern 
void
coord_liste_afficher( FILE * const stream ,     /* canal d'affichage */ 
		      coord_t * const liste ,	/* liste des cases */
		      const int nb_elems )	/* nb de cases */
{
     int i = 0 ;

     /*----------*/
     
     fprintf( stream , "{ " );
     for( i=0 ; i<nb_elems ; i++ )
     {
	  coord_afficher( stream , liste[i] );
	  fprintf( stream , " " );
     }
     fprintf( stream , "}");
}

/* 
 * Determination de l'orientation de 2 coordonnees
 */

extern
orientation_t
coord_orientation_donner( const coord_t c1 ,
			  const coord_t c2 )
{
 
     if( c1.l == c2.l )
     {
	  return( HORIZONTAL );
     }
     else
     {
	  if( c1.c == c2.c )
	       return( VERTICAL );
	  else
	       return( OBLIQUE ) ;
     }
}
