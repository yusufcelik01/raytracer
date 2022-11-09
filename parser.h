#ifndef __HW1__PARSER__
#define __HW1__PARSER__

#include <string>
#include <vector>

#include "rtmath.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Face.hpp"
#include "Mesh.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"

namespace parser
{
    //Notice that all the structures are as simple as possible
    //so that you are not enforced to adopt any style or design.
    //struct Vec3f
    //{
    //    float x, y, z;
    //};

    //struct Vec3i
    //{
    //    int x, y, z;
    //};

    //struct Vec4f
    //{
    //    float x, y, z, w;
    //};


    struct PointLight
    {
        vec3f position;
        vec3f intensity;
    };






    struct Scene
    {
        //Data
        vec3i background_color;
        float shadow_ray_epsilon;
        int max_recursion_depth;
        std::vector<Camera> cameras;
        vec3f ambient_light;
        std::vector<PointLight> point_lights;
        std::vector<Material> materials;
        std::vector<vec3f> vertex_data;
        //std::vector<Sphere*> spheres;
        //std::vector<Triangle*> triangles;
        std::vector<Mesh*> meshes;
        std::vector<Object*> objects;
        Object* AccStrBVH;

        //Functions
        void loadFromXml(const std::string &filepath);
        void render(Camera camera);
        void render(size_t cameraId);
        bool rayQuery(Ray ray, IntersectionData& closestObjData, bool isShadowRay, float maxT);
        vec3f getRayColor(Ray ray, int depth, bool isPrimaryRay, Material currentMedium);
        vec3f getObjNorm(const IntersectionData& data);
        vec3f calculateLighting(Ray eyeRay, Material material, vec3f surfNorm, vec3f p);
    };

    struct plyData
    {
        std::vector<vec3f> vertices;
        std::vector<vec3i> triangles;
    };
}

#endif
