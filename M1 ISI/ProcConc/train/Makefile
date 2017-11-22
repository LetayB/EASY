# MAKEFILE for TP Moniteur
#
#

#
# Directories
#


#
# Macros Object files
#

#--- mains

fich_obj_ligne = ligne.o moniteur_voie_unique.o sens.o train.o trajet.o

#
# Options for compilation
#
#
GCCOBJ = gcc -c -I.  
GCCLNK = gcc -I.  

#
# Libraries UNIX
#
#
LIB_PTHREAD = -lpthread
MACRO_PTHREAD = -D_REENTRANT

#
# Libraries LINUX 
#
#
# LIB_PTHREAD = 
# MACRO_PTHREAD = 

# 
# Options for debugger
#
opt = -g -Wall -DDEBUG
# opt = -g -Wall 
# opt = -Wall 
# opt = -O2 -Wall 
#


#
#-------- Mains ---------------
#

ligne : $(fich_obj_ligne)
	$(GCCLNK) $(fich_obj_ligne) $(opt) $(LIB_PTHREAD) $(MACRO_PTHREAD) -o ligne 

ligne.o : ligne.c 
	$(GCCOBJ) $(opt)  ligne.c -o ligne.o 

#
#-------- Modules -------------
#

moniteur_voie_unique.o : moniteur_voie_unique.c moniteur_voie_unique.h sens.h
	$(GCCOBJ) $(opt)  moniteur_voie_unique.c -o moniteur_voie_unique.o 

sens.o : sens.c sens.h 
	$(GCCOBJ) $(opt)  sens.c -o sens.o 

train.o : train.c train.h sens.h
	$(GCCOBJ) $(opt)  train.c -o train.o 

trajet.o : trajet.c trajet.h sens.h
	$(GCCOBJ) $(opt)  trajet.c -o trajet.o 

#
#-------- Clean objects -------
#

clean : 
	rm -f *.o 

#
#-------- Distrib -------
#

distrib : 
	rm -f *~
	rm -f  TP_Chemin_Fer.tar.gz
	tar cvf TP_Chemin_Fer.tar *.[ch] Makefile
	gzip TP_Chemin_Fer.tar

#
#-------- All executables -------
#

all : ligne 

tags :
	etags *.c *.h 
