CC              = g++ -std=c++14
CC_OBJ_FLAGS    = -c -fPIC
CC_Shared_FLAGS = -shared -Wl,-soname,libuRwellTools.so
ROOT_CFLAGS     = $(shell ${ROOTSYS}/bin/root-config --cflags)
ROOT_LIBS       = $(shell ${ROOTSYS}/bin/root-config --libs)
libuRwellTools  = libuRwellTools


all:		uRwellTools.o
		mkdir -p lib ; rm -f lib/*.so
		$(CC) $(CC_Shared_FLAGS) -o lib/${libuRwellTools}.so.1.0.1 uRwellTools.o
		cd lib;\
		ln -sf ${libuRwellTools}.so.1.0.1 ${libuRwellTools}.so.1; ln -sf ${libuRwellTools}.so.1.0.1 ${libuRwellTools}.so

uRwellTools.o:	src/uRwellTools.cc include/uRwellTools.h
		$(CC) $(CC_OBJ_FLAGS) src/uRwellTools.cc -o $@ -I ./include

clean:      
		rm -f JPsiGen.exe *.o lib/*.so.* lib/*.so
