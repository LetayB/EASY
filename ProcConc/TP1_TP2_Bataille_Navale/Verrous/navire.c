/*----------------------------------------------------------------------

   Nom         : navire.c

   Description : processus indépendant représentant un navire

   Usage       : ./navire.c <fichier mer> <nom bateau>

   Auteur      : FROGER Adrien - L3 SPI - groupe 2

/--------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>  //pour la primitive fcntl
#include <string.h> //pour strcpy
#include <unistd.h> //pour sleep
#include <time.h>   //pour initialisation du random

#include <mer.h>
#include <commun.h>
#include <bateau.h>



/////////////////////////
//      FONCTIONS      //
/////////////////////////


//=====================================================================
//==========[ VEROUILLAGE/DEVEROUILLAGE POUR LA MER ENTIERE ]==========
//=====================================================================

/*
* Verrouille la totalité du fichier mer en vue du placement aléatoire d'un nouveau bâteau dans ce dernier.
* Renvoie CORRECT si le fichier mer pu être verouillé, ERREUR le cas contraire.
*/
int verrouiller_mer( const int fd,	/* Fichier mer */
                    bateau_t * bateau)  /* Bâteau qui va se déplacer (propriétaire du verrou) */
{
    /* Déclaration d'un verrou interne (structure flock) */
    struct flock verrou;

    /* Construction du verrou interne : affectation de la structure flock précédemment créée */

    verrou.l_type   = F_WRLCK ;	  /* verrou en écriture (exclusif) */
			          /* --> incompatible avec la pose de tout autre verrou */
    verrou.l_whence = (short)0 ;  /* type de repérage dans le fichier à verrouiller */
     					// --> 0  par rapport àu debut
      					//     1  par rapport à la position courante
      					//     2  par rapport à la fin
    verrou.l_start  = (long)0; 	  /* debut de la portée du v. (0 = début peu utile ici car l_len à 0)*/
    verrou.l_len    = 0;	  /* longueur de la zone à verrouiller (0 = tout le fichier) */
    verrou.l_pid    = bateau_pid_lire(*bateau);	/* PID du bâteau verrouilleur*/

    printf("Bateau %c --> attente pose verrou interne sur mer\n" , 
    bateau_marque_lire(*bateau));

    //pose bloquante : si mer non verouillable, le bâteau attend jusqu'à pouvoir la verrouiller
    if( fcntl( fd , F_SETLKW , &verrou ) == -1 ) 
    {
        return(ERREUR);
    }

    /* Fin de l'attente passive, le verrou est posé */
    printf("Bateau %c --> pose du verrou interne sur mer\n", bateau_marque_lire(*bateau));

    return(CORRECT);
}


/*
* Déverrouille la totalité du fichier mer en vue du placement aléatoire d'un nouveau bâteau sur ce dernier
* Renvoie CORRECT ou ERREUR en cas d'échec.
*/
int deverrouiller_mer( const int fd,	  /* Fichier mer */
                      bateau_t * bateau)  /* Bâteau qui s'est deplacé (propriétaire du verrou) */
{
    /* Déclaration d'un verrou interne (structure flock) */
    struct flock verrou;

    /* Construction du verrou interne : affectation de la structure flock précédemment créée */

    verrou.l_type   = F_UNLCK ;	  /* levée de verrou */
    verrou.l_whence = (short)0 ;  /* type de repérage dans le fichier à verrouiller */
     					// --> 0  par rapport àu debut
      					//     1  par rapport à la position courante
      					//     2  par rapport à la fin
    verrou.l_start  = (long)0; 	  /* debut de la portée du v. (0 = début peu utile ici car l_len à 0)*/
    verrou.l_len    = 0;	  /* longueur de la zone à verrouiller (0 = tout le fichier) */
    verrou.l_pid    = bateau_pid_lire(*bateau);	/* PID du bâteau déverrouilleur*/

    printf("Bateau %c --> demande levee verrou interne sur mer\n" , bateau_marque_lire(*bateau));

    if( fcntl( fd , F_SETLK , &verrou ) == -1 )
    {
        return(ERREUR);
    }

    printf("Bateau %c --> levee du verrou interne sur mer\n", bateau_marque_lire(*bateau));

    return(CORRECT);
}

//==========================================================================
//==========[ VEROUILLAGE/DEVEROUILLAGE POUR LE HEADER DE LA MER ]==========
//==========================================================================

