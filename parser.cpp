#include "parser.h"
#include "tinyxml2.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <limits>


#define BUFF_SIZE 512

void getPlyLine(std::fstream& strm, char* buffer, size_t size)
{
    strm.getline(buffer, size);     
    while(strncmp(buffer, "comment", 7) == 0)
    {
        strm.getline(buffer, size);     
    }
}

enum plyFormat
{
    ASCII,
    BIN_LITTLE_ENDIAN,
    BIN_BIG_ENDIAN
};

void parsePly(const char* filePath, parser::plyData& plyMesh)
{
    std::ifstream file;
    file.open(filePath);
    plyFormat format;

    char buffer[512];
    file.getline(buffer, 512);
    if(strcmp(buffer, "ply") != 0)
    {
        std::cerr << "ERR: " << filePath << " is not a ply file" << std::endl;
        exit(1);
    }
    file.getline(buffer, 512);
    if(strncmp(buffer, "format ", 7) !=0)
    {
        std::cerr << "ERR: incorrect file format" << std::endl;
        exit(1);
    }
    if(strncmp(&buffer[7], "ascii", 5) == 0)
    {
        format = ASCII;
    }
    else if(strncmp(&buffer[7], "binary_little_endian", 20) == 0)
    {
        format = BIN_LITTLE_ENDIAN;
    }
    else if(strncmp(&buffer[7], "binary_big_endian", 17) == 0)
    {
        format = BIN_BIG_ENDIAN;
    }
    else
    {
        std::cerr << "ERR: incorrect file format" << std::endl;
        exit(1);
    }

    std::stringstream stream;
    size_t vertexCount, faceCount;


    while(1)
    {
        file.getline(buffer, 512);
        stream << buffer;
        std::string tmp;
        stream >> tmp;
        if(tmp == "comment")
        {
            stream.str("");
            continue;
        }
        if(tmp == "element")
        {
            stream >> tmp;
            
            file.getline(buffer, 512);
            file.getline(buffer, 512);
            file.getline(buffer, 512);
            stream >> tmp;
            if(tmp == "property")
            {
                
            }
        }
    }
    //if(tmp != "element")
    //{
    //    exit(1);    
    //}
    //file >> tmp;
    //if(tmp != "vertex")
    //{
    //    exit(1);    
    //}
    //file >> vertexCount; 
    ////consume "property float x/y/z" lines
    //file >> tmp; file >> tmp; file >> tmp; 
    //file >> tmp; file >> tmp; file >> tmp; 
    //file >> tmp; file >> tmp; file >> tmp; 
    //file >> tmp;
    //if(tmp != "element")
    //{
    //    exit(1);    
    //}
    //file >> tmp;
    //if(tmp != "face")
    //{
    //    exit(1);    
    //}
    //file >> faceCount;
    //file >> tmp; file >> tmp; file >> tmp; file >> tmp;file >> tmp;


    



}

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
            material.type = MIRROR;
            child = element->FirstChildElement("MirrorReflectance");
            stream << child->GetText() << std::endl;
            stream >> material.mirror.x >> material.mirror.y >> material.mirror.z;
        }
        else if(element->Attribute("type", "conductor") != NULL) 
        {
            material.type = CONDUCTOR;
            child = element->FirstChildElement("MirrorReflectance");
            stream << child->GetText() << std::endl;
            stream >> material.mirror.x >> material.mirror.y >> material.mirror.z;
            child = element->FirstChildElement("RefractionIndex");
            stream << child->GetText() << std::endl;
            stream >> material.refraction_index;
            child = element->FirstChildElement("AbsorptionIndex");
            stream << child->GetText() << std::endl;
            stream >> material.absorption_index;
        }
        else if(element->Attribute("type", "dielectric") != NULL) 
        {
            material.type = DIELECTRIC;
            child = element->FirstChildElement("AbsorptionCoefficient");
            stream << child->GetText() << std::endl;
            stream >> material.absorption_coefficent.x >> material.absorption_coefficent.y >> material.absorption_coefficent.z;
            child = element->FirstChildElement("RefractionIndex");
            stream << child->GetText() << std::endl;
            stream >> material.refraction_index;
        }


        child = element->FirstChildElement("AmbientReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("DiffuseReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("SpecularReflectance");
        stream << child->GetText() << std::endl;
        //if(child)
        //{
        //    stream << child->GetText() << std::endl;
        //}
        //else
        //{
        //    stream << "0 0 0" << std::endl;
        //}
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
        stream >> material.phong_exponent;

        materials.push_back(material);
        element = element->NextSiblingElement("Material");
    }

    //Get VertexData
    element = root->FirstChildElement("VertexData");
    stream << element->GetText() << std::endl;
    vec3f vertex;
    
    //insert one dummy vertex to make indices start from one
    //this will allow other functions to directly acces the VAO 
    vertex = vec3f(std::numeric_limits<float>::quiet_NaN());
    vertex_data.push_back(vertex);
    while (!(stream >> vertex.x).eof())
    {
        stream >> vertex.y >> vertex.z;
        vertex_data.push_back(vertex);
    }
    stream.clear();

    //Get Meshes
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Mesh");
    Mesh* mesh;
    mesh = new Mesh();
    while (element)
    {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;
        stream >> mesh->material_id;

        child = element->FirstChildElement("Faces");
        //check if it is a ply file
        const char* plyFileName = child->Attribute("plyFile");
        if(plyFileName != NULL)//it is a ply file
        {
            char *plyFilePath;
            if(plyFileName[0] == '~' || plyFileName[0] == '/' )
            {
                strcpy(plyFilePath, plyFileName);//abs path
            }
            else if( index(filepath.c_str(), '/') == NULL)
            {
                strcpy(plyFilePath, plyFileName);
            }
            else
            {
                const char* begin = filepath.c_str();
                const char* end = rindex(begin, '/');
                plyFilePath = new char[end-begin + 128];
                strncpy(plyFilePath, begin, end - begin + 1);
                strncat(plyFilePath, plyFileName, 126);
            }
            //std::cout << plyFilePath << std::endl;
            plyData obj;
            parsePly(plyFilePath, obj);
            
        }

        stream << child->GetText() << std::endl;
        Face* face;
        face = new Face();
        while (!(stream >> face->v0_id).eof())
        {
            stream >> face->v1_id >> face->v2_id;
            mesh->faces.push_back(face);
            face = new Face();
        }
        stream.clear();

        //meshes.push_back(mesh);
        objects.push_back(mesh);//runtime polymorph
        meshes.push_back(mesh);
        mesh = new Mesh();
        element = element->NextSiblingElement("Mesh");
    }
    delete mesh;//one extra was allotaced at the end of the loop
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

        //triangles.push_back(triangle);
        objects.push_back(new Triangle(triangle));//runtime polymorph
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

        //spheres.push_back(sphere);
        objects.push_back(new Sphere(sphere));//runtime polymorph
        element = element->NextSiblingElement("Sphere");
    }
}

