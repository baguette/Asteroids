EXE=asteroid
FRAMEWORKS=-framework OpenGL -framework GLUT
SOURCES=vector.cpp entity.cpp explosion.cpp alien.cpp ship.cpp asteroid.cpp laser.cpp game.cpp title.cpp gameover.cpp bitmap.cpp main.cpp
CC=gcc
CXX=g++

all: $(SOURCES)
	$(CXX) -g -o $(EXE) $(FRAMEWORKS) $(SOURCES)

gameover.cpp: gameover.hpp
explosion.cpp: explosion.hpp
title.hpp: scene.hpp
title.cpp: title.hpp
game.hpp: scene.hpp
game.cpp: game.hpp
vector.cpp: vector.hpp
entity.cpp: entity.hpp
laser.cpp: laser.hpp
alien.cpp: alien.hpp entity.hpp entity.cpp
asteroid.cpp: asteroid.hpp entity.hpp entity.cpp
ship.cpp: ship.hpp entity.hpp entity.cpp
bitmap.cpp: bitmap.hpp
main.cpp: vector.cpp entity.cpp explosion.cpp alien.cpp ship.cpp asteroid.cpp laser.cpp game.cpp title.cpp gameover.cpp bitmap.cpp

clean:
	rm -f $(EXE)
	rm -f *.o
	rm -rf asteroid.dSYM

sloc:
	cat *.c* *.h* Makefile |wc -l