/*
* Verrouille le header du fichier mer en lecture pour signaler à vision.c qu'un déplacement est en cours.
* Renvoie CORRECT si le header a pu être verouillé, ERREUR le cas contraire.
*/
int verrouiller_header_mer( const int fd,      /* Fichier mer */
                           bateau_t * bateau)  /* Bâteau qui va se déplacer (propriétaire du verrou) */
{
    /* Déclaration d'un verrou interne (structure flock) */
    struct flock verrou;

    /* Construction du verrou interne : affectation de la structure flock précédemment créée */

    verrou.l_type   = F_RDLCK ;		  /* verrou en lecture (paratagé) */
			         	  /* --> pose d'autres verrous en lecture possible  */
    verrou.l_whence = (short)0 ;	  /* type de repérage dans le fichier à verrouiller */
     						// --> 0  par rapport àu debut
      						//     1  par rapport à la position courante
      						//     2  par rapport à la fin
    verrou.l_start  = (long)0; 		  /* debut de la portée du verrou (0 = début) */
    verrou.l_len    = MER_TAILLE_ENTETE;  /* longueur de la zone à verrouiller (l'entete) */
    verrou.l_pid    = bateau_pid_lire(*bateau);	/* PID du bâteau verrouilleur*/

    printf("Bateau %c --> attente pose verrou interne sur l'entete du fichier mer\n" ,
    bateau_marque_lire(*bateau));

    if( fcntl( fd , F_SETLKW , &verrou ) == -1 ) 
    //pose bloquante : si trop de verrous existants, le bâteau attend de pouvoir verrouiller
    {
        return(ERREUR);
    }



    /* Fin de l'attente passive, le verrou est posé */
    printf("Bateau %c --> pose du verrou interne  sur l'entete du fichier mer\n", 
    bateau_marque_lire(*bateau));

    return(CORRECT);
}


/*
* Déverrouille le header du fichier mer
* Renvoie CORRECT si le header a pu être verrouillé, ERREUR le cas contraire.
*/
int deverrouiller_header_mer( const int fd,	/* Fichier mer */
                             bateau_t * bateau)	/* Bâteau qui s'est déplacé (propriétaire du verrou) */
{
    /* Déclaration d'un verrou interne (structure flock) */
    struct flock verrou;

    /* Construction du verrou interne : affectation de la structure flock précédemment créée */

    verrou.l_type   = F_UNLCK ;		  /* levée de verrou */
    verrou.l_whence = (short)0 ;	  /* type de repérage dans le fichier à verrouiller */
     						// --> 0  par rapport àu debut
      						//     1  par rapport à la position courante
      						//     2  par rapport à la fin
    verrou.l_start  = (long)0; 		  /* debut de la portée du verrou (0 = début) */
    verrou.l_len    = MER_TAILLE_ENTETE;  /* longueur de la zone à verrouiller (l'entete) */
    verrou.l_pid    = bateau_pid_lire(*bateau);	/* PID du bâteau déverrouilleur*/

    printf("Bateau %c --> demande levee verrou interne sur l'entete du fichier mer\n" ,
    bateau_marque_lire(*bateau));

    if( fcntl( fd , F_SETLK , &verrou ) == -1 )
    {
        return(ERREUR);
    }

    printf("Bateau %c --> levee du verrou interne sur l'entete du fichier mer\n", 
    bateau_marque_lire(*bateau));

    return(CORRECT);
}



//===============================================================================
//==========[ VEROUILLAGE/DEVEROUILLAGE POUR LE VOISINAGE D'UN BATEAU ]==========
//===============================================================================

/*
* Verouille le voisinage passé en paramètre d'un bâteau passé en paramètre en vue du déplacement de ce bâteau
* Renvoie CORRECT si tout le voisinage a pu être verouille, ERREUR le cas contraire.
*/
int navire_verrouiller_voisinage( const int fd,			 /* Fichier mer */
                                 bateau_t * bateau,		 /* Bâteau qui va se déplacer */
                                 coord_t * const liste_voisins,  /* Liste des cases voisines */
                                 const int nb_voisins)		 /* Nombre de cases voisines 
								    dans la liste */
{
    int i;
    case_t case_mer ;

    /* Déclaration d'un verrou interne (structure flock) */
    struct flock verrou;

    /* Construction du verrou interne : affectation de la structure flock précédemment créée */

    verrou.l_type   = F_WRLCK ;		  /* verrou en écriture (exclusif) */
	  				  /* --> incompatible avec la pose de tout autre verrou */


    verrou.l_whence = (short)0 ;	  	/* type de repérage dans le fichier à verrouiller */
     							// --> 0  par rapport àu debut
      							//     1  par rapport à la position courante
      							//     2  par rapport à la fin
    verrou.l_len    = (long)MER_TAILLE_CASE;	/* longueur de la portée du verrou : une case */
    verrou.l_pid    = bateau_pid_lire(*bateau);	/* PID du bâteau verrouilleur*/


    /* Parcours la liste des cases à verrouiller */
    for(i=0; i<nb_voisins; i++) //pour chaque case
    {
        mer_case_lire( fd , liste_voisins[i], &case_mer );
        if( case_mer == MER_CASE_LIBRE )
        {
            /* On indique la posit° de la case à verrouiller dans le champ l_start de la struct flock*/
            verrou.l_start  = (long)liste_voisins[i].pos;
            
	    printf("Bateau %c --> demande pose d'un verrou interne sur la case en [%i,%i]\n", 
	    bateau_marque_lire(*bateau),liste_voisins[i].l,liste_voisins[i].c);

            /* On pose un verrou interne non bloquant exclusif sur la case à verrouiller */
	    //pose non bloquante : si le verouillage d'un voisin n'est pas possible, on retourne ERREUR
            if( fcntl( fd , F_SETLK , &verrou ) == -1 ) 
            {
                return(ERREUR);
            }

            printf("Bateau %c --> pose le verrou interne sur la case en [%i,%i]\n",
	    bateau_marque_lire(*bateau),liste_voisins[i].l,liste_voisins[i].c);
        }
    }

    return(CORRECT);
}


