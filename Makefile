SHELL= /bin/sh
srcdir= ./src
libdir= ./build
VPATH= $(srcdir)

CC= gcc
CXX= g++
COMMON_FLAGS= -g -O3 -msse2 
CFLAGS = $(COMMON_FLAGS)
CXXFLAGS = $(COMMON_FLAGS) -std=c++17 
LDFLAGS = -pthread


#TODO correct header dependency issues
#header dependency variables
OBJECT_CLASS_HEADERS= Object.hpp IntersectionData.hpp Matrix.hpp Material.hpp rtmath.hpp Ray.hpp
PARSER_HEADERS= parser.h tinyxml2.h $(OBJECT_CLASS_HEADERS) ply.h $(POINT_LIGHT_HEADERS) $(AREA_LIGHT_HEADERS) rtmath.hpp Camera.hpp ImageTexture.hpp Texture.hpp
AREA_LIGHT_HEADERS= AreaLight.hpp vec3.hpp
POINT_LIGHT_HEADERS= PointLight.hpp vec3.hpp
MATH_HEADERS=  vec2.hpp vec3.hpp vec4.hpp mat4x4.hpp UniformRandomGenerator.hpp 

#object files
PARSER_OBJECTS= plyfile.o parser.o 
GEOMETRY_OBJECTS= Object.o Mesh.o Sphere.o Face.o Triangle.o
TEXTURES= ImageTexture.o PerlinNoise.o CheckerBoard.o Image.o SphericalEnvLight.o
MATH_OBJECTS=  vec2.o vec3.o vec4.o mat4x4.o UniformRandomGenerator.o rtmath.o Matrix.o

OBJ=miniz.o $(PARSER_OBJECTS) Material.o  main.o core.o tinyxml2.o  BoundingBox.o BVH.o BVHConstruction.o $(MATH_OBJECTS) img.o $(GEOMETRY_OBJECTS) AreaLight.o $(TEXTURES) tonemap.o 
OBJECT_FILES= $(addprefix $(libdir)/,$(OBJ) ) 

raytracer: build $(OBJECT_FILES)
	$(CXX) -o raytracer $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) $(OBJECT_FILES)

build:
	-mkdir build

$(libdir)/main.o: $(srcdir)/main.cpp $(PARSER_HEADERS) ppm.h BVHConstruction.hpp
	$(CXX) $(CXXFLAGS) -c $<  -o $@
$(libdir)/parser.o: $(srcdir)/parser.cpp $(PARSER_HEADERS) Material.hpp
	$(CXX) $(CXXFLAGS) -c $<  -o $@
$(libdir)/tinyxml2.o: $(srcdir)/tinyxml2.cpp tinyxml2.h
	$(CXX) $(CXXFLAGS) -c $<  -o $@
$(libdir)/core.o: $(srcdir)/core.cpp img.hpp Ray.hpp $(OBJECT_CLASS_HEADERS) $(PARSER_HEADERS)
	$(CXX) $(CXXFLAGS) -c $<  -o $@
	

ply_test: 
	gcc plytest.c plyfile.c -o plytest -g


$(libdir)/vec2.o: vec2.hpp $(srcdir)/vec2.cpp
$(libdir)/vec3.o: vec3.hpp $(srcdir)/vec3.cpp
$(libdir)/vec4.o: vec4.hpp $(srcdir)/vec4.cpp
$(libdir)/mat4x4.o: mat4x4.hpp $(srcdir)/mat4x4.cpp
$(libdir)/Matrix.o: Matrix.hpp $(srcdir)/Matrix.cpp
$(libdir)/rtmath.o: rtmath.hpp $(srcdir)/rtmath.cpp

AreaLight.o: AreaLight.cpp $(AREA_LIGHT_HEADERS)

$(libdir)/plyfile.o: $(srcdir)/plyfile.c ply.h
	$(CC) $(CFLAGS) -w -c $< -o $@
	

$(libdir)/Object.o: $(srcdir)/Object.cpp Object.hpp $(OBJECT_CLASS_HEADERS) $(MATH_HEADERS)
	$(CXX) $(CXXFLAGS) -c $<  -o $@
Sphere.o: Sphere.cpp Sphere.hpp $(OBJECT_CLASS_HEADERS) $(MATH_HEADERS)
Face.o: Face.cpp Face.hpp $(OBJECT_CLASS_HEADERS) $(MATH_HEADERS)
Triangle.o: Triangle.cpp Triangle.hpp Face.hpp $(OBJECT_CLASS_HEADERS)
Mesh.o: Mesh.cpp Mesh.hpp Face.hpp $(OBJECT_CLASS_HEADERS $(MATH_HEADERS))
InstancedMesh.o: InstancedMesh.cpp InstancedMesh.hpp Mesh.hpp Face.hpp $(OBJECT_CLASS_HEADERS $(MATH_HEADERS))

BoundingBox.o: BoundingBox.hpp BoundingBox.cpp $(MATH_HEADERS)
BVH.o: $(OBJECT_CLASS_HEADERS) BoundingBox.hpp
BVHConstruction.o: $(OBJECT_CLASS_HEADERS) BVH.hpp

