# Makefile
#
# It uses implicit rules to compile .c files using the
# values assigned to macros CC and CFLAGS
#

CC = gcc
CFLAGS = -g -Wall -std=c11


thread: thread.o
	gcc -o thread thread.o -lpthread


thread-alt: thread-alt.o
	gcc -o thread-alt thread-alt.o 

clean:
	rm -f *.o thread 
