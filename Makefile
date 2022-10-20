CC= gcc
CXX= g++
CFLAGS = -g 
CXXFLAGS = -std=c++17


GEOMETRY= Mesh.o Sphere.o Face.o Triangle.o
OBJECT_FILES= main.o  parser.o  ppm.o tinyxml2.o  vec3.o img.o $(GEOMETRY)

raytracer: $(OBJECT_FILES)
	$(CXX) -o raytracer $(OBJECT_FILES) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS)

all:
	g++ *.cpp -o raytracer $(CFLAGS) $(CXXFLAGS)


vec3.cpp: vec3.hpp
main.o: main.cpp parser.h ppm.h
parser.o: parser.cpp parser.h
Sphere.o: Sphere.cpp Sphere.hpp Object.hpp
Triangle.o: Triangle.cpp Triangle.hpp Object.hpp
Face.o: Face.cpp Face.hpp Object.hpp
Mesh.o: Mesh.cpp Mesh.hpp Object.hpp

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c $< 




.PHONY: del_images
del_images:
	-rm *.ppm
	-rm *.png

.PHONY: clean
clean:
	-rm *.o 
	-rm raytracer

test: raytracer
	for foo in hw1/inputs/*.xml; do ./raytracer $$foo; done

simple_test: raytracer
	./raytracer hw1/inputs/simple.xml
	./raytracer hw1/inputs/spheres.xml
	./raytracer hw1/inputs/cornellbox.xml
	./raytracer hw1/inputs/two_spheres.xml
	./raytracer hw1/inputs/four_spheres.xml
	./raytracer hw1/inputs/spheres_mirror.xml
	./raytracer hw1/inputs/cornellbox_recursive.xml
