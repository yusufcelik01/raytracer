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
#include "PointLight.hpp"
#include "AreaLight.hpp"
#include "tinyxml2.h"

namespace parser
{


    struct Scene
    {
        //Data
        vec3i background_color;
        float shadow_ray_epsilon;
        int max_recursion_depth;
        std::vector<Camera> cameras;
        
        vec3f ambient_light;
        std::vector<PointLight> point_lights;
        std::vector<AreaLight> area_lights;

        std::vector<Material> materials;
        std::vector<mat4x4> rotations;
        std::vector<mat4x4> scalings;
        std::vector<mat4x4> translations;
        
        //std::vector<Images> images;
        //std::vector<Textures> textures;

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
        bool rayQuery(Ray ray, IntersectionData& retData, bool isShadowRay, float maxT, float nearDist, vec3f gaze);
        vec3f getRayColor(Ray ray, int depth, bool isPrimaryRay, Material currentMedium);
        vec3f getRayColor(Ray ray, int depth, bool isPrimaryRay, Material currentMedium, vec3f gaze, float nearDist);
        vec3f getObjNorm(const IntersectionData& data);
        vec3f calculateLighting(Ray eyeRay, Material material, vec3f surfNorm, vec3f p);

        private:
        void renderRow(void*);
        void renderRowMultiSampled(void*);

        //parsing helpers
        void getObjAttributes(tinyxml2::XMLNode* element, Object* obj);
        void parseTextures(tinyxml2::XMLNode* sceneNode);
    };

    struct plyData
    {
        std::vector<vec3f> vertices;
        std::vector<Face> triangles;
    };
}

void parsePly(char* filePath, parser::plyData& plyMesh);

#endif
