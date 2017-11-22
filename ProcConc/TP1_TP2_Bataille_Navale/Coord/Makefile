# MAKEFILE for TDA Coord (coordonnees d'une case)
#
#

#
# Directories
#
#top_dir = /lium/buster1/jacob/Enseignements/Licence/L3/Inf310A_SE/TP/TP_Bataille_Navale
top_dir = ..
coord_dir = $(top_dir)/Coord
commun_dir = $(top_dir)/Commun

#
# Object files
#

fich_obj_test_tda_coord = test_tda_coord.o coord.o 

#
# Options for compilation
#

GCCOBJ = gcc -c 
GCCLNK = gcc 

# LINUX ou SOLARIS
opt_SE = -D_LINUX_
#opt_SE = -D_SOLARIS_

# Option for debugger
opt = -g -Wall -D_DEBUG_
# opt = -O2 -Wall
#
#
#



#
#-------- Mains ---------------
#


#
#-------- Utilitaires ---------------
#

#
# --- Tests ---
#

test_tda_coord : $(fich_obj_test_tda_coord)
	$(GCCLNK) $(fich_obj_test_tda_coord) $(opt) $(opt_SE) -o test_tda_coord -I./ -I$(commun_dir)

test_tda_coord.o : test_tda_coord.c coord.h
	$(GCCOBJ) $(opt) $(opt_SE) test_tda_coord.c -o test_tda_coord.o -I./ -I$(commun_dir)

#
#-------- Modules -------------
#

coord.o : coord.c coord.h 
	$(GCCOBJ) $(opt) $(opt_SE) coord.c -o coord.o -I./ -I$(commun_dir)

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

all : test_tda_coord

#
#-------- Make tags  -------
#

tags : 
	etags *.c *.h




