#include "parser.h"
#include "ppm.h"
#include "img.hpp"
#include "Ray.hpp"
#include "tinyxml2.h"
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <limits>

void parser::Scene::loadFromXml(const std::string &filepath)
{
    tinyxml2::XMLDocument file;
    std::stringstream stream;

    auto res = file.LoadFile(filepath.c_str());
    if (res)
    {
        throw std::runtime_error("Error: The xml file cannot be loaded.");
    }

    auto root = file.FirstChild();
    if (!root)
    {
        throw std::runtime_error("Error: Root is not found.");
    }

    //Get BackgroundColor
    auto element = root->FirstChildElement("BackgroundColor");
    if (element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0 0 0" << std::endl;
    }
    stream >> background_color.x >> background_color.y >> background_color.z;

    //Get ShadowRayEpsilon
    element = root->FirstChildElement("ShadowRayEpsilon");
    if (element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0.001" << std::endl;
    }
    stream >> shadow_ray_epsilon;

    //Get MaxRecursionDepth
    element = root->FirstChildElement("MaxRecursionDepth");
    if (element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0" << std::endl;
    }
    stream >> max_recursion_depth;

    //Get Cameras
    element = root->FirstChildElement("Cameras");
    element = element->FirstChildElement("Camera");
    Camera camera;
    while (element)
    {
        auto child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Gaze");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Up");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("NearPlane");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("NearDistance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("ImageResolution");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("ImageName");
        stream << child->GetText() << std::endl;

        stream >> camera.position.x >> camera.position.y >> camera.position.z;
        stream >> camera.gaze.x >> camera.gaze.y >> camera.gaze.z;
        stream >> camera.up.x >> camera.up.y >> camera.up.z;
        stream >> camera.near_plane.x >> camera.near_plane.y >> camera.near_plane.z >> camera.near_plane.w;
        stream >> camera.near_distance;
        stream >> camera.image_width >> camera.image_height;
        stream >> camera.image_name;

        cameras.push_back(camera);
        element = element->NextSiblingElement("Camera");
    }

    //Get Lights
    element = root->FirstChildElement("Lights");
    auto child = element->FirstChildElement("AmbientLight");
    stream << child->GetText() << std::endl;
    stream >> ambient_light.x >> ambient_light.y >> ambient_light.z;
    element = element->FirstChildElement("PointLight");
    PointLight point_light;
    while (element)
    {
        child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Intensity");
        stream << child->GetText() << std::endl;

        stream >> point_light.position.x >> point_light.position.y >> point_light.position.z;
        stream >> point_light.intensity.x >> point_light.intensity.y >> point_light.intensity.z;

        point_lights.push_back(point_light);
        element = element->NextSiblingElement("PointLight");
    }

    //Get Materials
    element = root->FirstChildElement("Materials");
    element = element->FirstChildElement("Material");
    Material material;
    while (element)
    {
        //material.is_mirror = (element->Attribute("type", "mirror") != NULL);
        material.type = REGULAR;
        if(element->Attribute("type", "mirror") != NULL)
        {
            material.type = REGULAR;
        }
        else if(element->Attribute("type", "conductor") != NULL) 
        {
            material.type = CONDUCTOR;
        }
        else if(element->Attribute("type", "dielectric") != NULL) 
        {
            material.type = DIELECTRIC;
        }


        child = element->FirstChildElement("AmbientReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("DiffuseReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("SpecularReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("MirrorReflectance");
        if(child)
        {
            stream << child->GetText() << std::endl;
        }
        else
        {
            stream << "0 0 0" << std::endl;
        }
        child = element->FirstChildElement("PhongExponent");
        if(child)
        {
            stream << child->GetText() << std::endl;
        }
        else
        {
            stream << "1" << std::endl;
        }

        stream >> material.ambient.x >> material.ambient.y >> material.ambient.z;
        stream >> material.diffuse.x >> material.diffuse.y >> material.diffuse.z;
        stream >> material.specular.x >> material.specular.y >> material.specular.z;
        stream >> material.mirror.x >> material.mirror.y >> material.mirror.z;
        stream >> material.phong_exponent;

        materials.push_back(material);
        element = element->NextSiblingElement("Material");
    }

    //Get VertexData
    element = root->FirstChildElement("VertexData");
    stream << element->GetText() << std::endl;
    vec3f vertex;
    while (!(stream >> vertex.x).eof())
    {
        stream >> vertex.y >> vertex.z;
        vertex_data.push_back(vertex);
    }
    stream.clear();

    //Get Meshes
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Mesh");
    Mesh mesh;
    while (element)
    {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;
        stream >> mesh.material_id;

        child = element->FirstChildElement("Faces");
        stream << child->GetText() << std::endl;
        Face face;
        while (!(stream >> face.v0_id).eof())
        {
            stream >> face.v1_id >> face.v2_id;
            mesh.faces.push_back(face);
        }
        stream.clear();

        meshes.push_back(mesh);
        mesh.faces.clear();
        element = element->NextSiblingElement("Mesh");
    }
    stream.clear();

    //Get Triangles
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Triangle");
    Triangle triangle;
    while (element)
    {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;
        stream >> triangle.material_id;

        child = element->FirstChildElement("Indices");
        stream << child->GetText() << std::endl;
        stream >> triangle.indices.v0_id >> triangle.indices.v1_id >> triangle.indices.v2_id;

        triangles.push_back(triangle);
        element = element->NextSiblingElement("Triangle");
    }

    //Get Spheres
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Sphere");
    Sphere sphere;
    while (element)
    {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;
        stream >> sphere.material_id;

        child = element->FirstChildElement("Center");
        stream << child->GetText() << std::endl;
        stream >> sphere.center_vertex_id;

        child = element->FirstChildElement("Radius");
        stream << child->GetText() << std::endl;
        stream >> sphere.radius;

        spheres.push_back(sphere);
        element = element->NextSiblingElement("Sphere");
    }
}

vec3f parser::Scene::getObjNorm(const IntersectionData& data)
{
    vec3f n;
    vec3f a,b,c;
    switch(data.hitType)
    {
        case SPHERE:
            n = norm(data.intersectionPoint - vertex_data[data.sphereCenterId - 1]);
            break;
        case TRIANGLE://fall through mesh
        case MESH:
            a = vertex_data[data.v0_id - 1]; 
            b = vertex_data[data.v1_id - 1]; 
            c = vertex_data[data.v2_id - 1]; 
            
            n = norm(cross(b-a, c-b));

            break;
        //default:
        //    n = vec3f(0.f);
    }

    return n;
}

vec3f parser::Scene::calculateLighting(Ray eyeRay, Material material, vec3f surfNorm, vec3f p)
{
    //for now only calculate diffuse & spcular shading
    //for only point lights
    vec3f w_eye = -norm(eyeRay.d);
    vec3f color = vec3f(0.f);
    for(PointLight light : point_lights)
    {
        vec3f w_light = (light.position - p);
        float distance = length(w_light); 
        w_light = norm(w_light);
        float d_sqr = distance * distance;

        vec3f irradiance = light.intensity / d_sqr;
        //diffuse shading
        vec3f cosTheta = dot(surfNorm, w_light);
        color += material.diffuse * cosTheta * irradiance;

        //specular shading 
        vec3f half = norm(w_light + w_eye);
        float cosAlpha = dot(half, surfNorm);
        cosAlpha = std::max(pow(cosAlpha, material.phong_exponent), 0.0);

        //cosAlpha = material.phong_exponent
        color += material.specular * cosAlpha * irradiance;
    }

    return color;
}

vec3f parser::Scene::getRayColor(Ray ray, int depth, bool isPrimaryRay)
{
    if(depth < 0)
    {
        return vec3f(0.f, 0.f, 0.f);
    }

    //float min_t = 1e20;
    float min_t = std::numeric_limits<float>::infinity();
    bool hit = false;
    IntersectionData closestObjData;
    closestObjData.material_id = -1;
    closestObjData.t = std::numeric_limits<float>::infinity(); 

    vec3f color = vec3f(0.f);

    for(Sphere sphere : spheres)
    {
        IntersectionData intData;
        if(sphere.intersectRay(vertex_data, ray, intData))
        {
            hit = true;
            if(intData.t < closestObjData.t)
            {
                closestObjData = IntersectionData(intData);
            }
        }

    }

    for(Triangle triangle : triangles)
    {
        IntersectionData intData;
        if(triangle.intersectRay(vertex_data, ray, intData))
        {
            hit = true;
            if(intData.t < closestObjData.t)
            {
                closestObjData = intData;
            }
        }

    }

    for(Mesh mesh: meshes)
    {
        IntersectionData intData;
        if(mesh.intersectRay(vertex_data, ray, intData))
        {
            hit = true;
            if(intData.t < closestObjData.t)
            {
                closestObjData = intData;
            }
        }

    }

    if(hit){
        Material objMaterial = materials[closestObjData.material_id - 1];
        vec3f n = getObjNorm(closestObjData);

        vec3f color = vec3f(0.f);
        color += objMaterial.ambient * ambient_light;
        color += calculateLighting(ray, objMaterial, n, closestObjData.intersectionPoint);        
        
        return color;
    }
    else {
        if(isPrimaryRay) {
            return this->background_color;
        }
        else {
            return vec3f(0, 0, 0);
        }
    }
    

}


void parser::Scene::render(Camera camera)
{
    vec3f e = camera.position;
    vec3f u, v, w;    
    v = norm(camera.up);
    w = norm(-camera.gaze);
    u = norm(cross(v, w));
    v = norm(cross(w, u));

    int nx = camera.image_width;
    int ny = camera.image_height;
    float l = camera.near_plane.x;
    float r = camera.near_plane.y;
    float b = camera.near_plane.z;
    float t = camera.near_plane.w;

    int x,y;
    unsigned char* img = new unsigned char[camera.image_width* camera.image_height * 3];

    vec3f m = e - w * camera.near_distance;
    vec3f q = m + l * u + t * v;

    for(y = 0; y < camera.image_height; ++y)
    {
        for(x = 0; x < camera.image_width; ++x)
        {
            float s_u = (x + 0.5) * (r-l)/float(nx);
            float s_v = (y + 0.5) * (t-b)/float(ny);

            vec3f s = q + s_u * u - s_v * v;
            Ray ray; 
            ray.o = e;
            ray.d = norm(s - e);

            
            vec3f color = getRayColor(ray, max_recursion_depth, true);
            vec3i c = clamp(vec3i(color), 0, 255);
            
            img[(camera.image_width*y + x)*3] = c.r;
            img[(camera.image_width*y + x)*3 + 1] = c.g;
            img[(camera.image_width*y + x)*3 + 2] = c.b;
            
        }
    }
    //write_ppm(camera.image_name.c_str() , img, nx, ny);
    write_png(camera.image_name.c_str() , nx, ny, img);

    delete[] img;
}



void parser::Scene::render(size_t cameraId)
{
}