/*
* Déverouille le voisinage passé en paramètre d'un bâteau passé en paramètre
* Renvoie CORRECT ou ERREUR en cas d'échec.
*/
int navire_deverrouiller_voisinage( const int fd,		   /* Fichier mer */
                                   bateau_t * bateau,		   /* Bâteau qui va se déplacer */
                                   coord_t * const liste_voisins,  /* Liste des cases voisines */
                                 const int nb_voisins)		   /* Nombre de cases voisines 
								      dans la liste */
{
    int i;

    /* Déclaration d'un verrou interne (structure flock) */
    struct flock verrou;

    /* Construction du verrou interne : affectation de la structure flock précédemment créée */

    verrou.l_type   = F_UNLCK ;		    	/* levée de verrou */
    verrou.l_whence = (short)0 ;		/* type de repérage dans le fichier à verrouiller */
     							// --> 0  par rapport àu debut
      							//     1  par rapport à la position courante
      							//     2  par rapport à la fin
    verrou.l_len    = (long)MER_TAILLE_CASE;	/* longueur de la portée du verrou : une case */
    verrou.l_pid    = bateau_pid_lire(*bateau);	/* PID du bâteau déverrouilleur*/


    /* Parcours la liste des cases à déverrouiller */
    for(i=0; i<nb_voisins; i++) //pour chaque case
    {
        /* On indique la posit° de la case à déverrouiller dans le champ l_start de la struct flock*/
        verrou.l_start  = (long)liste_voisins[i].pos;

        printf("Bateau %c --> demande levee d'un verrou interne sur la case en [%i,%i]\n", 
	bateau_marque_lire(*bateau),liste_voisins[i].l,liste_voisins[i].c);


        /* On déverouille la case */
        if( fcntl( fd , F_SETLK , &verrou ) == -1 )
        {
            return(ERREUR);
        }

        printf("Bateau %c --> leve le verrou interne sur la case en [%i,%i]\n", 
	bateau_marque_lire(*bateau),liste_voisins[i].l,liste_voisins[i].c);
    }

    return(CORRECT);
}



//==============================================================================
//==========[ VEROUILLAGE/DEVEROUILLAGE POUR LE BOUCLIER D'UN BATEAU ]==========
//==============================================================================


/*
* Verouille les cases constituant le corps d'un bâteau passé en paramètre (= activation du bouclier)
* Renvoie CORRECT ou ERREUR en cas d'échec.
*/
int navire_verrouiller_corps( const int fd,	 /* Fichier mer */
                             bateau_t * bateau)  /* Bâteau dont on souhaite verrouiller le corps */
{
    int i;
    coord_t * corps_bateau;

    /* Récupération des coordonnées du corps du bâteau */
    corps_bateau = bateau_corps_lire(bateau);

    /* Déclaration d'un verrou interne (structure flock) */
    struct flock verrou;

    /* Construction du verrou interne : affectation de la structure flock précédemment créée */

    verrou.l_type   = F_RDLCK ;		    	/* verrou en lecture (exclusif) */
			          		/* --> incompatible avec la pose de tout autre verrou*/
    verrou.l_whence = (short)0 ;		/* type de repérage dans le fichier à verrouiller */
     							// --> 0  par rapport àu debut
      							//     1  par rapport à la position courante
      							//     2  par rapport à la fin
    verrou.l_len    = (long)MER_TAILLE_CASE;	/* longueur de la portée du verrou : une case */
    verrou.l_pid    = bateau_pid_lire(*bateau);	/* PID du bâteau verrouilleur*/

    /* Parcours la liste des cases à verrouiller (corps du bâteau) */
    for(i=0; i<BATEAU_TAILLE; i++) //pour chaque case
    {
        /* On indique la posit° de la case à verrouiller dans le champ l_start de la struct flock*/
        verrou.l_start  = (long)corps_bateau[i].pos;

        printf("Bateau %c --> attente bouclier sur la case [%i,%i]\n", 
	bateau_marque_lire(*bateau),corps_bateau[i].l,corps_bateau[i].c);

        /* On pose un verrou interne bloquant sur la case à verrouiller */
	// Pose bloquante : le bâteau attend de pouvoir activer son boulier pour poursuivre
        if( fcntl( fd , F_SETLKW , &verrou ) == -1 ) 
        {
            return(ERREUR);
        }

    	/* Fin de l'attente passive, le verrou est posé */
        printf("Bateau %c --> pose bouclier sur la case en [%i,%i]\n", 
	bateau_marque_lire(*bateau),corps_bateau[i].l,corps_bateau[i].c);
    }

    return(CORRECT);
}

