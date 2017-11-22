/*----------------------------------------------------------------------

   Nom         : navire.c

   Description : processus indépendant représentant un navire

   Usage       : ./navire <nom fichier bateaux> <marque bateau>

   Auteur      : FROGER Adrien - L3 SPI - groupe 2

/--------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include <bateau.h>

#include "signaux.h" //Correspondance nom__signal_perso <--> nom officiel



////////////////////////
/* Variables globales */
////////////////////////

char fich_bat[128]; //Référence (chemin + nom + extension) du fichier bateaux
pid_t pid_amiral;   //PID de l'Amiral
bateau_t bateau;    //Bateau créé par le processus




//////////////
/* Handlers */
//////////////

//Handler déclenché à la réception du signal SIG_INFOS. Ce signal informe le navire que l'amiral l'invite à inscrire
//les informations le  concernant dans le fichier des  bateaux en MS. Le navire  inscrit alors ses informations dans
//le fichier et renvoie le  signal SIG_INFOS pour signaler à l'amiral qu'il à terminé de s'inscrire dans le fichier.
static void hdl_ecriture_ms(int signal)
{
    printf("=[ Bateau %c ]=> Ecriture des infos du bateau dans le fichier en MS\n", bateau_marque_lire(bateau) );

    /* Le bateau s'ajoute dans le fichier en MS */
    if( bateau_fichier_ajouter( fich_bat, bateau ) != 0 )
    {
        printf("Erreur durant l'inscription du bateau %c dans le fichier en MS\n", bateau_marque_lire(bateau));
        exit(ERREUR);
    }

    /* Le bateau signale qu'il a fini d'écrire dans le fichier en MS */
    printf("=[ Bateau %c ]=> Fin de l'écriture dans le fichier en MS\n", bateau_marque_lire(bateau) );
    //            ,/
    //          ,'/	    Le  navire  envoie  le  signal  SIG_INFOS
    //        ,' /	    à l'amiral  pour  lui  indiquer  qu'il  a
    //      ,'  /_____,     terminé l'inscription de ses informations 
    //    .'____    ,'      dans le fichier en MS.
    //         /  ,'
    //        / ,'
    //       /,'
    //      /'
    kill( pid_amiral , SIG_INFOS );
}



//Handler permettant d'aborder la fin du jeu. Ce handler est appellé dans plusieurs cas:
// * Réception d'un signal SIG_COULE   : le bateau affiche un message approprié et le processus se termine
// * Réception d'un signal SIG_NRJ_OUT : le bateau affiche qu'il n'a plus d'énergie et attend la réception d'un signal SIG_COULE
// * Réception d'un signal SIG_WINNER  : le bateau affiche un message approprié et le processus se termine
static void hdl_arret_de_jeu(int signal)
{
    if(signal == SIG_COULE)
    {
        printf("=[ Bateau %c ]=> Le bateau a été coulé\n", bateau_marque_lire(bateau) );
    }
    else if(signal == SIG_NRJ_OUT)
    {
        printf("=[ Bateau %c ]=> Le bateau n'a plus d'énergie\n", bateau_marque_lire(bateau) );
        //Arrivé ici, le bateau n'a plus d'énergie. On attend alors qu'un autre bateau le coule.
        //pause() n'attend pas un signal en particulier mais arrivé à ce moment, 
	//le seul signal que peut recevoir le navire est le signal SIG_COULE.
        pause();
    }
    else if(signal == SIG_WINNER)
    {
        printf("=[ Bateau %c ]=> Bateau Vainqueur !!\n", bateau_marque_lire(bateau) );
    }
    else
    {
        fprintf(stderr, "Signal d'arrêt de jeu non recconu\n");
        exit(ERREUR);
    }

    exit(CORRECT);
}





//////////
// MAIN //
//////////

int main( int nb_arg , char * tab_arg[] )
{

    char marque ; //Marque du bateau

    struct sigaction action_ecriture_MS;


    //========[ Action écriture MS (SIG_INFOS) ]=======
    action_ecriture_MS.sa_handler = hdl_ecriture_ms;
    action_ecriture_MS.sa_flags   = 0;
    if (sigaction(SIG_INFOS, &action_ecriture_MS, NULL) < 0)
    {
        perror ("Erreur concernant le sigaction pour le signal SIG_INFOS");
        return 1;
    }


    //========[ Action arrêt de jeu (SIG_COULE, SIG_NRJ_OUT ou SIG_WINNER) ]=======
    //Signal() ne rétablit pas le handleur utilisateur à la fin de l'excécution de ce dernier
    //mais c'est sans importance car le handler hdl_arret_de_jeu aboutit sur un exit du processus
    signal(SIG_NRJ_OUT , hdl_arret_de_jeu);
    signal(SIG_COULE   , hdl_arret_de_jeu);
    signal(SIG_WINNER  , hdl_arret_de_jeu);



    /* Vérification du nombre d'arguments */
    if( nb_arg != 3 )
    {
        fprintf( stderr , "Usage : %s <nom fichier bateaux> <marque bateau>\n", tab_arg[0] );
        exit(-1) ;
    }

    /* Recopie des paramètres */
    strcpy( fich_bat , tab_arg[1] );
    marque = *( tab_arg[2] );

    /* Récupération du PID de l'Amiral */
    if( bateau_fichier_amiral_lire( fich_bat , &pid_amiral ) == ERREUR)
    {
        fprintf(stderr, "=[ Bateau %c ] Erreur durant la récupération du PID de l'amiral\n", bateau_marque_lire(bateau) );
	exit(ERREUR);
    }

    /* Initialisation des champs du bateau */
    bateau_marque_ecrire( &bateau , marque );
    bateau_pid_ecrire( &bateau , getpid() );
    bateau_actif_ecrire( &bateau , FAUX );

    /* Initialisation de la fonction random */
    srandom(getpid()) ;


    /* Lancement de la phase de jeu */
    // (envoi perpetuel du signal SIG_JEU avec attente aléatoire entre chaque envoi)

    //            ,/
    //          ,'/		Tant  que le processus  du navire n'est pas  mis  en
    //        ,' /		pause  par  SIG_NRJ_OUT ou quitté par  SIG_COULE  ou 
    //      ,'  /_____,     par  SIG_WINNER,  il  demande  à  jouer  à  l'amiral
    //    .'____    ,'
    //         /  ,'
    //        / ,'
    //       /,'
    //      /'
    while( 1 )
    {

        printf("=[ Bateau %c ]=> Le bateau demande à jouer\n", bateau_marque_lire(bateau) );
        //Attente aléatoire de 1 à 5 secondes entre chaque demande
        sleep(1 + (5-1)*((float)(rand()) / (float)(RAND_MAX)));
        if( kill(pid_amiral, SIG_JEU) != 0)
	{      
            fprintf(stderr, "Erreur : Impossible de contacter l'amiral\n");
            exit(ERREUR);
	}
    }

    exit(ERREUR); //Code mort (sortie prévue par le handler arret_de_jeu)
}
