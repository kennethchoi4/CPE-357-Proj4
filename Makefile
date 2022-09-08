CC = gcc
CFLAGS = -Wall -Wextra -Werror -ansi -pedantic
MAIN = tree
OBJS = build.o main.o arg.o
DOTH = build.h arg.h
all: $(MAIN)

$(MAIN) : $(OBJS) $(DOTH)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

main.o: main.c $(DOTH)
	$(CC) -gdwarf-2 -g3 -c main.c 

arg.o: arg.c $(DOTH)
	$(CC) -gdwarf-2 -g3 -c arg.c 

build.o: build.c $(DOTH)
	$(CC) -gdwarf-2 -g3 -c build.c 

clean :
	rm * .o $(MAIN) core