CC = g++
CFLAGS = -Wall
EXEC_NAME = t
INCLUDES =
LIBS =
OBJ_FILES = main.o Temps.o Coureur.o Commissaire.o
INSTALL_DIR = /usr/bin

all : $(EXEC_NAME)

clean :
	rm $(OBJ_FILES) $(EXEC_NAME) 

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

Temps.o:  Temps.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

Coureur.o:  Coureur.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

Commissaire.o:  Commissaire.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

main.o: main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<