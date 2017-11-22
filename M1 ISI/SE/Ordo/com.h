#ifndef _COM_H_
#define _COM_H_
 
#include <commun.h>
 
/* 
 * Type
 */
 
/*
 * Chargement d'une liste de commandes a partir d'un fichier 
 */
 
extern
err_t com_fichier_charger( char **** commandes , /* tableau commandes a remplir */
			   int * nb_commandes , 
			   char * fichier_liste_com ) ;  /* Nom du fichier */
 
/*
 * Affichage d'une commande 
 */
 
extern
void com_afficher(char ** commande ) ; /* tableau des arguments d'une commande se finissant par NULL */
 
/*
 * Affichage d'une liste de commandes
 */
extern
void com_liste_afficher(char *** commandes , /* tableau des commandes */
			const int nb_commandes ) ; /* Nb de commandes */
 
#endif
 