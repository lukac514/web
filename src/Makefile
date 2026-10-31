

OBJ=web
SRC=main.c net.c sqlite.c
CC=gcc
INC=../include
FLAGS=-lsqlite3

$(OBJ):$(SRC)
	$(CC) $^ -o $@ -I$(INC) $(FLAGS)

clean:
	rm $(OBJ)