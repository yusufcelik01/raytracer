#ifndef __BOUNDING__VOLUME__HIERARCHY__DEFINED
#define __BOUNDING__VOLUME__HIERARCHY__DEFINED

#include "Object.hpp"
#include "BoundingBox.hpp"
#include <vector>
#include <iterator>


enum AXIS_TYPE
{
    X_AXIS,
    Y_AXIS,
    Z_AXIS
};

class BVH : public Object
{
    public:
        Object* left;
        Object* right;


        //constructors
        BVH();
        //template<typename Iterator>
        //BVH(const std::vector<vec3f>& VAO, Iterator begin, Iterator end, AXIS_TYPE axis);

        bool intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intersectionData);
        //vec3f getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const;
        int getMaterialId();
        virtual BoundingBox* getBoundingBox(const std::vector<vec3f>& VAO);
        virtual BoundingBox* getBoundingBox() const ;

};

//template<typename T>
//Object* constructBVH(const std::vector<vec3f>& VAO, std::vector<T>::iterator begin, std::vector<T>::iterator end, AXIS_TYPE axis);

#endif