/*
* Déverouille les cases constituant le corps d'un bâteau passé en paramètre (= désactivation du bouclier)
* Renvoie CORRECT ou ERREUR en cas d'échec.
*/
int navire_deverrouiller_corps( const int fd,	   /* Fichier mer */
                               bateau_t * bateau)  /* Bateau dont on souhaite verrouiller le corps */
{
    int i;
    coord_t * corps_bateau;

    /* Récupération des coordonnées du corps du bâteau */
    corps_bateau = bateau_corps_lire(bateau);

    /* Déclaration d'un verrou interne (structure flock) */
    struct flock verrou;

    /* Construction du verrou interne : affectation de la structure flock précédemment créée */

    verrou.l_type   = F_UNLCK ;		    	/* levée de verrou */
    verrou.l_whence = (short)0 ;		/* type de repérage dans le fichier à verrouiller */
     							// --> 0  par rapport àu debut
      							//     1  par rapport à la position courante
      							//     2  par rapport à la fin
    verrou.l_len    = (long)MER_TAILLE_CASE;	/* longueur de la portée du verrou : une case */
    verrou.l_pid    = bateau_pid_lire(*bateau);	/* PID du bâteau déverrouilleur*/

    /* Parcours la liste des cases a deverrouiller (corps du bâteau) */
    for(i=0; i<BATEAU_TAILLE; i++) //pour chaque case
    {
        /* On indique la posit° de la case à déverrouiller dans le champ l_start de la struct flock*/
        verrou.l_start  = (long)corps_bateau[i].pos;
        printf("Bateau %c --> demande levee d'un verrou interne sur la case en [%i,%i]\n", 
	bateau_marque_lire(*bateau),corps_bateau[i].l,corps_bateau[i].c);

        /* On déverouille la case */
        if( fcntl( fd , F_SETLK , &verrou ) == -1 )
        {
            return(ERREUR);
        }

        printf("Bateau %c --> leve le verrou interne sur la case en [%i,%i]\n", 
	bateau_marque_lire(*bateau),corps_bateau[i].l,corps_bateau[i].c);
    }

    return(CORRECT);
}





//=============================================================
//==========[ VEROUILLAGE/DEVEROUILLAGE D'UNE CIBLE ]==========
//=============================================================

/*
* Verouille une cible passée en paramètre pour un bâteau passé en paramètre.
* Le bateau tente de poser un verrou en écriture (exclusif) sur la case. En cas d'échec, cela
* signifie que la case est déjà verouillée (bouclier activé ou cible vérouillée par un autre bâteau). 
* Il ne peut donc pas tirer.
* Renvoie CORRECT si la cible a pu être verouille (pas de bouclier) ou ERREUR le cas contraire.
*/
int navire_verrouiller_cible( const int fd,	 /* Fichier mer */
                             bateau_t * bateau,  /* Bateau tireur */
                             coord_t cible)	 /* Cible visée */
{

    /* Déclaration d'un verrou interne (structure flock) */
    struct flock verrou;


    /* Construction du verrou interne : affectation de la structure flock précédemment créée */

    verrou.l_type   = F_WRLCK ;		    	/* verrou en écriture (exclusif) */
			          		/* --> incompatible avec la pose de tout autre verrou*/
    verrou.l_whence = (short)0 ;		/* type de repérage dans le fichier à verrouiller */
     							// --> 0  par rapport àu debut
      							//     1  par rapport à la position courante
      							//     2  par rapport à la fin
    verrou.l_start  = (long)cible.pos;		/* debut de la portée du verrou */
    verrou.l_len    = (long)MER_TAILLE_CASE;	/* longueur de la portée du verrou : une case */
    verrou.l_pid    = bateau_pid_lire(*bateau);	/* PID du bâteau verrouilleur*/


    printf("Bateau %c --> demande verouillage de la cible [%i,%i]\n", 
    bateau_marque_lire(*bateau),cible.l,cible.c);

    /* On pose un verrou interne non bloquant exclusif sur la case à verrouiller */
    if( fcntl( fd , F_SETLK , &verrou ) == -1 ) //Renvoie -1 si pose impossible
    {
        return(ERREUR);
    }

    printf("Bateau %c --> verouillage de la cible [%i,%i]\n", 
    bateau_marque_lire(*bateau),cible.l,cible.c);

    return(CORRECT);
}



