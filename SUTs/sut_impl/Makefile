

#CFLAGS  = -g3 -ggdb3 -Wall -std=gnu99
INCLUDE = -I.

#LDPATH  =
#LDFLAGS = -lpthread

PATH := c:\opt\gcc-4.9-win64\bin:$(PATH)
export PATH

CC=gcc

all:		liblfr_ctrl.a

liblfr_ctrl.a: lfr_ctrl.o
	ar cru $(@) $(<) 
	ranlib $(@)

lfr_ctrl.o: lfr_ctrl.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o lfr_ctrl.o lfr_ctrl.c

clean:		
	rm -f *~ *.o 
