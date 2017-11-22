#ifndef _MSG_MESSAGES_H_
#define _MSG_MESSAGES_H_

#include <messages.h>

#define TUBE_NOM "pipe_l3tp5"

/*
 * Definition des fonctions 
 */

/* Affichage d'un message */
extern
void pipe_afficher( char * const message );

/* Remplissage d'un message */
extern
void pipe_remplir( char * message , const char val ) ;

#endif
