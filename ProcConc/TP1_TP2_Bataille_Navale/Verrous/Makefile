# MAKEFILE for TP Bataille Navale 
# version Verrous
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
verrou_dir = $(top_dir)/Verrous

#
# Object files
#

fich_obj_navire = navire.o $(mer_dir)/mer.o $(bateau_dir)/bateau.o $(coord_dir)/coord.o
fich_obj_vision = vision.o $(mer_dir)/mer.o $(bateau_dir)/bateau.o $(coord_dir)/coord.o


#
# Options for compilation
#

GCCOBJ = gcc -c 
GCCLNK = gcc 


# Option for debugger
opt = -g -Wall -D_DEBUG_
# opt = -O2 -Wall
#
#
#



#
#-------- Mains ---------------
#

navire : $(fich_obj_navire)
	$(GCCLNK) $(fich_obj_navire) $(opt) -o navire -I./ -I$(commun_dir) -I$(coord_dir) -I$(bateau_dir) -I$(mer_dir)

navire.o : navire.c $(commun_dir)/commun.h $(coord_dir)/coord.h $(bateau_dir)/bateau.h $(mer_dir)/mer.h
#navire.o : navire.c navire.h $(commun_dir)/commun.h $(coord_dir)/coord.h $(bateau_dir)/bateau.h $(mer_dir)/mer.h
	$(GCCOBJ) $(opt) navire.c -o navire.o -I./ -I$(commun_dir) -I$(coord_dir) -I$(bateau_dir) -I$(mer_dir)

vision : $(fich_obj_vision)
	$(GCCLNK) $(fich_obj_vision) $(opt) -o vision -I$(commun_dir) -I$(mer_dir)

vision.o : vision.c $(commun_dir)/commun.h $(mer_dir)/mer.h
	$(GCCOBJ) $(opt) vision.c -o vision.o -I$(commun_dir) -I$(coord_dir) -I$(bateau_dir) -I$(mer_dir)

#
# --- Tests ---
#


#
#-------- Modules -------------
#

$(mer_dir)/mer.o : 
	cd $(mer_dir) ; make -f Makefile mer.o ; cd $(verrou_dir)

$(bateau_dir)/bateau.o : 
	cd $(bateau_dir) ; make -f Makefile bateau.o ; cd $(verrou_dir)

$(coord_dir)/coord.o : 
	cd $(coord_dir) ; make -f Makefile coord.o ; cd $(verrou_dir)

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

all : navire vision

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
	lint navire.c $(coord_dir)/coord.c $(bateau_dir)/bateau.c $(mer_dir)/mer.c -I$(commun_dir) -I$(mer_dir) -I$(coord_dir) -I$(bateau_dir)
