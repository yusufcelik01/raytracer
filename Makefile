CC= gcc
CXX= g++
CFLAGS = -g
CXXFLAGS = -std=c++17

OBJECT_FILES= main.o  parser.o  ppm.o  Sphere.o  tinyxml2.o  vec3.o

raytracer: $(OBJECT_FILES)
	$(CXX) -o raytracer $(OBJECT_FILES) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS)

all:
	g++ *.cpp -o raytracer $(CFLAGS) $(CXXFLAGS)


main.o: main.cpp parser.h ppm.h
parser.o: parser.cpp parser.h
Sphere.o: Sphere.cpp Sphere.hpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< 




.PHONY: del_images
del_images:
	-rm *.ppm
	-rm *.png

.PHONY: clean
clean:
	-rm *.o 
	-rm raytracer

