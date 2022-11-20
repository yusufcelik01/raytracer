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
        std::vector<mat4x4> rotations;
        std::vector<mat4x4> scalings;
        std::vector<mat4x4> translations;
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
        void renderRow(void*);
        bool rayQuery(Ray ray, IntersectionData& closestObjData, bool isShadowRay, float maxT);
        vec3f getRayColor(Ray ray, int depth, bool isPrimaryRay, Material currentMedium);
        vec3f getObjNorm(const IntersectionData& data);
        vec3f calculateLighting(Ray eyeRay, Material material, vec3f surfNorm, vec3f p);
    };

    struct plyData
    {
        std::vector<vec3f> vertices;
        std::vector<Face> triangles;
    };
}

#endif
