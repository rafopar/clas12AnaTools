## -*- Makefile -*-
##
## User: rafopar
## Time: Jan 3, 2022 9:53:05 PM
## Makefile created by Oracle Developer Studio.
##
## This file is generated automatically.
##

CC              = g++ -std=c++11
CC_OBJ_FLAGS    = -c -fPIC
CC_Shared_FLAGS = -shared -Wl,-soname,libclas12AnaTools.so
HIPO_FLAGS      = -I/home/rafopar/work/git/hipo/hipo4
ROOT_CFLAGS     = $(shell /home/rafopar/work/builds/root/root/bin/root-config --cflags)
ROOT_LIBS       = $(shell /home/rafopar/work/builds/root/root/bin/root-config --libs)
clas12AnaTools  = libclas12AnaTools

## Target: all
all:		RecParticle.o clas12AnaTools.o
		mkdir -p lib; rm -f lib/*.so
		$(CC) $(CC_Shared_FLAGS) -o lib/${clas12AnaTools}.so.1.0.1 $?
		cd lib;\
		ln -sf ${clas12AnaTools}.so.1.0.1 ${clas12AnaTools}.so.1; ln -sf ${clas12AnaTools}.so.1.0.1 ${clas12AnaTools}.so
		
RecParticle.o:	src/RecParticle.cc include/RecParticle.h
		$(CC) $(CC_OBJ_FLAGS) src/RecParticle.cc -o $@ $(ROOT_CFLAGS) $(HIPO_FLAGS) -I./include

clas12AnaTools.o:	src/clas12AnaTools.cc include/clas12AnaTools.h
			$(CC) $(CC_OBJ_FLAGS) src/clas12AnaTools.cc -o $@ $(ROOT_CFLAGS) -I./include


## Target: clean
clean:		 
		rm -f *.o lib/*.so*


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux
