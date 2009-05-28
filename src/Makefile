##################################################
##
## Create Date:   May 6th, 2009
## Author     :   dragon, www.gnumac.cn
## Modify Date:   May 6th, 2009
##
##################################################
CC = g++
INCLUDE += -I ./ -I/usr/local/include
CFLAGS +=  `pkg-config --cflags gtk+-2.0` -g -O2 -W 
LIBS +=  `pkg-config --libs gtk+-2.0 gthread-2.0` -lsupc++ -lz -lm -lc 

OBJ = dragon.o task.o conn.o http.o ftp.o xml.o conf.o taskctrl.o ui.o 

target = fastget 

VPATH = ./

.SUFFIXES:	.o .cpp

.cpp.o:
	$(CC) $(INCLUDE) -c $(CFLAGS) -o $@ $<

all:	$(target) 

$(target):	$(OBJ)
	$(CC) -o $(target) $(OBJ) $(LIBS)
	mv $(target) ../

clean:
	rm -rf *.o *~ $(target) 