/*
* Déverouille une cible passée en paramètre pour un bâteau passé en paramètre.
* Renvoie CORRECT si la cible a pu être déverouillée ou ERREUR le cas contraire.
*/
int navire_deverrouiller_cible( const int fd,	   /* Fichier mer */
                               bateau_t * bateau,  /* Bateau tireur */
                               coord_t cible)	   /* Cible visee */
{
    /* Déclaration d'un verrou interne (structure flock) */
    struct flock verrou;

    /* Construction du verrou interne : affectation de la structure flock précédemment créée */

    verrou.l_type   = F_UNLCK ;		    	/* levée de verrou */
    verrou.l_whence = (short)0 ;		/* type de repérage dans le fichier à verrouiller */
     							// --> 0  par rapport àu debut
      							//     1  par rapport à la position courante
      							//     2  par rapport à la fin
    verrou.l_start  = (long)cible.pos;		/* debut de la portée du verrou */
    verrou.l_len    = (long)MER_TAILLE_CASE;	/* longueur de la portée du verrou : une case */
    verrou.l_pid    = bateau_pid_lire(*bateau);	/* PID du bâteau déverrouilleur*/

    printf("Bateau %c --> demande deverouillage de la cible [%i,%i]\n", 
    bateau_marque_lire(*bateau),cible.l,cible.c);

    /* Levée du verrou */
    if( fcntl( fd , F_SETLK , &verrou ) == -1 )
    {
        return(ERREUR);
    }
    printf("Bateau %c --> deverouillage de la cible [%i,%i]\n", 
    bateau_marque_lire(*bateau),cible.l,cible.c);

    return(CORRECT);
}








////////////////////
//      MAIN      //
////////////////////


