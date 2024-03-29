#ifndef __BOUNDING__VOLUME__HIERARCHY__CONSTRUCTION
#define __BOUNDING__VOLUME__HIERARCHY__CONSTRUCTION


#include "BVH.hpp"
#include "Face.hpp"

Object* constructObjectBVH(const std::vector<vec3f>& VAO, std::vector<Object*>::iterator begin, std::vector<Object*>::iterator end, AXIS_TYPE axis);
Object* constructMeshBVH(const std::vector<vec3f>& VAO, std::vector<Face*>::iterator begin, std::vector<Face*>::iterator end, AXIS_TYPE axis);
#endif
