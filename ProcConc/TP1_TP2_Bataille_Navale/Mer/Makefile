# MAKEFILE for TDA Mer in TP Bataille Navale
#
#

#
# Directories
#
#top_dir = /lium/buster1/jacob/Enseignements/Licence/L3/Inf310A_SE/TP/TP_Bataille_Navale
top_dir = ..
commun_dir = $(top_dir)/Commun
mer_dir = $(top_dir)/Mer
coord_dir = $(top_dir)/Coord
bateau_dir = $(top_dir)/Bateau

#
# Object files
#

fich_obj_test_tda_mer = test_tda_mer.o mer.o $(coord_dir)/coord.o $(bateau_dir)/bateau.o
fich_obj_init_fich_mer = init_fich_mer.o mer.o $(bateau_dir)/bateau.o $(coord_dir)/coord.o

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

init_fich_mer : $(fich_obj_init_fich_mer)
	$(GCCLNK) $(fich_obj_init_fich_mer) $(opt) $(opt_SE) -o init_fich_mer -I$(commun_dir) -I$(mer_dir) -I$(bateau_dir) -I$(coord_dir)

init_fich_mer.o : init_fich_mer.c mer.h
	$(GCCOBJ) $(opt) $(opt_SE) init_fich_mer.c -o init_fich_mer.o -I$(commun_dir) -I$(mer_dir) -I$(bateau_dir) -I$(coord_dir)

#
# --- Tests ---
#

test_tda_mer : $(fich_obj_test_tda_mer)
	$(GCCLNK) $(fich_obj_test_tda_mer) $(opt) $(opt_SE) -o test_tda_mer -I$(commun_dir) -I$(mer_dir) -I$(bateau_dir)

test_tda_mer.o : test_tda_mer.c $(commun_dir)/commun.h $(coord_dir)/coord.h mer.h
	$(GCCOBJ) $(opt) $(opt_SE) test_tda_mer.c -o test_tda_mer.o -I$(commun_dir) -I$(mer_dir) -I$(coord_dir) -I$(bateau_dir)

#
#-------- Modules -------------
#

mer.o : mer.c mer.h 
	$(GCCOBJ) $(opt) $(opt_SE) mer.c -o mer.o -I$(commun_dir) -I$(mer_dir) -I$(coord_dir) -I$(bateau_dir)

$(coord_dir)/coord.o :
	cd $(coord_dir) ; make -f Makefile coord.o ; cd $(mer_dir) 

$(bateau_dir)/bateau.o :
	cd $(bateau_dir) ; make -f Makefile bateau.o ; cd $(mer_dir) 



#
#-------- Headers -------------
#

mer.h : $(commun_dir)/commun.h $(coord_dir)/coord.h $(bateau_dir)/bateau.h
	touch mer.h

#
#-------- Clean objects -------
#

clean : 
	rm -f *.o 


#
#-------- All executables -------
#

all : init_fich_mer test_tda_mer 

#
#-------- Make tags  -------
#

tags : 
	etags *.c *.h

lint :
	lint test_tda_mer.c mer.c $(coord_dir)/coord.c $(bateau_dir)/bateau.c -I$(commun_dir) -I$(mer_dir) -I$(coord_dir) -I$(bateau_dir)
