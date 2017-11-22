#if defined(__WIN32)             //Pour le rafraichissement de la console
#define CLEAR(); system("cls");
#else
#define CLEAR(); system("clear");
#endif

#define DEBUG      0 //Affichage de chaque etape
#define CLEARPRINT 1 //Affichage minimal

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <mer.h>
#include <bateau.h>

#include "signaux.h" //Correspondance nom__signal_perso <--> nom officiel




////////////////////////
/* Variables globales */
////////////////////////

char  nomprog[128] ; //nom du programme (amiral.c)
char fich_bat[128] ; //nom du programme (amiral.c) //référence (chemin + nom + extension) du fichier bateaux
char fich_mer[128] ; //nom du programme (amiral.c) //référence (chemin + nom + extension) du fichier mer

int fd_mer;

bateau_t * liste_bateaux; //Liste des bateaux en MC
int nb_bateaux; 	  //Nombre de bâteaux dans la liste

//Tableau pour contenir les energies des différents bateaux
int tab_energies[BATEAU_NB_MAX];

//Nombre cumule de bateaux traites par l'amiral
int cumul_bateaux = 0;



//////////////
/* HANDLERS */
//////////////


//Declenché par SIG_INFOS. Peu utile, à part pour rendre SIG_INFO capturable.
static void hdl_ecriture_ok(int sig)
{
    #if DEBUG
        printf("--> Le bateau s'est bien inscrit dans le fichier en MS\n");
    #endif
}

//Declenché par SIGALRM. Peu utile, à part pour rendre SIGALRM capturable.
static void hdl_sigalrm(int sig)
{
    #if DEBUG
        printf("--> Timeout !\n");
    #endif
}


