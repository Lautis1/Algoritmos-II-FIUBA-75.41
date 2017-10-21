EXEC	=	pruebas
OBJFILES	=	*.c

CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes


all: $(EXEC)

run: all
	./$(EXEC)

valgrind: 
	valgrind $(VFLAGS) ./$(EXEC)

$(EXEC): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJFILES)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

