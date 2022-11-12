CC= gcc
CXX= g++
CFLAGS = -g
CXXFLAGS = -std=c++17
LDFLAGS = 


PARSER_FILES= parser.o plyfile.o
OBJECT_HPP_DEP= Object.hpp IntersectionData.hpp rtmath.hpp Ray.hpp
GEOMETRY= Mesh.o Sphere.o Face.o Triangle.o
OBJECT_FILES= main.o  core.o $(PARSER_FILES)  tinyxml2.o  BoundingBox.o BVH.o BVHConstruction.o vec3.o img.o $(GEOMETRY)

raytracer: $(OBJECT_FILES)
	$(CXX) -o raytracer $(OBJECT_FILES) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS)

all:
	g++ *.cpp -o raytracer $(CFLAGS) $(CXXFLAGS)

ply_test:
	gcc plytest.c plyfile.c -o plytest -g


vec3.cpp: vec3.hpp
main.o: main.cpp parser.h ppm.h
parser.o: parser.cpp parser.h tinyxml2.h $(OBJECT_HPP_DEP) ply.h
plyfile.o: ply.h
#plytest.o: ply.h

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
	-rm time*.txt

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
	(time ./raytracer hw1/inputs/bunny.xml) 2> time_bunny.txt
	(time ./raytracer hw1/inputs/scienceTree.xml) 2> time_scienceTree.txt
	(time ./raytracer hw1/inputs/scienceTree_glass.xml) 2> time_scienceTree_glass.txt
	(time ./raytracer hw1/inputs/chinese_dragon.xml) 2> time_chinese_dragon.txt
	(time ./raytracer hw1/inputs/horse_and_mug.xml) 2> time_horse_and_mug.txt

hw:
	tar -czf raytracer.tar.gz Makefile *.cpp *.hpp *.h

