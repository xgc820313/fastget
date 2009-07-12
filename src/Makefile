############################################################
#           
#  Author:  Dragon, Fong's National Engineering Co., Ltd.
#  Date  :  2009-04-22
#  Version: 0.1
#  Remarks: create a cross-platform multi-thread downloader
# 
############################################################
OBJECTOUT		= obj
SRCS  = $(wildcard *.cpp)

OBJECTS	= $(addprefix $(OBJECTOUT)/,$(notdir $(patsubst %.cpp, %.o, $(SRCS))))

TARGET	=	fastget

WORKHOME		= .
#APISUBDIR		= $(WORKHOME)/include
APISUBDIR		= .
APIHEADER		= $(wildcard $(APISUBDIR)/*.h)
DIRS			= $(sort $(dir $(SRCS)))
INCLUDE			= $(addprefix -I, $(DIRS)) -I$(APISUBDIR)

ACTDATE			= $(shell date +%Y%m%d%H%M)
TAR				= $(notdir $(basename $(TARGET)))$(ACTDATE).tar.bz2

CC	= g++ -g
AR      = @ar
RM	= @rm -v
CP      = cp
MV	= mv -v

MAKEVARSFILE           = .Makefile.pvar
DEPENDFILE             = depend

vpath %.cpp $(DIRS)
vpath %.h $(APISUBDIR)
VPATH = $(DIRS)

CFLAGS		= -I/Developer/SDKs/MacOSX10.5.sdk/Library/Frameworks/Cairo.framework/Headers -I/Developer/SDKs/MacOSX10.5.sdk/Library/Frameworks/GLib.framework/Headers -I/Developer/SDKs/MacOSX10.5.sdk/Library/Frameworks/Gtk.framework/Headers -g -O2 -W #`pkg-config --cflags gtk+-2.0` -g -O2
LDFLAGS		= -mmacosx-version-min=10.5 -framework GLib -framework Gtk -framework Cairo # `pkg-config --libs gtk+-2.0 gthread-2.0` 
LOADLIBES	= -lz -lm -lsupc++ -lc -lexpat

.FONY: clean .distclean tar copy  $(MAKEVARSFILE) 


all: .$(OBJECTOUT) $(TARGET) 

$(TARGET):$(OBJECTS) 
	$(CC) $(LDFLAGS) -o $(WORKHOME)/$@ $^ $(LDFLAGS) $(LOADLIBES) 
	mkdir -p ../$(TARGET).app/Contents/MacOS
	mv $(TARGET) ../$(TARGET).app/Contents/MacOS

$(OBJECTOUT)/%.o: %.cpp
	$(CC) $(INCLUDE) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

clean:
	@find . -name "*.o" -exec rm {} \;
	$(RM) -f $(MAKEVARSFILE) $(DEPENDFILE) $(OBJECTOUT)/*
#	$(RM) -f $(WORKHOME)/$(TARGET)
	$(RM) -rf ../$(TARGET).app

.$(OBJECTOUT):
	@mkdir -p $(OBJECTOUT) >/dev/null 2>&1
