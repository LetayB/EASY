#ifndef _COMMUN_H_
#define _COMMUN_H_
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
 
/* Constantes */
 
#define MAX_STRING 256 
 
 
/* Message d'erreur */
 
typedef int err_t ;
 
#define OK 0 
 
 
#define ERR_FICHIER -100 
#define ERR_MEMOIRE -101
#define ERR_LIGNE_VIDE -102
 
 
/* Types */
typedef enum booleen_s { FAUX , VRAI } booleen_t ; 
 
#endif
 