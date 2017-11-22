/*----------------------------------------------------------------------

   Nom         : visison.c

   Description : affiche le fichier mer passé en paramètre dès
		 qu'une modification a été détéctée dans ce dernier

   Usage       : ./vision.c <fichier mer>

   Auteur : FROGER Adrien - L3 SPI - groupe 2

/--------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <mer.h>


/*
* Permet de tester la présence d'un verrou sur le header du fichier mer signifiant
* qu'un déplacement est en cours.
*
* Retourne 1 si le header est verouillé, 2 le cas contraire
*/
int verif_verrou_header(int fd_mer)
{
    struct flock verrou;

    verrou.l_whence = (short)0;
    verrou.l_start = (long)0;
    verrou.l_len = MER_TAILLE_ENTETE;

    fcntl(fd_mer, F_GETLK, &verrou);

    return (verrou.l_type == F_UNLCK) ? 1 : 0;
}




int main( int nb_arg , char * tab_arg[] )
{
    char nomprog[128] ;
    char fich_mer[128];
    int fd_mer;

    int nbBateaux;

    struct stat infos_fichier_mer; //structure pour contenir les infos relatives au fichier mer
    time_t date_derniere_modif;    //date de la dernière modification 
				   //(nombre de secondes ecoulées depuis le 1 janv. 1970 00H00m00)

    /* Contrôle du nombre de paramètres */
    if( nb_arg != 2 )
    {
        fprintf( stderr , "Usage : %s <nom fichier mer>\n",
                 tab_arg[0] );
        exit(-1) ;
    }

    /* Recopie des informations passées en paramètre */
    strcpy( nomprog  , tab_arg[0] );
    strcpy( fich_mer , tab_arg[1] );


    /* Ouverture fichier mer */
    if( ( fd_mer = open( fich_mer , O_RDWR , 0644 )) == -1 )
    {
        fprintf( stderr, "%s : Probleme durant l'ouverture sur %s\n", nomprog , fich_mer);
        exit(-1);
    }
    printf("Mer ouverte");


    /* Récupération de la date de dernière modification */

    //on récupère les informations relatives au fichier dans infos_fichier_mer
    stat (fich_mer,&infos_fichier_mer); 
    //on "sauvegarde" le champ relatif à la date de derrnière modif dans date_derniere_modif
    date_derniere_modif = infos_fichier_mer.st_ctime; 


    /* Boucle infinie affichant la mer à chaque changement */
    while(1)
    {
        //on récupère les informations relatives au fichier dans infos_fichier_mer
        stat (fich_mer,&infos_fichier_mer);

        //Si cette dernière est différente de celle précedement "sauvegardee" dans date_derniere_modif,
        //on la "sauvegarde" (on actualise date_derniere_modif) et on affiche la mer.
        if(date_derniere_modif != infos_fichier_mer.st_ctime)
        {
            date_derniere_modif = infos_fichier_mer.st_ctime;

            system ("clear"); //fait un "clear" du terminal

	    /* Si un déplacement est en cours, on attend qu'il se termine */
	    while(verif_verrou_header(fd_mer))

            printf("Affichage de la mer: \n");
	    mer_afficher(fd_mer);

            mer_nb_bateaux_lire( fd_mer , &nbBateaux);
            printf("Nombre de bateaux : %i\n", nbBateaux);
        }
    }


    close(fd_mer); //code mort
    return 0;       //code mort

}
