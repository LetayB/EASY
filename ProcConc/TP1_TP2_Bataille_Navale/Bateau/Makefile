# MAKEFILE for TDA Bateau
#
#

#
# Directories
#
#top_dir = /lium/buster1/jacob/Enseignements/Licence/L3/Inf310A_SE/TP/TP_Bataille_Navale
top_dir = ..
commun_dir = $(top_dir)/Commun
coord_dir = $(top_dir)/Coord
bateau_dir =  $(top_dir)/Bateau

#
# Object files
#

fich_obj_init_fich_bateaux = init_fich_bateaux.o bateau.o $(coord_dir)/coord.o
fich_obj_test_tda_bateau = test_tda_bateau.o bateau.o $(coord_dir)/coord.o
fich_obj_kill_bateaux = kill_bateaux.o bateau.o $(coord_dir)/coord.o


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

init_fich_bateaux : $(fich_obj_init_fich_bateaux)
	$(GCCLNK) $(fich_obj_init_fich_bateaux) $(opt) $(opt_SE) -o init_fich_bateaux -I$(commun_dir) -I$(bateau_dir) -I$(coord_dir)

init_fich_bateaux.o : init_fich_bateaux.c 
	$(GCCOBJ) $(opt) $(opt_SE) init_fich_bateaux.c -o init_fich_bateaux.o -I$(commun_dir) -I$(bateau_dir) -I$(coord_dir)

kill_bateaux : $(fich_obj_kill_bateaux)
	$(GCCLNK) $(fich_obj_kill_bateaux) $(opt) $(opt_SE) -o kill_bateaux -I$(commun_dir) -I$(bateau_dir) -I$(coord_dir)

kill_bateaux.o : kill_bateaux.c bateau.h
	$(GCCOBJ) $(opt) $(opt_SE) kill_bateaux.c -o kill_bateaux.o -I$(commun_dir) -I$(bateau_dir) -I$(coord_dir)

#
# --- Tests ---
#

test_tda_bateau : $(fich_obj_test_tda_bateau)
	$(GCCLNK) $(fich_obj_test_tda_bateau) $(opt) $(opt_SE) -o test_tda_bateau -I$(commun_dir) -I$(coord_dir) -I$(bateau_dir) 

test_tda_bateau.o : test_tda_bateau.c bateau.h
	$(GCCOBJ) $(opt) $(opt_SE) test_tda_bateau.c -o test_tda_bateau.o -I$(commun_dir) -I$(coord_dir) -I$(bateau_dir)

#
#-------- Modules -------------
#

bateau.o : bateau.c bateau.h  
	$(GCCOBJ) $(opt) $(opt_SE) bateau.c -o bateau.o -I$(commun_dir) -I$(coord_dir) -I./

$(coord_dir)/coord.o : 
	cd $(coord_dir) ; make -f Makefile coord.o ; cd $(bateau_dir)

#
#-------- Headers -------------
#

bateau.h : $(commun_dir)/commun.h $(coord_dir)/coord.h
	touch bateau.h

#
#-------- Clean objects -------
#

clean : 
	rm -f *.o 


#
#-------- All executables -------
#

all : init_fich_bateaux kill_bateaux test_tda_bateau

#
#-------- Make tags  -------
#

tags : 
	etags *.c *.h
