EXE=asteroid
FRAMEWORKS=-framework OpenGL -framework GLUT
SOURCES=vector.cpp entity.cpp alien.cpp ship.cpp asteroid.cpp laser.cpp bitmap.cpp main.cpp
CC=gcc
CXX=g++

all: $(SOURCES)
	$(CXX) -g -o $(EXE) $(FRAMEWORKS) $(SOURCES)

vector.cpp: vector.hpp
entity.cpp: entity.hpp
laser.cpp: laser.hpp
alien.cpp: alien.hpp entity.hpp entity.cpp
asteroid.cpp: asteroid.hpp entity.hpp entity.cpp
ship.cpp: ship.hpp entity.hpp entity.cpp
bitmap.cpp: bitmap.hpp
main.cpp: vector.hpp entity.hpp ship.hpp bitmap.hpp asteroid.hpp

clean:
	rm -f $(EXE)
	rm -rf asteroid.dSYM

sloc:
	cat *.c* *.h* Makefile |wc -l
