#---------------------------------------------------------------------------#
#                                                                           #
#									    #
#									    #
#                                 MAKE FILE			            #
# 									    #
#									    #
#									    #
#									    #			    
#___________________________________________________________________________#
CC = gcc
CFLAGS = -c -Wall
FLAGS = -o
INCLUDE = -I../include/
HEADER = ../include/
OBJ = ../obj/
SRC = ../src/
OUTPUT = ../bin/
DFLAGS = -g 

$(OUTPUT)client : $(OBJ)client_main.o $(OBJ)client_functions.o $(OBJ)client_error.o
	$(CC) $(FLAGS) client $(OBJ)client_main.o $(OBJ)client_functions.o $(OBJ)client_error.o
	mv client $(OUTPUT)

$(OBJ)client_main.o : $(SRC)client_main.c $(HEADER)client_header.h
	$(CC) $(DFLAGS) $(CFLAGS) $(SRC)client_main.c $(INCLUDE)
	mv client_main.o $(OBJ)

$(OBJ)client_functions.o : $(SRC)client_functions.c $(HEADER)client_header.h
	$(CC) $(DFLAGS) $(CFLAGS) $(SRC)client_functions.c $(INCLUDE)
	mv client_functions.o $(OBJ)

$(OBJ)client_error.o : $(SRC)client_error.c $(HEADER)client_header.h
	$(CC) $(DFLAGS) $(CFLAGS) $(SRC)client_error.c $(INCLUDE)
	mv client_error.o $(OBJ)

val: 
	valgrind $(VFLAGS) $(OUTPUT)client


clean:
	rm $(OBJ)*.o
	rm $(OUTPUT)client $(OUTPUT)server