ImageTexture.o: Texture.hpp ImageTexture.hpp ImageTexture.cpp rtmath.hpp
PerlinNoise.o: Texture.hpp PerlinNoise.hpp PerlinNoise.cpp rtmath.hpp
CheckerBoard.o: Texture.hpp CheckerBoard.hpp CheckerBoard.cpp



$(libdir)/%.o: $(srcdir)/%.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c $<  -o $@

$(libdir)/%.o: $(srcdir)/%.c %.h
	$(CC) $(CFLAGS) -c $<  -o $@



.PHONY: del_images
del_images:
	-rm *.png
	-rm *.exr

.PHONY: clean
clean:
	-rm $(libdir)/*.o 
	-rmdir $(libdir)
	-rm raytracer

#test: raytracer
#	for foo in hw1/inputs/*.xml; do ./raytracer $$foo; done

simple_test: raytracer
	./raytracer hw1/inputs/simple.xml
	./raytracer hw1/inputs/cornellbox.xml
	./raytracer hw1/inputs/two_spheres.xml
	#./raytracer hw1/inputs/four_spheres.xml
	./raytracer hw1/inputs/spheres_mirror.xml
	time ./raytracer hw1/inputs/cornellbox_recursive.xml

test: raytracer 
	make simple_test
	(time ./raytracer hw1/inputs/bunny.xml) 2> time_bunny.txt
	(time ./raytracer hw1/inputs/scienceTree.xml) 2> time_scienceTree.txt
	(time ./raytracer hw1/inputs/scienceTree_glass.xml) 2> time_scienceTree_glass.txt
	(time ./raytracer hw1/inputs/chinese_dragon.xml) 2> time_chinese_dragon.txt
	#(time ./raytracer hw1/inputs/horse_and_mug.xml) 2> time_horse_and_mug.txt


test2: raytracer
	./raytracer hw2/inputs/simple_transform.xml
	./raytracer hw2/inputs/spheres.xml
	(time ./raytracer hw2/inputs/ellipsoids.xml) 2> time_ellipsoids.txt
	(time ./raytracer hw2/inputs/metal_glass_plates.xml) 2> time_metal_glass_plates.txt
	(time ./raytracer hw2/inputs/marching_dragons.xml) 2> time_marching_dragons.txt
	(time ./raytracer hw2/inputs/dragon_metal.xml) 2> time_dragon_metal.txt
	#(time ./raytracer hw2/inputs/grass/grass_desert.xml) 2> time_grass.txt
	

test3:
	(time ./raytracer hw3/inputs/cornellbox_area.xml) 2> time_cornellbox_area.txt
	(time ./raytracer hw3/inputs/metal_glass_plates.xml) 2> time_rough_metal_glass_plates_brushed.txt
	(time ./raytracer hw3/inputs/spheres_dof.xml) 2> time_spheres_dof.txt
	(time ./raytracer hw3/inputs/cornellbox_boxes_dynamic.xml) 2> time_cornellbox_boxes_dynamic.txt

	
test4:
	./raytracer hw4/inputs/brickwall_with_normalmap.xml 
	./raytracer hw4/inputs/cube_cushion.xml 
	./raytracer hw4/inputs/cube_perlin.xml 
	./raytracer hw4/inputs/cube_perlin_bump.xml 
	./raytracer hw4/inputs/cube_wall.xml 
	./raytracer hw4/inputs/cube_wall_normal.xml 
	./raytracer hw4/inputs/cube_waves.xml 
	./raytracer hw4/inputs/galactica_static.xml 
	./raytracer hw4/inputs/wood_box.xml 
	./raytracer hw4/inputs/wood_box_all.xml 
	./raytracer hw4/inputs/wood_box_no_specular.xml 
	./raytracer hw4/inputs/ellipsoids_texture.xml 
	./raytracer hw4/inputs/sphere_nearest_bilinear.xml 
	./raytracer hw4/inputs/sphere_nobump_bump.xml 
	./raytracer hw4/inputs/sphere_nobump_justbump.xml 
	./raytracer hw4/inputs/sphere_perlin.xml 
	./raytracer hw4/inputs/sphere_perlin_bump.xml 

test5:
	(time ./raytracer hw5/inputs/glass_sphere_env.xml) 2> time_glass_sphere_env.txt
	(time ./raytracer hw5/inputs/mirror_sphere_env.xml) 2> time_mirror_sphere_env.txt
	(time ./raytracer hw5/inputs/cube_directional.xml) 2> time_cube_directional.txt
	(time ./raytracer hw5/inputs/cube_point.xml) 2> time_cube_point.txt
	(time ./raytracer hw5/inputs/cube_point_hdr.xml) 2> time_cube_point_hdr.txt

test_brdf:
	./raytracer hw6/brdf/inputs/brdf_blinnphong_original.xml
	./raytracer hw6/brdf/inputs/brdf_phong_original.xml
	./raytracer hw6/brdf/inputs/brdf_phong_modified.xml
	./raytracer hw6/brdf/inputs/brdf_phong_modified_normalized.xml
	./raytracer hw6/brdf/inputs/brdf_blinnphong_modified.xml
	./raytracer hw6/brdf/inputs/brdf_blinnphong_modified_normalized.xml

hw:
	tar -czf raytracer.tar.gz Makefile src/
