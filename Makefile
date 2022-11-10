CC= gcc
CXX= g++
CFLAGS = -Ofast
CXXFLAGS = -std=c++17
LDFLAGS = 


OBJECT_HPP_DEP= Object.hpp IntersectionData.hpp rtmath.hpp Ray.hpp
GEOMETRY= Mesh.o Sphere.o Face.o Triangle.o
OBJECT_FILES= main.o  core.o parser.o  tinyxml2.o  BoundingBox.o BVH.o BVHConstruction.o vec3.o img.o $(GEOMETRY)

raytracer: $(OBJECT_FILES)
	$(CXX) -o raytracer $(OBJECT_FILES) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS)

all:
	g++ *.cpp -o raytracer $(CFLAGS) $(CXXFLAGS)
bahadir:
	g++ $(CXXFLAGS) $(CFLAGS) *.cpp  -o main

vec3.cpp: vec3.hpp
main.o: main.cpp parser.h ppm.h
parser.o: parser.cpp parser.h tinyxml2.h $(OBJECT_HPP_DEP)

Sphere.o: Sphere.cpp Sphere.hpp $(OBJECT_HPP_DEP)
Face.o: Face.cpp Face.hpp $(OBJECT_HPP_DEP)
Mesh.o: Mesh.cpp Mesh.hpp Face.hpp $(OBJECT_HPP_DEP)
Triangle.o: Triangle.cpp Triangle.hpp Face.hpp $(OBJECT_HPP_DEP)

BoundingBox.o: BoundingBox.hpp BoundingBox.cpp rtmath.hpp 
BVH.o: $(OBJECT_HPP_DEP) BoundingBox.hpp
BVHConstruction.o: $(OBJECT_HPP_DEP) BVH.hpp

core.o: parser.h img.hpp Ray.hpp $(OBJECT_HPP_DEP)

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

#test: raytracer
#	for foo in hw1/inputs/*.xml; do ./raytracer $$foo; done

simple_test: raytracer
	./raytracer hw1/inputs/simple.xml
	./raytracer hw1/inputs/spheres.xml
	./raytracer hw1/inputs/cornellbox.xml
	./raytracer hw1/inputs/two_spheres.xml
	./raytracer hw1/inputs/four_spheres.xml
	./raytracer hw1/inputs/spheres_mirror.xml
	time ./raytracer hw1/inputs/cornellbox_recursive.xml

test:
	make simple_test
	time ./raytracer hw1/inputs/bunny.xml
	time ./raytracer hw1/inputs/scienceTree.xml
	time ./raytracer hw1/inputs/scienceTree_glass.xml

hw:
	tar -czf raytracer.tar.gz Makefile *.cpp *.hpp *.h

