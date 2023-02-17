OUT_DIR = obj
OBJS	= ${OUT_DIR}/main.o ${OUT_DIR}/bst.o ${OUT_DIR}/btree.o
SOURCE	= main.c malloc_structs/bst.c malloc_structs/btree.c
HEADER	= malloc_structs/malloc_structs.h
OUT	= mso
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

obj/main.o: main.c
	$(CC) $(FLAGS) main.c  -o ${OUT_DIR}/main.o

obj/bst.o: malloc_structs/bst.c
	$(CC) $(FLAGS) malloc_structs/bst.c -o ${OUT_DIR}/bst.o

obj/btree.o: malloc_structs/btree.c
	$(CC) $(FLAGS) malloc_structs/btree.c -o ${OUT_DIR}/btree.o


clean:
	rm -f $(OBJS) $(OUT)