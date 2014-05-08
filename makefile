#Compiler
CC=g++
#Standard
STD=-std=c++11
#link options
LIBS=-lGL -lGLU -lX11 -lXxf86vm -lglog -pthread
#Warnings
WARN=-Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -pedantic
#Flags
CFLAGS=$(WARN) 
#Includes
IDIR=include
#Librarys
LDIR=lib
#Output directory
ODIR=obj
#Source
SDIR=src
#Shared
SLDIR=/usr/lib
#Options
OPT=-DDEBUG

HEADERS=window.h rle.h file_loader.h image.h utils.h ilbm_iff.h
DEPS=$(patsubst %,$(IDIR)/%,$(HEADERS))

OBJECTS= window.o rle.o file_loader.h image.o utils.o ilbm_iff.o main.o
OBJ=$(patsubst %,$(ODIR)/%,$(OBJECTS))

#quad.o: $(DEPS)
#	$(CC) $(STD) -c -o $(ODIR)/%.o $< $(CFLAGS)

quad: $(SDIR)/*.cpp
	$(CC) $(STD) $(OPT) -I$(IDIR) -o $@ $^ $(CFLAGS)$(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm -f quad
