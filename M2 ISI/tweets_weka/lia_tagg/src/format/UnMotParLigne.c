
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/*................................................................*/

#define TailleLigne     40000

#define True    1
#define False   0

void ERREUR(char *ch1,char *ch2)
{
fprintf(stderr,"ERREUR : %s %s\n",ch1,ch2);
exit(0);
}

/*................................................................*/

int main(int argc, char **argv)
{
char ch[TailleLigne],*pt;
int i,cut_tag;

cut_tag=False;
if ((argc>1)&&(!strcmp(argv[1],"-cut_tag"))) cut_tag=True;

if (!cut_tag)
 for(;fgets(ch,TailleLigne,stdin);)
  for(pt=strtok(ch," \t\n");pt;pt=strtok(NULL," \t\n")) printf("%s\n",pt);
else
 for(;fgets(ch,TailleLigne,stdin);)
  for(pt=strtok(ch," \t\n");pt;pt=strtok(NULL," \t\n"))
   {
   for(i=0;pt[i];i++)
    {
    if ((pt[i]=='<')&&(i>0)) printf("\n");
    printf("%c",pt[i]);
    if ((pt[i]=='>')&&(pt[i+1])) printf("\n");
    }
   printf("\n");
   }
}
  
