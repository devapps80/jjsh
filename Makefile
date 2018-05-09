SRCS=jjsh.c

CC=gcc
#CFLAGS=-Wall -Werror -Wextra -std=gnu99

all:
	$(CC) $(SRCS) -o bin/jjsh

clean:
	rm -f bin/jjsh
