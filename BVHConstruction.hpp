#ifndef __BOUNDING__VOLUME__HIERARCHY__CONSTRUCTION
#define __BOUNDING__VOLUME__HIERARCHY__CONSTRUCTION


#include "BVH.hpp"
#include "Face.hpp"

Object* constructMeshBVH(const std::vector<vec3f>& VAO, std::vector<Face*>& objects, AXIS_TYPE axis);
Object* constructObjectBVH(const std::vector<vec3f>& VAO, std::vector<Object*>& objects, AXIS_TYPE axis);

#endif
