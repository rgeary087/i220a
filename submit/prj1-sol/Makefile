TARGET = hamming
OBJS = \
       main.o \
       hamming.o

CC = gcc
CFLAGS = -std=c11 -g -Wall
LDFLAGS = 

$(TARGET):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) -o $@

hamming.o:	hamming.c hamming.o
		$(CC) $(CFLAGS) -c $<

main.o:		main.c hamming.c
		$(CC) $(CFLAGS) -c $<

clean:		
		rm -f *~ *.o $(TARGET)
