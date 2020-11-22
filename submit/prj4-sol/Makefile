TARGET =	y86-sim

OBJS =	\
	main.o \
	ysim.o

CC = gcc

CFLAGS = -std=c11 -g -Wall

CPPFLAGS = -I $$HOME/cs220/include

LDFLAGS = -L $$HOME/cs220/lib -l cs220 -l y86

INCLUDE =	/home/rgeary1/cs220/include


$(TARGET):	$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS)  -o $@

ysim.o:		ysim.h $(INCLUDE)/y86.h $(INCLUDE)/errors.h

main.o:		main.c ysim.h $(INCLUDE)/y86.h $(INCLUDE)/yas.h $(INCLUDE)/errors.h

.PHONY:
	
clean:		
		rm -f *~ *.o $(TARGET) 
