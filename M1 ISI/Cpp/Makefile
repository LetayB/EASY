CC = g++
CFLAGS = -Wall
EXEC_NAME = td1
INCLUDES = 
LIBS =
OBJ_FILES = td1.o
INSTALL_DIR = /usr/bin
 
all : $(EXEC_NAME)
 
clean :
	rm $(OBJ_FILES) $(EXEC_NAME) 
 
$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)
 
main.o: main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<