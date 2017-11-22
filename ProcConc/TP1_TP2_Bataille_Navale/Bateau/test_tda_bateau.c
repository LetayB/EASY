#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef _LINUX_
#include <string.h>
#else
#include <strings.h>
#endif

#include <commun.h>
#include <bateau.h>


int
main( int nb_arg , char * tab_arg[] )
{
     bateau_t bateau1 ;
     char fich_bateaux[128];     
     char fich_bateaux_sauvegarde[128];
     int i , n , p; 
     int l , c ;
     off_t pos ;
     int nb_bateaux ;
     int max_bateaux ;
     bateau_t * liste_bateaux = (bateau_t *) NULL ; /* ATTENTION a bien initialiser la liste des bateaux */
     int dernier ;
     pid_t pid_amiral ;
     pid_t pid ;
     int ind ;
     coord_t coord ;
     coord_t * pt_corps ;
     int ind_bateau ;

     /*----------*/

     if( nb_arg != 3 )
     {
	  fprintf( stderr , "Usage : %s <fichier bateaux> <n>\n",
		   tab_arg[0] );
	  exit(-1) ;
     }

     strcpy( fich_bateaux , tab_arg[1]) ;
     sscanf( tab_arg[2] , "%d" , &n ) ;
     
     srandom( (unsigned int)getpid() );
     
     /* Initialisation fichier */
     bateau_fichier_initialiser( fich_bateaux ) ;

     /* Test ecriture pid processus bateau amiral dans le fichier des bateaux */
     bateau_fichier_amiral_ecrire( fich_bateaux, getpid()) ;

     /* Test ajout de bateaux dans le fichier */
     l = 0 ;
     c = 1 ;
     pos = 2 ;
     bateau1.marque = 'A'+n ;
     bateau1.pid = n+100 ;
     max_bateaux = (int)( random()%10 ) + 1 ;
     for( i=0 ; i<max_bateaux ; i++ )
     {
	  for( p=0 ; p<BATEAU_TAILLE ; p++ )
	  {
	       bateau1.corps[p].l= l++;
	       bateau1.corps[p].c = c++;
	       bateau1.corps[p].pos = pos++;
	  }
	  
	  bateau1.marque++  ;
	  bateau1.pid++ ;
	  bateau1.actif =  (bateau1.actif+1)%2 ;
	  bateau_fichier_ajouter( fich_bateaux, bateau1 ) ;
     }

     /* Test affichage fichier des bateaux */
     printf("\nAffichage du fichier des bateaux\n");
     bateau_fichier_afficher(fich_bateaux);

     /* Test lecture du pid du processus "bateau amiral" */
     bateau_fichier_amiral_lire(fich_bateaux, &pid_amiral);
#ifdef _LINUX_
     printf("\nLe pid du processus bateau amiral dans le fichier des bateaux est %d\n",
	    pid_amiral ) ;
#else
     printf("\nLe pid du processus bateau amiral dans le fichier des bateaux est %ld\n",
	    pid_amiral ) ;
#endif
     
     /* Test chargement fichier dans liste de bateaux */
     bateau_liste_charger( fich_bateaux, &liste_bateaux, &nb_bateaux) ;

     /* Test sauvegarde liste de bateaux dans fichier */
     strcpy( fich_bateaux_sauvegarde , fich_bateaux ) ;
     strcat( fich_bateaux_sauvegarde , "_sauv" ) ;
     bateau_liste_sauver( fich_bateaux_sauvegarde , liste_bateaux , nb_bateaux , pid_amiral ) ;

     /* Test affichage d'une liste de bateaux */    
     printf( "\nAffichage de la liste des %d bateaux\n", nb_bateaux);
     bateau_liste_afficher( liste_bateaux, nb_bateaux) ;

     /* Test conversion pid --> num de bateau */
     pid = 100+n+(random()%max_bateaux);
#ifdef _LINUX_
     printf( "\nLe pid numero %d correspond au bateau numero %d dans la liste des bateaux\n",
	     pid , bateau_liste_pid2ind( pid , liste_bateaux, nb_bateaux)) ;
#else
     printf( "\nLe pid numero %ld correspond au bateau numero %d dans la liste des bateaux\n",
	     pid , bateau_liste_pid2ind( pid , liste_bateaux, nb_bateaux)) ;
#endif

     /* Test conversion num de bateau --> pid */
     i = random()%max_bateaux ;
#ifdef _LINUX_
     printf( "\nLe bateau numero %d correspond au pid numero %d dans la liste des bateaux\n",
	     i , bateau_liste_ind2pid( i , liste_bateaux , nb_bateaux ) ) ;
#else
     printf( "\nLe bateau numero %d correspond au pid numero %ld dans la liste des bateaux\n",
	     i , bateau_liste_ind2pid( i , liste_bateaux , nb_bateaux ) ) ;
#endif

     /* Test etat des bateaux */    
     if( bateau_liste_actifs( liste_bateaux , nb_bateaux ) )
     { 
	  printf(" Il y a au moins un bateau actif dans la liste\n");
     }

     if( bateau_liste_dernier(liste_bateaux, nb_bateaux, &dernier) )
     {
	  printf(" Il n'y a qu'un bateau actif dans la liste et son indice dans la liste est %d\n" ,
		 dernier );
	  
     }
     
     /* Test rechercher a quel bateau appartient une coordonnee */
     ind = (int)(random()%nb_bateaux) ;
     pt_corps = bateau_corps_lire( &(liste_bateaux[ind]) ) ;
     ind = (int)(random()%BATEAU_TAILLE) ;
     coord_affecter( &coord , *(pt_corps+ind) ) ;
     if( bateau_liste_coord_chercher( coord ,
				      liste_bateaux,
				      nb_bateaux,
				      &ind_bateau ) == PAS_TROUVE )
     {
	  printf(" La coordonnee " );
	  coord_afficher( stdout , coord );
	  printf(" n'a pas ete trouvee dans la liste des bateaux \n");
	  bateau_liste_afficher( liste_bateaux, 
				 nb_bateaux );
     }
     else
     {
	  printf(" La coordonnee " );
	  coord_afficher( stdout , coord );
	  printf( " appartient au bateau \n");
	  bateau_afficher( stdout , 
			   liste_bateaux[ind_bateau] );
     }
     printf("\n\n");

     coord_ecrire( &coord , 0 , 0 , 0 );
     if( bateau_liste_coord_chercher( coord ,
				      liste_bateaux,
				      nb_bateaux,
				      &ind_bateau ) == PAS_TROUVE )
     {
	  printf(" La coordonnee " );
	  coord_afficher( stdout , coord );
	  printf(" n'a pas ete trouvee dans la liste des bateaux \n");
	  bateau_liste_afficher( liste_bateaux, 
				 nb_bateaux );
     }
     else
     {
	  printf(" La coordonnee " );
	  coord_afficher( stdout , coord );
	  printf( " appartient au bateau \n");
	  bateau_afficher( stdout , 
			   liste_bateaux[ind_bateau] );
     }
     printf("\n\n");


     exit(0); 
}