//Ce handleur est declenche a chaque fois que l'amiral recoit une demande de jeu de la part d'un navire 
//Il permet d'offir la possibilité a un bateau 
//	- de s'ajouter a la liste en MC par le biais d'un fichier en MS
//      - de s'initialiser dans la mer
//      - de jouer (deplacement/tir)
//@param sig       : le numéro du signal recu
//@param * siginfo : pointeur sur structure siginfo pr differentes infos dont le PID de l'expediteur
//@param *context  : pointeur sur structure pour le contexte
static void hdl_jeu(int sig, siginfo_t *siginfo, void *context)
{
    #if !CLEARPRINT
        printf ("[Amiral] Reception d'une demande de jeu du bateau dont le pid est : %d\n", siginfo->si_pid);
    #endif

    //Indice du bateau courant dans la liste en MC
    int indice;

    //Gestion du déplacement
    coord_t * liste_voisins = (coord_t *)NULL;
    int nb_voisins = 0;
    booleen_t ok = FAUX;

    // Gestion du tir
    coord_t cible ;
    booleen_t acquisition ;
    int indice_bateau_cible;


    /* Traitement */

    //On cherche l'indice du bateau expediteur du signal dans la liste en MC
    #if DEBUG
        printf("--> Recherche du bateau dans la liste en MC\n");
    #endif
    indice = bateau_liste_pid2ind(siginfo->si_pid,liste_bateaux,nb_bateaux);

    //Si la recherche nous retourne -1, c'est que le bateau n'a pas été trouvé dans la liste en MC
    //On offre alors à ce dernier la possibilité de s'ajouter à la liste en MC.
    //Pour cela :
    // - On sauvegarde l'état du jeu (pour ne pas perdre les informations concernant les autres bateaux lors du rechargement)
    // - On envoi le signal SIG_INFOS au bateau pour lui dire de s'inscrire dans le fichier en MS
    // - On attend que le bateau nous retourne ce signal
    // - On recharge la liste à partir du fichier en MS
    if( indice == -1)
    {
        #if DEBUG
            printf("--> Bateau non trouve dans la liste\n");
            printf("--> Sauvegarde de la partie\n");
        #endif

        bateau_liste_sauver(fich_bat, liste_bateaux, nb_bateaux, getpid());

	#if DEBUG
            printf("--> Envoi signal SIG_INFOS au bateau\n");
	#endif
	//            ,/
	//          ,'/		L'amiral envoie un signal SIG_INFO pour
	//        ,' /		indiquer au navire qu'il peut s'ajouter
	//      ,'  /_____,     au fichier en MS.
	//    .'____    ,'
	//         /  ,'
	//        / ,'
	//       /,'
	//      /'
        kill(siginfo->si_pid , SIG_INFOS);

        //On attend la confirmation que le bateau s'est bien écrit en MS 
	//(attente d'un signal SIG_INFOS)
        //Ou en cas de defaillance du bateau, le signal SIG_ALARM de fin du timeout pour l'écriture en MS.
        sigset_t masque_attente;	       //Declaration du masque temporaire pour l'attente
        sigfillset(&masque_attente);           //On bloque tous les signaux
        sigdelset(&masque_attente, SIG_INFOS); //On débloque SIG_INFOS
        sigdelset(&masque_attente, SIGALRM);   //On débloque SIGALRM

        //Lancement d'un timeout qui stoppera l'attente si elle n'est
        //pas stoppée par la reception du SIG_INFOS attendu du bateau.
        //Dans TIMEOUT secondes, on poursuivera : évite une attente infinie dans le
        //cas d'une défaillance de la part du bateau.
        alarm(5);

        #if DEBUG
            printf("--> Attente du signal SIG_INFOS de confirmation du bateau ou le timeout\n");
	#endif

	//Attente passive avec comme masque le masque "masque_attente"
        sigsuspend(&masque_attente);

        //Attente terminée, on recharge la liste
        bateau_liste_charger(fich_bat, &liste_bateaux, &nb_bateaux);

        //On récupère à nouveau l'indice qui est à présent présent en MC sauf si problème du côté bateau
        indice = bateau_liste_pid2ind(siginfo->si_pid,liste_bateaux,nb_bateaux);
        if(indice == -1) //En cas d'échec, on sort de la fonction pour ne pas bloquer le jeu
        {
            printf ("[Amiral] Erreur de recuperation des infos du bateau dont le pid est : %d\n", siginfo->si_pid);
            return;
        }
    }

    //A ce stade, nous avons récupéré l'indice du bateau dans la liste en MC.
    //On regarde alors s'il a été initialisé dans la mer (statut actif) ou initialisé puis coulé (cf commentaires l317)
    //et si ce n'est pas le cas, on tente d'initialiser le bateau dans la mer.
    //En cas d'echec, on sort de la fonction (le bateau retentera ultérieurement de s'initialiser)

//	   Vérifie que le bateau n'est pas déjà          Verifie que le bateau n'a pas déjà
//	           initialisé dans la mer                    été initialisé puis coulé
//    \_________________________ _____________________/    \_____________ _____________/
//     			        V                                        V
    if( bateau_actif_lire( liste_bateaux[indice] ) == FAUX &&  tab_energies[indice] != -1 )
    {
        if( mer_bateau_initialiser( fd_mer, &liste_bateaux[indice]) == ERREUR ) //tentative d'initialisation du bateau dans la mer
        {
            #if !CLEARPRINT
                printf ("[Amiral] Impossible d'initialiser le bateau %c dans la mer\n", bateau_marque_lire( liste_bateaux[indice] ) );
	    #endif
            return; //une nouvelle demande d'initialisation sera renvoyée par le navire ultérieurement
        }

	#if !CLEARPRINT
            printf ("[Amiral] Bateau %c initialisé dans la mer\n", bateau_marque_lire( liste_bateaux[indice] ) );
	#endif

        //En cas de réussite de l'initialisation dans la mer, on place le statut du bateau à actif
        bateau_actif_ecrire( &liste_bateaux[indice] , VRAI);

        //Son energie est initialisee au maximum
        tab_energies[indice] = BATEAU_MAX_ENERGIE;

        //On augmente le nombre cumule de bateaux
        cumul_bateaux ++;

        //On affiche la mer
        CLEAR();
        if( mer_afficher(fd_mer) == ERREUR )
        {
            printf ("[Amiral] Impossible d'afficher la mer\n");
            exit(ERREUR) ;
        }

	sleep(1); //Temporisation d'affichage

	return; //On considère qu'une initialisation coûte un tour de jeu
    }


    //Arrivé ici, le bateau peut tenter de jouer
    //(il était ou a été inscrit en MC, son indice à été récupéré, il était ou a été initialisé dans la mer)
    if( tab_energies[indice] >= (5*BATEAU_MAX_ENERGIE)/100 ) //on verifie si le bateau a assez d'energie pour jouer
    {
        /////////////////
        // DEPLACEMENT //
        /////////////////

        coord_liste_creer(&liste_voisins);

	#if DEBUG
            printf("--> Recherche des voisins du bateau %c\n", bateau_marque_lire( liste_bateaux[indice] ) );
	#endif

        if( mer_voisins_rechercher( fd_mer, liste_bateaux[indice], &liste_voisins, &nb_voisins) == ERREUR )
        {
            printf ("[Amiral] Erreur dans la recherche des voisins du du bateau %c\n", bateau_marque_lire( liste_bateaux[indice] ) );
            exit(ERREUR);
        }

        if( mer_bateau_deplacer( fd_mer, &liste_bateaux[indice], liste_voisins, nb_voisins, &ok ) == ERREUR )
        {
            printf ("[Amiral] Erreur durant le déplacement du du bateau %c\n", bateau_marque_lire( liste_bateaux[indice] ) );
            exit(ERREUR);
        }

        coord_liste_detruire(&liste_voisins);

        if( ok ) //Le bateau est parvenu a se déplacer
        {
	    #if !CLEARPRINT
                printf ("[Amiral] Deplacement du bateau %c reussie\n", bateau_marque_lire( liste_bateaux[indice] ) );
	    #endif
            //Un déplacement consomme 5% d'énergie, on decremente l'ernergie du bateau en consequence
            tab_energies[indice] -= (5*BATEAU_MAX_ENERGIE)/100;
        }
        else
        {
	    #if !CLEARPRINT
                printf ("[Amiral] Deplacement du bateau %c impossible\n", bateau_marque_lire( liste_bateaux[indice] ) );
	    #endif
        }

        /////////
        // TIR //
        /////////

        /* Acquisition d'une cible (bateau différent de lui meme) */
        if( mer_bateau_cible_acquerir( fd_mer, liste_bateaux[indice], &acquisition, &cible ) == ERREUR )
        {
            printf ("[Amiral] Erreur durant l'acquisition d'une cible par le bateau %c\n", bateau_marque_lire( liste_bateaux[indice] ) );
            exit(ERREUR);
        }

        if( acquisition == VRAI ) //Le bateau parvient a acquierir une cible
        {
	    #if DEBUG
                printf("--> Cible choisie par le bateau %c\n", bateau_marque_lire( liste_bateaux[indice] ) );
                coord_afficher( stdout , cible );
                printf("\n");
	    #endif

            //On récupère l'indice de la victime
            if( bateau_liste_coord_chercher( cible, liste_bateaux, nb_bateaux, &indice_bateau_cible) == PAS_TROUVE)
            {
                printf ("[Amiral] Erreur durant la recuperation de l'indice du bateau vise par le bateau %c\n", bateau_marque_lire( liste_bateaux[indice] ) );
                exit(ERREUR);
            }


            //Si la victime n'a pas assez d'énergie pour activer son bouclier on rend le tir effectif
            if( tab_energies[indice_bateau_cible] < BATEAU_SEUIL_BOUCLIER )
            {
                //Tir du boulet
                if( mer_bateau_cible_tirer( fd_mer, cible) == ERREUR )
                {
                    printf ("[Amiral] Erreur durant le tir du bateau %c\n", bateau_marque_lire( liste_bateaux[indice] ) );
                    exit(ERREUR);
                }

                //On affiche la mer 1 seconde pour voir le boulet à l'écran
                CLEAR();
                if( mer_afficher(fd_mer) == ERREUR )
                {
                    printf ("[Amiral] Impossible d'afficher la mer\n");
                    exit(ERREUR) ;
                }

                printf("#==============================\n");
                printf("====== Le bateau %c coule ======\n", bateau_marque_lire(liste_bateaux[indice_bateau_cible]));
                printf("#==============================\n");

                sleep(1); //Temporisation d'affichage

                //On coule le bateau touche
                mer_bateau_couler( fd_mer, liste_bateaux[indice_bateau_cible] );

                //On met ses coordonnées à -1.
                //Cela permet d'éviter que ce soit son indice qui soit renvoyé
                //lors d'un futur appel à bateau_liste_coord_chercher avec comme
                //cible l'une des coordonnées où le bateau est mort
                bateau_rm_coord(&liste_bateaux[indice_bateau_cible]);

                //On lui envoie le signal SIG_COULE a la victime pour lui indiquer qu'elle a été coulé
                kill( bateau_pid_lire( liste_bateaux[indice_bateau_cible] ) , SIG_COULE);

                //Par sécurité, on place son énergie à -1
                //A la réception du signal SIG_COULE, le processus du bateau se tue
                //mais il serait dans l'absolu possible qu'un déplacement antérieur
                //ait été placé en attente (signal pendant). Ainsi, en placant l'énergie
                //à -1, on évitera que le bateau ne tente de jouer 1 dernière
                //fois après sa mort
                tab_energies[indice_bateau_cible] = -1;

                bateau_actif_ecrire( &liste_bateaux[indice_bateau_cible] , FAUX );
            }
        }
        else //Pas de cible acquise
        {
	    #if DEBUG
                printf("--> Pas de cible acquise pour le bateau %c\n", bateau_marque_lire( liste_bateaux[indice] ) );
	    #endif
        }

        //Affichage de la mer
        CLEAR();
        if( mer_afficher(fd_mer) == ERREUR )
        {
            printf ("[Amiral] Impossible d'afficher la mer\n");
            exit(ERREUR) ;
        }

        //On vérifie si le bateau est vainqueur
        if (bateau_liste_dernier( liste_bateaux, nb_bateaux, &indice ) == VRAI && cumul_bateaux > 4)
        {
	    //Le bateau est vainqueur
            kill(siginfo->si_pid, SIG_WINNER); //On envoie le signal SIG_WINNER au processus qui gère le bateau

            printf("\n╔═══════════════╗\n");
            printf  ("║ Vainqueur : %c ║ \n" ,bateau_marque_lire(liste_bateaux[indice]) );
            printf  ("╚═══════════════╝\n\n");

            bateau_liste_detruire( &liste_bateaux );
            exit(CORRECT);
        }

    }
    else //le bateau est à court d'energie
    {
        #if !CLEARPRINT
            printf ("[Amiral] Le bateau %c est à court d'energie\n", bateau_marque_lire( liste_bateaux[indice] ) );
	#endif
        //On envoie le signal SIG_NRJ_OUT au processus pour le stopper (il n'a plus le droit d'envoyer des demandes de jeu)
        kill( siginfo->si_pid, SIG_NRJ_OUT);
    }
}


