PROJECT = quadr

CFLAGS = -g -Wall -std=c11
LDFLAGS =  -lm
SRC_FILES = main.c quadr.c quadr.h

$(PROJECT):	quadr.o main.o
		$(CC) quadr.o main.o  $(LDFLAGS) -o  $@



#automatically generated dependencies produced by make depend: main.c quadr.c quadr.h

dist:		$(PROJECT).tar.gz

$(PROJECT).tar.gz:	$(SRC_FILES)
			tar -cf $(PROJECT).tar $(SRC_FILES)
			gzip -f $(PROJECT).tar

clean:
		rm -f *.o $(PROJECT) $(PROJECT).tar.gz

depend:		
		$(CC0 -MM $(CPPFLAGS) *.c

