ROOTFLAGS = `root-config --cflags`
ROOTLIBS = `root-config --libs`
DEBUG = -Wall

all: anap4.c
	c++ $(ROOTLIBS) $(ROOTFLAGS) $(DEBUG) -o anap4 anap4.c

clean:
	rm -f anap4 *.o
