#ifndef _MSG_MESSAGES_H_
#define _MSG_MESSAGES_H_

#include <messages.h>

#define CLE_BAL 72
#define MSG_TYPE 22

/* Structure message */

typedef struct corps 
{ 
  char buffer[MESSAGES_TAILLE];
} corps_t ; 

typedef struct message
{
  long     type ;
  corps_t corps ;
} message_t ;

/*
 * Definition des fonctions 
 */

/* Affichage d'un message */
extern
void msg_afficher( message_t * const message );

/* Remplissage d'un message */
extern
void msg_remplir( message_t * message , const char val ) ;

#endif
