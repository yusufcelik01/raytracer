CC= gcc
CXX= g++
CFLAGS = -O3
CXXFLAGS = -std=c++17
LDFLAGS = -pthread


PARSER_FILES= parser.o plyfile.o
OBJECT_HPP_DEP= Object.hpp IntersectionData.hpp rtmath.hpp Ray.hpp
GEOMETRY= Mesh.o Sphere.o Face.o Triangle.o #InstancedMesh.o
MATH_DEP=  vec2.hpp vec3.hpp vec4.hpp mat4x4.hpp
MATH_OBJECTS=  vec2.o vec3.o vec4.o mat4x4.o 

OBJECT_FILES= main.o core.o $(PARSER_FILES)  tinyxml2.o  BoundingBox.o BVH.o BVHConstruction.o $(MATH_OBJECTS) img.o $(GEOMETRY) AreaLight.o

raytracer: $(OBJECT_FILES)
	$(CXX) -o raytracer $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) $(OBJECT_FILES)

all:
	g++ *.cpp -o raytracer $(CFLAGS) $(CXXFLAGS)

ply_test:
	gcc plytest.c plyfile.c -o plytest -g


vec2.o: vec2.hpp
vec3.o: vec3.hpp
vec4.o: vec4.hpp
mat4x4.o: mat4x4.hpp

AREA_LIGHT_HEADERS= AreaLight.hpp vec3.hpp
POINT_LIGHT_HEADERS= PointLight.hpp vec3.hpp
AreaLight.o: AreaLight.cpp $(AREA_LIGHT_HEADERS)

PARSER_HEADERS= parser.h tinyxml2.h $(OBJECT_HPP_DEP) ply.h $(POINT_LIGHT_HEADERS) $(AREA_LIGHT_HEADERS) rtmath.hpp Camera.hpp
parser.o: parser.cpp $(PARSER_HEADERS)
plyfile.o: plyfile.c ply.h
	$(CC) $(CFLAGS) -w -c plyfile.c

Sphere.o: Sphere.cpp Sphere.hpp $(OBJECT_HPP_DEP)
Face.o: Face.cpp Face.hpp $(OBJECT_HPP_DEP)
Triangle.o: Triangle.cpp Triangle.hpp Face.hpp $(OBJECT_HPP_DEP)
Mesh.o: Mesh.cpp Mesh.hpp Face.hpp $(OBJECT_HPP_DEP)
InstancedMesh.o: InstancedMesh.cpp InstancedMesh.hpp Mesh.hpp Face.hpp $(OBJECT_HPP_DEP)


BoundingBox.o: BoundingBox.hpp BoundingBox.cpp $(MATH_DEP)
BVH.o: $(OBJECT_HPP_DEP) BoundingBox.hpp
BVHConstruction.o: $(OBJECT_HPP_DEP) BVH.hpp

core.o: img.hpp Ray.hpp $(OBJECT_HPP_DEP) $(PARSER_HEADERS)

main.o: main.cpp parser.h ppm.h

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
	./raytracer hw1/inputs/cornellbox.xml
	./raytracer hw1/inputs/two_spheres.xml
	./raytracer hw1/inputs/four_spheres.xml
	./raytracer hw1/inputs/spheres_mirror.xml
	time ./raytracer hw1/inputs/cornellbox_recursive.xml

test: raytracer 
	make simple_test
	(time ./raytracer hw1/inputs/bunny.xml) 2> time_bunny.txt
	(time ./raytracer hw1/inputs/scienceTree.xml) 2> time_scienceTree.txt
	(time ./raytracer hw1/inputs/scienceTree_glass.xml) 2> time_scienceTree_glass.txt
	(time ./raytracer hw1/inputs/chinese_dragon.xml) 2> time_chinese_dragon.txt
	(time ./raytracer hw1/inputs/horse_and_mug.xml) 2> time_horse_and_mug.txt


test2: raytracer
	./raytracer hw2/inputs/simple_transform.xml
	./raytracer hw2/inputs/spheres.xml
	(time ./raytracer hw2/inputs/marching_dragons.xml) 2> time_marching_dragons.txt
	(time ./raytracer hw2/inputs/dragon_metal.xml) 2> time_dragon_metal.txt
	(time ./raytracer hw2/inputs/ellipsoids.xml) 2> time_ellipsoids.txt
	(time ./raytracer hw2/inputs/metal_glass_plates.xml) 2> time_metal_glass_plates.txt
	(time ./raytracer hw2/inputs/grass/grass_desert.xml) 2> time_grass.txt
	

#(time ./raytracer hw3/inputs/spheres_dof.xml) 2> time_spheres_dof.txt
#(time ./raytracer hw3/inputs/cornellbox_area.xml) 2> time_cornellbox_area.txt
test3:
	(time ./raytracer hw3/inputs/metal_glass_plates.xml) 2> time_metal_glass_plates_brushed.txt

	

hw:
	tar -czf raytracer.tar.gz Makefile *.cpp *.hpp *.h *.c

