TARGET = hamming-encode
TARGET2 = hamming-decode
OBJS = \
       main.o \
       hamming.o

CC = gcc
CFLAGS = -std=c11 -g -Wall
LDFLAGS = 

$(TARGET2):	$(OBJS) $(TARGET)
		$(CC) $(OBJS) $(LDFLAGS) -o $@ 

$(TARGET):	
		ln -s -f $(TARGET2) $(TARGET)


hamming.o:	hamming.c hamming.h 
		$(CC) $(CFLAGS) -c $<

main.o:		main.c hamming.c
		$(CC) $(CFLAGS) -c $<

.PHONY:		clean

clean:		
		rm -f *~ *.o main.o hamming.o 
