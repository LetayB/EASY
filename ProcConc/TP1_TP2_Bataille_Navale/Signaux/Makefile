# MAKEFILE for TP Bataille Navale 
# version Signaux
#
#

#
# Directories
#
top_dir = ..
commun_dir = $(top_dir)/Commun
coord_dir = $(top_dir)/Coord
bateau_dir = $(top_dir)/Bateau
mer_dir = $(top_dir)/Mer
signaux_dir = $(top_dir)/Signaux

#
# Object files
#

fich_obj_navire = navire.o $(mer_dir)/mer.o $(bateau_dir)/bateau.o $(coord_dir)/coord.o
fich_obj_amiral = amiral.o $(mer_dir)/mer.o $(bateau_dir)/bateau.o $(coord_dir)/coord.o


#
# Options for compilation
#

GCCOBJ = gcc -c 
GCCLNK = gcc 


# Option for debugger
opt = -g -Wall -D_DEBUG_
#opt = -O2 -Wall
#
#
#



#
#-------- Mains ---------------
#

navire : $(fich_obj_navire)
	$(GCCLNK) $(fich_obj_navire) $(opt) -o navire -I./ -I$(commun_dir) -I$(coord_dir) -I$(bateau_dir) -I$(mer_dir)

navire.o : navire.c $(commun_dir)/commun.h $(coord_dir)/coord.h $(bateau_dir)/bateau.h $(mer_dir)/mer.h
	$(GCCOBJ) $(opt) navire.c -o navire.o -I./ -I$(commun_dir) -I$(coord_dir) -I$(bateau_dir) -I$(mer_dir)

amiral : $(fich_obj_amiral)
	$(GCCLNK) $(fich_obj_amiral) $(opt) -o amiral -I$(commun_dir) -I$(mer_dir)

amiral.o : amiral.c $(commun_dir)/commun.h $(mer_dir)/mer.h
	$(GCCOBJ) $(opt) amiral.c -o amiral.o -I$(commun_dir) -I$(coord_dir) -I$(bateau_dir) -I$(mer_dir)

#
# --- Tests ---
#


#
#-------- Modules -------------
#

$(mer_dir)/mer.o : 
	cd $(mer_dir) ; make -f Makefile mer.o ; cd $(signaux_dir)

$(bateau_dir)/bateau.o : 
	cd $(bateau_dir) ; make -f Makefile bateau.o ; cd $(signaux_dir)

$(coord_dir)/coord.o : 
	cd $(coord_dir) ; make -f Makefile coord.o ; cd $(signaux_dir)

#
#-------- Headers -------------
#

#
#-------- Clean objects -------
#

clean : 
	rm -f *.o 


#
#-------- All executables -------
#

all : navire amiral

#
#-------- All tests -------
#

#
#-------- Tags -------
#

tags :
	etags *.c 
#
#------- Program verifier ----------
#

lint :
	lint amiral.c $(coord_dir)/coord.c $(bateau_dir)/bateau.c $(mer_dir)/mer.c -I$(commun_dir) -I$(mer_dir) -I$(coord_dir) -I$(bateau_dir)
