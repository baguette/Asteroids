EXE=asteroid
FRAMEWORKS=-framework OpenGL -framework GLUT
SOURCES=entity.cpp alien.cpp ship.cpp asteroid.cpp bitmap.cpp main.cpp
CC=gcc
CXX=g++

all: $(SOURCES)
	$(CXX) -o $(EXE) $(FRAMEWORKS) $(SOURCES)

entity.cpp: entity.hpp
alien.cpp: alien.hpp entity.hpp entity.cpp
asteroid.cpp: asteroid.hpp entity.hpp entity.cpp
ship.cpp: ship.hpp entity.hpp entity.cpp
bitmap.cpp: bitmap.hpp
main.cpp: entity.hpp ship.hpp bitmap.hpp asteroid.hpp

clean:
	rm -f $(EXE)