int main( int nb_arg , char * tab_arg[] )
{
    char nomprog[128]  ; // Nom du programme (navire)
    char fich_mer[128] ; // Référence (chemin + nom + extension) du fichier mer
    char marque_bateau ; // Marque du nouveau bâteau

    int fd_mer ; // Descripteur du fichier mer

    int no_err = CORRECT ;

    int nbBateaux; // Nombre de bâteaux dans la mer
    booleen_t init = VRAI;


    // Gestion des cases voisines
    coord_t * liste_voisins = (coord_t *)NULL ;
    int nb_voisins = 0 ;

    // Gestion des tirs
    coord_t cible ;
    booleen_t acquisition ;


    // Gestion du bâteau
    bateau_t nouveau_bateau;
    pid_t pid ;  // PID du bâteau
    int energie; // Energie du bâteau

    booleen_t ok    = VRAI ; //indique si le bâteau a reussi à se deplacer
    booleen_t coule = FAUX ; //indique si le bâteau a été coulé


    /* Vérification du nombre d'arguments */
    if( nb_arg != 3 )
    {
        fprintf( stderr , "Usage : %s <nom fichier mer> <nom bateau>\n",
                 tab_arg[0] );
        exit(-1) ;
    }

    /* Recopie des paramètres */
    strcpy( nomprog  	  , tab_arg[0] );
    strcpy( fich_mer 	  , tab_arg[1] );
    marque_bateau = *(tab_arg[2]);


    /* Ouverture fichier mer */
    if( ( fd_mer = open( fich_mer , O_RDWR , 0644 )) == -1 )
    {
        fprintf( stderr, "%s : Probleme durant l'ouverture sur %s\n", nomprog , fich_mer);
        exit(-1);
    }
    printf("Mer ouverte... \n");


    /* Initialisation des informations concernant le bâteau */

    // Récupération du PID du bâteau
    pid = getpid();
    printf("PID du nouveau bateau : %d \n", pid);

    // Initialisation des champs du bâteau et de l'énergie du bâteau
    bateau_marque_ecrire( &nouveau_bateau , marque_bateau );
    bateau_pid_ecrire( &nouveau_bateau , pid );
    bateau_actif_ecrire( &nouveau_bateau , VRAI );
    energie = BATEAU_MAX_ENERGIE; //BATEAU_MAX_ENERGIE défini dans bateau.h

    /* Initialisation du bâteau dans la mer */

    /*//////////////////////////////
    // Verouillage du fichier mer //
    /*////////////////////////////////////////////////////////////////////*
    //
    //	Verrou  sur  la totalité du fichier mer  car le placement
    //	d'un  bâteau  est  aléatoire. On ne connait  donc  pas  à
    //	l'avance  les  cases  concernées par l'arrivée du nouveau 
    //	bâteau, nous  sommes donc  obligé  de de  verrouiller  la  
    //  totalité du fichier mer.
    //
    //            .-""-.
    /*           / .--. \
    //          / /    \ \
    //          | |    | |
    //          | |.-""-.|
    //         ///`.::::.`\
    //        ||| ::/  \:: ;
    //        ||; ::\__/:: ;
    //         \\\ '::::' /
    //          `=':-..-'`
    //
    /////////////////////////////////////////////////////////////////////*/
    //si le bâteau n'a pu verouiller le fichier mer à terme de son attente passive, il est tué
    if( ( no_err = verrouiller_mer( fd_mer , &nouveau_bateau )) )
    {
        printf("Probleme durant le verouillage de la mer pour l'initialisation du bâteau %c\n", 
	bateau_marque_lire(nouveau_bateau)) ;
        exit(no_err) ;
    }

    /* Initialisation du bâteau dans la mer */
    if( ( no_err = mer_bateau_initialiser( fd_mer , &nouveau_bateau) ) )
    {
        fprintf( stderr, "%s : erreur %d dans mer_bateau_initialiser\n", nomprog , no_err );
        if( no_err == ERREUR )
        {
            fprintf( stderr, 
	    "\t(mer_bateau_initialiser n'a pas pu placer le bateau \"%c\" dans la mer)\n" , 
	    bateau_marque_lire(nouveau_bateau) );
        }
        exit(no_err) ;
    }
    else
    {
    	/* On incrémente le nombre de bâteaux */
        mer_nb_bateaux_lire  ( fd_mer , &nbBateaux );
        mer_nb_bateaux_ecrire( fd_mer , ++nbBateaux);
    }


    /*////////////////////////////////
    // Déverouillage du fichier mer //
    /*/////////////////////////////////////////////////////////////////////*/
    if( (no_err = deverrouiller_mer( fd_mer , &nouveau_bateau )) )
    {
        printf("Probleme durant le deverouillage de la mer post-initialisation du bâteau %c\n", 
	bateau_marque_lire(nouveau_bateau));
        exit(no_err) ;
    }
    /////////////////////////////////////////////////////////////////////////


    /* Si le bateau dispose d'assez d'energie, il active son bouclier */
    if(energie >= BATEAU_SEUIL_BOUCLIER) //BATEAU_SEUIL_BOUCLIER défini dans bateau.h
    {
    	/*/////////////////////////////
   	// Verouillage pour bouclier //
        /*//////////////////////////////////////////////////////////////////////*
        //
        // Si  le  bâteau  a  assez d'énergie, il active  son  bouclier.
        // Cela implique que les autres bateaux ne peuvent le verouiller
        // comme cible pour envoyer un boulet de canon dans son corps.
        //
        //            .-""-.
        /*           / .--. \
        //          / /    \ \
        //          | |    | |
        //          | |.-""-.|
        //         ///`.::::.`\
        //        ||| ::/  \:: ;
        //        ||; ::\__/:: ;
        //         \\\ '::::' /
        //          `=':-..-'`
        //
        ///////////////////////////////////////////////////////////////////////*/
        if( (no_err = navire_verrouiller_corps( fd_mer , &nouveau_bateau )) )
        {
            printf("Probleme durant l'activation du bouclier du bâteau %c\n",
	    bateau_marque_lire(nouveau_bateau));
            exit(no_err) ;
        }
    }

    sleep (5); // Attente pour permettre aux autres bateaux de s'initlialiser 
	       // (d'avoir l'opportunité de verouiller le fichier mer intégralement)

    /* Initialisation de la fonction random */
    srandom(getpid()) ;

    /*
    * Actions cycliques du bâteau
    */

    // Les bâteaux ne cessent de se déplacer tant qu'ils leur reste assez d'énergie pour cela
    while( energie > (5*BATEAU_MAX_ENERGIE/100) )
    {
        /* Attente aléatoire de 2 à 5 secondes avant un déplacement */
        sleep(2 + (5-2)*((float)(rand()) / (float)(RAND_MAX)));


        /* Verification du bâteaux pré-déplacement */

        // Avant de se déplacer, le bâteau vérifie qu'il n'a pas été coulé par un autre bâteau
        if( (no_err = mer_bateau_verifier( fd_mer,nouveau_bateau, &coule )) )
        {
            fprintf( stderr, "%s : erreur %d dans mer_bateau_verifier\n", nomprog , no_err );
            exit(no_err) ;
        }

        if( coule ) // Le bâteau a éte coulé
        {
            printf("#==============================\n");
            printf("====== Le bateau %c coule ======\n", bateau_marque_lire(nouveau_bateau));
            printf("#==============================\n");

            bateau_actif_ecrire( &nouveau_bateau , FAUX ); //on passe son statut en inactif 
							   //(peu utile ici vu la suite des 
							   // instructions)
            mer_bateau_couler( fd_mer,  nouveau_bateau );  //on l'efface de la carte

            /* On décrémente le nombre de bâteaux */
            mer_nb_bateaux_lire  ( fd_mer , &nbBateaux );
            mer_nb_bateaux_ecrire( fd_mer , --nbBateaux);

            close(fd_mer);
            exit(0);
        }




        /* Il vérifie également qu'il n'est pas vainqueur */
        mer_nb_bateaux_lire( fd_mer , &nbBateaux);
        if(nbBateaux > 1) init = FAUX;
        if(nbBateaux == 1 && init == FAUX)
        {
            printf("#======================================\n");
            printf("====== Le bateau %c est vainqueur ======\n", bateau_marque_lire(nouveau_bateau));
            printf("#======================================\n");

            close(fd_mer);
            exit(0);
        }


        /*
        *  Preparation du deplacement
        */

        /* Récupération de la liste des cases voisines au bateau */
        coord_liste_creer(&liste_voisins);

        if( (no_err = mer_voisins_rechercher( fd_mer, nouveau_bateau, &liste_voisins, &nb_voisins)) )
        {
            fprintf( stderr, "%s : erreur %d dans mer_voisins_rechercher\n", nomprog , no_err );
            exit(no_err) ;
        }

        printf( "Liste des voisins :\n");
        coord_liste_afficher( stdout , liste_voisins , nb_voisins );
        printf("\n\n");

    	/*////////////////////////////////////////////
   	// Verouillage des cases voisines du bâteau //
        /*//////////////////////////////////////////////////////////////////////*
        //
        // Le bâteau verrouille les cases libres disponibles pour son déplacement.
	// Ainsi, il réserve tout  pourtour disponible  pour son déplacement : un
	// autre bâteau ne pourra alors plus s'y déplacer. 
	// Si le verouillage d'une case échoue, c'est qu'elle est  déjà réservée 
	// pour  le  déplacement d'un autre bâteau.  Le  déplacement d'un bâteau 
	// étant aléatoire (N,S,E ou O), on considère alors le déplacement comme
	// impossible  afin de  s'assurer que deux bâteaux ne  tentent pas de se 
	// déplacer au même emplacement simultanément.
        //
        //            .-""-.
        /*           / .--. \
        //          / /    \ \
        //          | |    | |
        //          | |.-""-.|
        //         ///`.::::.`\
        //        ||| ::/  \:: ;
        //        ||; ::\__/:: ;
        //         \\\ '::::' /
        //          `=':-..-'`
        //
        ///////////////////////////////////////////////////////////////////////*/
	if (navire_verrouiller_voisinage( fd_mer, &nouveau_bateau, liste_voisins, nb_voisins) 
	    == ERREUR )
        {
            printf ("--> Erreur lors du verouillage des cases voisines\n");
            ok = FAUX;
        }
        else
        {

            /* Si le bâteau dispose d'assez d'énergie pour avoir un bouclier
	       c'est qu'il a activé son bouclier */
	    /* On le désactive donc vu qu'il va se déplacer 
	       (ne plus être sur les cases qu'il a protégé avec son bouclier */
            if(energie >= BATEAU_SEUIL_BOUCLIER) //on desative son bouclier
            {

		/*/////////////////////////////
		// Déverouillage du bouclier //
		/*/////////////////////////////////////////////////////////////////////*/
                if( (no_err = navire_deverrouiller_corps( fd_mer , &nouveau_bateau )) )
                {
                    printf("Probleme durant la desactivation du bouclier du bâteau %c\n",
		    bateau_marque_lire(nouveau_bateau));
                    exit(no_err) ;
                }
    		/////////////////////////////////////////////////////////////////////////
            }
            //Note : Ce n'est pas grave si le bâteau se fait toucher entre la désactivation de son 
	    //bouclier et  le  moment ou son  déplacement  est effectif car si un boulet de  canon
            //s'inscruste  dans  sa   coque,  cela sera  "sans effet"  du  fait  que  la  fonction 
	    //mer_bateau_deplacer écrit la marque du bâteau au nouvel emplacement et la marque  de
            //de case vide à l'ancien. La boulet sera donc "gommé".

            /* Déplacement */

    	    /*////////////////////////////////////////
   	    // Verouillage du header du fichier mer //
            /*//////////////////////////////////////////////////////////////////////*
            //
            // Le bâteau verrouille le header du fichier mer pour signaler à vision.c
	    // qu'un déplacement est en cours.
            //
            //            .-""-.
            /*           / .--. \
            //          / /    \ \
            //          | |    | |
            //          | |.-""-.|
            //         ///`.::::.`\
            //        ||| ::/  \:: ;
            //        ||; ::\__/:: ;
            //         \\\ '::::' /
       	    //          `=':-..-'`
            //
  	    ///////////////////////////////////////////////////////////////////////*/
            verrouiller_header_mer(fd_mer, &nouveau_bateau);


            if( (no_err = mer_bateau_deplacer( fd_mer,&nouveau_bateau, liste_voisins, nb_voisins, 
	    				       &ok )) )
            {
                fprintf( stderr, "%s : erreur %d dans mer_bateau_deplacer\n", nomprog , no_err );
                exit(no_err) ;
            }

	    /*//////////////////////////////////////////
	    // Déverouillage du header du fichier mer //
	    /*/////////////////////////////////////////////////////////////////////*/
            deverrouiller_header_mer(fd_mer, &nouveau_bateau);
	    /////////////////////////////////////////////////////////////////////////

            /* Décrementation de l'énergie */
	    // Un deplacement consome 5% de l'energie initiale du bâteau */
            energie -= (5*BATEAU_MAX_ENERGIE)/100;
            printf("Energie restante pour le bateau %c : %i\n", 
	    bateau_marque_lire(nouveau_bateau), energie);


            /* Si le bateau dispose d'assez d'énergie, il active son bouclier */
            if(energie >= BATEAU_SEUIL_BOUCLIER)
            {
	    	/*/////////////////////////////
	   	// Verouillage pour bouclier //
		/*//////////////////////////////////////////////////////////////////////*
		//
		// Si  le  bâteau  a  assez d'énergie, il active  son  bouclier.
		// Cela implique que les autres bateaux ne peuvent le verouiller
		// comme cible pour envoyer un boulet de  canon dans son  corps.
		//
		//            .-""-.
		/*           / .--. \
		//          / /    \ \
		//          | |    | |
		//          | |.-""-.|
		//         ///`.::::.`\
		//        ||| ::/  \:: ;
		//        ||; ::\__/:: ;
		//         \\\ '::::' /
		//          `=':-..-'`
		//
		///////////////////////////////////////////////////////////////////////*/
                if( (no_err = navire_verrouiller_corps( fd_mer , &nouveau_bateau )) )
                {
                    printf("Probleme durant l'activation du bouclier du bâteau %c\n",
		    bateau_marque_lire(nouveau_bateau));
                    exit(no_err) ;
                }
            }

        }


	/*///////////////////////////////////////////////////////////////
	// Déverouillage des cases precédement verouillées (voisinage) //
	/*/////////////////////////////////////////////////////////////////////*/
        if( (no_err = navire_deverrouiller_voisinage( fd_mer, &nouveau_bateau, liste_voisins, 
						      nb_voisins)) )
        {
            printf("Probleme durant le deverouillage des cases voisines au bateau %c\n",
	    bateau_marque_lire(nouveau_bateau));
            exit(no_err) ;
        }
	/////////////////////////////////////////////////////////////////////////

        coord_liste_detruire(&liste_voisins);

        if( ok )
        {
            printf( "Deplacement reussi\n");
        }
        else
        {
            printf(" Deplacement impossible\n");
        }


        /* TIR */

        /* Acquisition d'une cible (bâteau différent de lui même) */
        if( (no_err = mer_bateau_cible_acquerir( fd_mer, nouveau_bateau, &acquisition, &cible )) )
        {
            fprintf( stderr, "%s : erreur %d dans mer_bateau_cible_acquerir\n",nomprog , no_err );
            exit(no_err) ;
        }

        if( acquisition ) // Le bâteau a acquis une cible
        {
            printf("Acquisition d'une cible par le bateau %c\n", bateau_marque_lire(nouveau_bateau) );
            bateau_afficher( stdout , nouveau_bateau );
            printf( "\n-->La cible choisie est la case \n");
            coord_afficher( stdout , cible );

	    /*///////////////////////////
	    // Verouillage de la cible //
	    /*//////////////////////////////////////////////////////////////////////*
	    //
 	    // Le bâteau tente de  verouiller la cible qu'il a acquis. 
	    // Si la fonction navire_verrouiller_cible renvoie ERREUR,
     	    // c'est que la cible est déjà verouillée (bouclier activé 
	    // ou  cible vérouillée  par un autre  bâteau). Le  bâteau 
	    // abandonne  alors son  tir. Sinon, il  tir sur sa cible.
	    //
	    //            .-""-.
	    /*           / .--. \
	    //          / /    \ \
	    //          | |    | |
	    //          | |.-""-.|
	    //         ///`.::::.`\
	    //        ||| ::/  \:: ;
	    //        ||; ::\__/:: ;
	    //         \\\ '::::' /
	    //          `=':-..-'`
	    //
	    ///////////////////////////////////////////////////////////////////////*/
	    //on tente de verouiller la case où l'on souhaite tirer
            if( navire_verrouiller_cible( fd_mer, &nouveau_bateau, cible) != ERREUR) 
            {

                /* Tir d'un boulet sur la cible acquise */
                if( (no_err = mer_bateau_cible_tirer( fd_mer, cible)) )
                {
                    fprintf( stderr, "%s : erreur %d dans mer_bateau_cible_tirer pour le bateau %c\n", 
		    nomprog , no_err, bateau_marque_lire(nouveau_bateau) );
                    exit(no_err) ;
                }

		/*/////////////////////////////
		// Déverouillage de la cible //
		/*/////////////////////////////////////////////////////////////////////*/
		//on tente de deverouiller la case où l'on a tiré
                if( navire_deverrouiller_cible( fd_mer, &nouveau_bateau, cible) == ERREUR) 
                {
                    fprintf( stderr, "%s : erreur %d dans deverouillage cible pour le bateau %c\n", 
		    nomprog , no_err, bateau_marque_lire(nouveau_bateau) );
                    exit(no_err) ;
                }
		/////////////////////////////////////////////////////////////////////////
            }
            else
            {
                printf("Impossible de verrouiller la cible : cible protegee\n");
            }

            printf( "\n\n");
        }
        else
        {
            printf("Pas d'acquisition de cible pour le bateau %c\n", 
	    bateau_marque_lire(nouveau_bateau) );
            bateau_afficher( stdout , nouveau_bateau );
            printf( "\n");
        }


    }//fin du while vérifiant que le bâteau a assez d'énergie pour se déplacer


    /* Le bâteau n'a plus d'énergie */
    printf("#===========================================\n");
    printf("====== Le bateau %c n'a plus d'energie ======\n", bateau_marque_lire(nouveau_bateau));
    printf("#===========================================\n");

    bateau_actif_ecrire( &nouveau_bateau , FAUX ); //on passe son statut en inactif 
						   //(peu utile ici vu la suite des instructions)
    mer_bateau_couler( fd_mer,  nouveau_bateau );  //on l'efface de la carte

    /* On décrémente le nombre de bâteaux */
    mer_nb_bateaux_lire  ( fd_mer , &nbBateaux );
    mer_nb_bateaux_ecrire( fd_mer , --nbBateaux);

    close(fd_mer);
    exit(0);
}

