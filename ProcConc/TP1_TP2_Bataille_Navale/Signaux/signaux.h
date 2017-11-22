/*//////////////////////////////
// Liste des signaux employés //
//////////////////////////////*/

// Fichier permettant d'effectuer la correspondance entre nom personnalisé 
// et nom "officiel" pour les signaux employés par la solution.

//  /!\ Attention : ne pas utiliser le signal SIGALRM


//=== SIG_INFOS ===
// - Amiral > Navire : demande au navire de s'inscrire dans le fichier en MS
// - Navire > Amiral : confirme à l'amiral la bonne inscripton dans le fichier en MS
#define SIG_INFOS     SIGUSR2


//=== SIG_JEU ===
// - Navire > Amiral : indique à l'amiral le souhait de jouer (avec initialisation si nécessaire)
#define SIG_JEU       SIGUSR1


//=== SIG_COULE ===
// - Amiral > Navire : indique au navire qu'il a été coulé
#define SIG_COULE     SIGTRAP


//=== SIG_NRJ_OUT ===
// - Amiral > Navire : indique au navire qu'il n'a plus d'énergie
#define SIG_NRJ_OUT   SIGABRT


//=== SIG_WINNER ===
// - Amiral > Navire : indique au navire qu'il est le gagnant
#define SIG_WINNER    SIGINT


