/*--------------------------------- -*/ 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#define UT_DOS      159              /* ÏÏ */
#define UT_UNIX   207
#define EA_DOS      142               /* ÅÈ */
#define EA_UNIX   197
#define AC_DOS      137              /* À */
#define AC_UNIX  192
#define AG_DOS     136             /* È */
#define AG_UNIX  200
#define CC_DOS     141              /* µ */
#define CC_UNIX  181
#define EC_DOS     144              /* Á */
#define EC_UNIX  193
#define ET_DOS      145             /* Í */
#define ET_UNIX  205
#define EG_DOS     143            /* É */
#define EG_UNIX  201
#define IT_DOS      149            /* Ý */
#define IT_UNIX   221
#define IC_DOS      148            /* Ñ */
#define IC_UNIX   209
#define OC_DOS      153            /* Â */
#define OC_UNIX  194
#define UC_DOS      158           /* Ã */
#define UC_UNIX  195
#define UG_DOS      157           /* Ë */
#define UG_UNIX  203
#define LI_DOS       163          /* » */
#define LI_UNIX    187
#define NU_DOS      161           /* ³ */
#define NU_UNIX   179
/* ------------------- --------------- */ 
    void help (void) {
printf ("-------------------------------------------\n");
printf (" Conversion d'accents MAC -> UNIX -\n");
printf ("    Usage:   reacc_mac2ux fich_mac fich_unix \n");
printf ("-------------------------------------------\n");
}
/* ------------------- --------------- */ 
int main (int argc, char * argv []) {
char  fdicoin [256], fdicout [256];
unsigned char carin, carout;
long int icar;
FILE * dicoin, * dicout;

if (argc != 3) {
   help();
   return 1;
}

strcpy (fdicoin, argv [1]);
printf ("Ouverture fichier entree %s\n", fdicoin);
if ((dicoin = fopen (fdicoin, "r")) == NULL) {
       printf ("Pb ouverture fichier %s\n", fdicoin);
}
 
strcpy (fdicout, argv [2]);
icar = 0;
while ( ((carin = fgetc (dicoin))!=NULL) & (! feof (dicoin))) {
      if (icar == 0) {
	 printf ("Ouverture fichier sortie %s\n", fdicout);
         if ((dicout = fopen (fdicout, "w")) == NULL) {
                printf ("Pb ouverture fichier %s\n", fdicout);
         }
      }
     carout = carin;
     switch (carin) {
           case UT_DOS : carout = UT_UNIX; break;     /* Ï */
           case EA_DOS : carout = EA_UNIX; break;     /* Å */
           case AC_DOS : carout = AC_UNIX; break;     /* À */
           case AG_DOS : carout = AG_UNIX; break;     /* È*/
           case CC_DOS : carout = CC_UNIX; break;     /* µ*/
           case EC_DOS : carout = EC_UNIX; break;     /* Á*/
           case ET_DOS : carout = ET_UNIX; break;     /* Í*/
           case EG_DOS : carout = EG_UNIX; break;     /* É*/
           case IT_DOS : carout = IT_UNIX; break;     /* Ý*/
           case IC_DOS : carout = IC_UNIX; break;     /* Ñ*/
           case OC_DOS : carout = OC_UNIX; break;     /* Â*/
           case UC_DOS : carout = UC_UNIX; break;     /* Ã*/
           case UG_DOS : carout = UG_UNIX; break;     /* Ë */
           case LI_DOS : carout = LI_UNIX; break;     /* » */
           case NU_DOS : carout = NU_UNIX; break;     /* ³ */
           case 131    : carout = 'E';     break;     /* E */
           case 233    : carout = 'E';     break;     /* E */
     } /* switch  */
     fputc (carout, dicout); /*printf("%c %d\n",carin,carin);*/
     icar ++;
 } /* while */
fclose (dicoin);
fclose (dicout);
printf ("- fichier %s converti en %s (%d caractÉres)\n", fdicoin, fdicout, icar);
return 0;
} /* end main */ 