//////////
// MAIN //
//////////

int main( int nb_arg , char * tab_arg[] )
{

    /* Structure sigaction pour une demande de jeu */
    struct sigaction action_jeu;

    /* Structure sigaction pour la reception de la confirmation d'ecriture en MS d'un bateau */
    struct sigaction action_infos;

    /* Structure sigaction pour le signal SIGALRM de timeout */
    struct sigaction action_sigalrm;


    /* Verification du nombre d'arguments */
    if( nb_arg != 3 )
    {
        fprintf( stderr , "Usage : %s <nom fichier mer> <nom fichier bateaux>\n", tab_arg[0] );
        exit(-1) ;
    }

    /* Recopie des parametres */
    strcpy( nomprog  , tab_arg[0] );
    strcpy( fich_mer , tab_arg[1] );
    strcpy( fich_bat , tab_arg[2] );


    /* Renseignement des champs de la structure sigaction pour une demande de jeu */
    action_jeu.sa_sigaction = hdl_jeu;
    action_jeu.sa_flags     = SA_SIGINFO; //Pour récupérer différentes informations dont le PID de l'expediteur du signal
    sigemptyset(&action_jeu.sa_mask);
    sigaddset(&action_jeu.sa_mask, SIG_INFOS); //On ignore un hypothétique signal SIG_INFOS reçu un peu tardivement
    /* Application de la structure sigaction pour une demande de jeu */
    if (sigaction(SIG_JEU, &action_jeu, NULL) < 0)
    {
        perror ("Erreur concernant le sigaction pour le signal SIG_JEU.");
        return 1;
    }


    /* Renseignement des champs de la structure sigaction pour le signal SIGALRM */
    action_sigalrm.sa_handler = hdl_sigalrm;
    action_sigalrm.sa_flags   = 0;
    /* Application de la structure sigaction pour le signal SIGALRM */
    if (sigaction(SIGALRM, &action_sigalrm, NULL) < 0)
    {
        perror ("Erreur concernant le sigaction pour le signal SIG_ALRM.");
        return 1;
    }


    /* Renseignement des champs de la structure sigaction pour la confirmation d'écriture en MS d'un bateau */
    action_infos.sa_handler = hdl_ecriture_ok;
    action_infos.sa_flags   = 0;
    /* Application de la structure sigaction pour la confirmation d'écriture en MS d'un bateau */
    if (sigaction(SIG_INFOS, &action_infos, NULL) < 0)
    {
        perror ("Erreur concernant le sigaction pour le signal SIG_INFOS.");
        return 1;
    }


    /* Ouverture fichier mer */
    if( ( fd_mer = open( fich_mer , O_RDWR , 0644 )) == -1 )
    {
        fprintf( stderr, "%s : Probleme durant l'ouverture sur %s\n", nomprog , fich_mer);
        exit(-1);
    }

    printf("Mer ouverte... \n");


    /* Initialisation de la liste des bateaux en MS */
    bateau_fichier_initialiser(fich_bat);

    /* Ecriture du PID de l'amiral dans la liste des bateaux*/
    if( bateau_fichier_amiral_ecrire( fich_bat, getpid() ) == ERREUR)
    {
        fprintf( stderr, "%s : Probleme durant l'écriture du PID amiral sur %s\n", nomprog , fich_bat);
        exit(-1);
    }

    printf( "\n\n[Amiral] --> Amiral (%d) disponible\n", getpid() );

    //Attente permanante de signaux
    while( 1 )
    {
        pause();
    }
    exit(ERREUR); //Code mort (sortie quand bateau vainqueur)

}

