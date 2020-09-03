PROJECT = key-value

CFLAGS = -g -Wall -std=c11
LDFLAGS =  -lm
SRC_FILES = key-value.c key-value.h main.c process-file.h process-file.c

$(PROJECT):	process-file.o key-value.o main.o
		$(CC) process-file.o key-value.o main.o  $(LDFLAGS) -o  $@



#automatically generated dependencies produced by make depend: main.c quadr.c quadr.h

dist:		$(PROJECT).tar.gz

$(PROJECT).tar.gz:	$(SRC_FILES)
			tar -cf $(PROJECT).tar $(SRC_FILES)
			gzip -f $(PROJECT).tar

clean:
		rm -f *.o $(PROJECT) $(PROJECT).tar.gz

depend:		
		$(CC0 -MM $(CPPFLAGS) *.c

