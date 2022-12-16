#include "parser.h"
#include "rtmath.hpp"
#include "ply.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <cstring>
#include <limits>
#include "stb_image.h"


char* navigateDirs(const char* curr_pwd, const char* action)
{
    char* newPath;
    if(action[0] == '~' || action[0] == '/')
    {
        strcpy(newPath, action);
    }
    else if( index(curr_pwd, '/') == NULL)
    {
        strcpy(newPath, action);
    }
    else
    {
        const char* begin = curr_pwd;
        const char* end = rindex(begin, '/');
        size_t newPathSize = end - begin  + 256;
        newPath = new char[newPathSize];
        for(size_t i = 0; i < newPathSize; ++i)
        {
            newPath[i] = '\0';
        }
        strncpy(newPath, begin, end - begin + 1);
        strncat(newPath, action, 254);
    }
    return newPath;
}

void Scene::loadFromXml(const std::string &filepath)
{
    tinyxml2::XMLDocument file;
    std::stringstream stream;

    auto res = file.LoadFile(filepath.c_str());
    if (res)
    {
        throw std::runtime_error("Error: The xml file cannot be loaded.");
    }

    tinyxml2::XMLNode* root = file.FirstChild();
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
    stream.clear();
    element = root->FirstChildElement("Cameras");
    element = element->FirstChildElement("Camera");
    Camera camera;
    while (element)
    {

        auto child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        stream >> camera.position.x >> camera.position.y >> camera.position.z;

        child = element->FirstChildElement("NearDistance");
        stream << child->GetText() << std::endl;
        stream >> camera.near_distance;

        child = element->FirstChildElement("ImageResolution");
        stream << child->GetText() << std::endl;
        stream >> camera.image_width >> camera.image_height;

        child = element->FirstChildElement("ImageName");
        stream << child->GetText() << std::endl;
        stream >> camera.image_name;

        child = element->FirstChildElement("Up");
        stream << child->GetText() << std::endl;
        stream >> camera.up.x >> camera.up.y >> camera.up.z;
        camera.up = norm(camera.up);

        child = element->FirstChildElement("NumSamples");
        if(child) {
            stream << child->GetText() << std::endl;
            stream >> camera.numSamples;
        }
        else {
            camera.numSamples = 1;
        }
            

        child = element->FirstChildElement("FocusDistance");
        if(child) {
            stream << child->GetText() << std::endl;
            stream >> camera.focusDistance;
        }
        else {
            camera.apertureSize = -1.f;
        }

        child = element->FirstChildElement("ApertureSize");
        if(child) {
            stream << child->GetText() << std::endl;
            stream >> camera.apertureSize;
            //std::cout << "apertureSize: " << camera.apertureSize << std::endl;
        }
        else {
            camera.apertureSize = -1.f;
        }

        stream.clear();

        const char* cameraType = element->Attribute("type");
        if(cameraType != NULL && strcmp(cameraType, "lookAt") == 0 )
        {
            child = element->FirstChildElement("GazePoint");
            stream << child->GetText() << std::endl;
            vec3f gazePoint,gazeDir;
            stream >> gazePoint.x >> gazePoint.y >> gazePoint.z;
            camera.gaze = norm(gazePoint - camera.position);

            float fovY, aspectRatio;
            child = element->FirstChildElement("FovY");
            stream << child->GetText() << std::endl;
            stream >> fovY;
            //convert fovy to radians
            fovY = fovY * M_PI / 180.f;
            aspectRatio = float(camera.image_width) / float(camera.image_height) ;


            camera.near_plane.x = - (camera.near_distance * tan(fovY/2.f) * aspectRatio);
            camera.near_plane.y = + (camera.near_distance * tan(fovY/2.f) * aspectRatio);
            camera.near_plane.z = - (camera.near_distance * tan(fovY/2.f));
            camera.near_plane.w = + (camera.near_distance * tan(fovY/2.f));


        }
        else
        {
            child = element->FirstChildElement("Gaze");
            stream << child->GetText() << std::endl;
            stream >> camera.gaze.x >> camera.gaze.y >> camera.gaze.z;

            child = element->FirstChildElement("NearPlane");
            stream << child->GetText() << std::endl;
            stream >> camera.near_plane.x >> camera.near_plane.y >> camera.near_plane.z >> camera.near_plane.w;

        }


        cameras.push_back(camera);
        element = element->NextSiblingElement("Camera");
    }

    //Get Lights
    element = root->FirstChildElement("Lights");

    auto child = element->FirstChildElement("AmbientLight");
    if(child) {
        stream << child->GetText() << std::endl;
        stream >> ambient_light.x >> ambient_light.y >> ambient_light.z;
    }
    else {
        ambient_light = vec3f(0.f);
    }


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

    element = root->FirstChildElement("Lights");
    element = element->FirstChildElement("AreaLight");
    AreaLight areaLight;
    while (element)
    {
        child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        stream >> areaLight.position.x >> areaLight.position.y >> areaLight.position.z;

        child = element->FirstChildElement("Normal");
        stream << child->GetText() << std::endl;
        stream >> areaLight.normal.x >> areaLight.normal.y >> areaLight.normal.z;
        areaLight.normal = norm(areaLight.normal);

        child = element->FirstChildElement("Radiance");
        stream << child->GetText() << std::endl;
        stream >> areaLight.radiance.r >> areaLight.radiance.g >> areaLight.radiance.b;

        child = element->FirstChildElement("Size");
        stream << child->GetText() << std::endl;
        stream >> areaLight.extent;

        area_lights.push_back(areaLight);
        element = element->FirstChildElement("AreaLight");
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
        stream >> material.ambient.x >> material.ambient.y >> material.ambient.z;

        child = element->FirstChildElement("DiffuseReflectance");
        stream << child->GetText() << std::endl;
        stream >> material.diffuse.x >> material.diffuse.y >> material.diffuse.z;

        child = element->FirstChildElement("SpecularReflectance");
        stream << child->GetText() << std::endl;
        stream >> material.specular.x >> material.specular.y >> material.specular.z;
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
        stream >> material.phong_exponent;
        child = element->FirstChildElement("Roughness");
        if(child)
        {
            stream << child->GetText() << std::endl;
            stream >>material.roughness;
        }
        else
        {
            material.roughness = -0.f;
        }


        materials.push_back(material);
        element = element->NextSiblingElement("Material");
    }
    stream.clear();


    parseTextures(root, filepath.c_str());

    //get scaling transformations
    element = root->FirstChildElement("Transformations");
    if(element)
    {
        element = element->FirstChildElement("Scaling");
        mat4x4 M(0.f);
        scalings.push_back(M);//insert a dummy transformation
                              //so indices start from 1
        while(element)
        {
            float Sx, Sy, Sz;
            stream << element->GetText() << std::endl;
            stream >> Sx >> Sy >> Sz;
            M = scale(Sx, Sy, Sz);
            scalings.push_back(M);
            //std::cout << "Scaling: " << Sx << " " << Sy << " " << Sz << std::endl;
            element = element->NextSiblingElement("Scaling");
        }
        stream.clear();


        //get translations
        element = root->FirstChildElement("Transformations");
        element = element->FirstChildElement("Translation");
        M = mat4x4(0.f);
        translations.push_back(M);//insert a dummy transformation
                                  //so indices start from 1
        while(element)
        {
            float Tx, Ty, Tz;
            stream << element->GetText() << std::endl;
            stream >>Tx >> Ty >> Tz;
            M = translate(Tx, Ty, Tz);
            translations.push_back(M);
            //std::cout << "Translation: " << Tx << " " << Ty << " " << Tz << std::endl;
            element = element->NextSiblingElement("Translation");
        }
        stream.clear();

        element = root->FirstChildElement("Transformations");
        element = element->FirstChildElement("Rotation");
        M = mat4x4(0.f);
        rotations.push_back(M);
        while(element)
        {
            float angle, Rx, Ry, Rz;
            stream << element->GetText() << std::endl;
            stream >> angle >> Rx >> Ry >> Rz;
            M = rotate(angle, vec3f(Rx, Ry, Rz));
            rotations.push_back(M);
            //std::cout << "Rotation: " << angle << "degrees, " << Rx << " " << Ry << " " << Rz << std::endl;
            element = element->NextSiblingElement("Rotation"); 
        }
        stream.clear();
    }

    stream.clear();
    //Get VertexData
    element = root->FirstChildElement("VertexData");
    if(element)
    {
        stream << element->GetText() << std::endl;
        vec3f vertex;

        //insert one dummy vertex to make indices start from one
        //this will allow other functions to directly acces the VAO 
        vertex = vec3f(std::numeric_limits<float>::quiet_NaN());
        //vertex_data.push_back(vertex);
        VAO.vertexCoords.push_back(vertex);
        while (!(stream >> vertex.x).eof())
        {
            stream >> vertex.y >> vertex.z;
            //vertex_data.push_back(vertex);
            VAO.vertexCoords.push_back(vertex);
        }
        stream.clear();
    }

    element = root->FirstChildElement("TexCoordData");
    if(element)
    {
        stream << element->GetText() << std::endl;
        vec2f texCoord;

        VAO.textureCoords.push_back(texCoord);
        while(!(stream >> texCoord.s).eof())
        {
            stream >> texCoord.t;
            VAO.textureCoords.push_back(texCoord);
        }
    }
    stream.clear();

    //Get Meshes
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Mesh");
    Mesh* mesh;
    mesh = new Mesh();
    while (element)
    {
        //stream.clear();

        getObjAttributes(element, mesh); 

        child = element->FirstChildElement("Faces");
        //check if it is a ply file
        const char* plyFileName = child->Attribute("plyFile");
        if(plyFileName != NULL)//it is a ply file
        {
            //std::cerr << "ply files are not implemented yet" << std::endl;
            //exit(-1);
            char *plyFilePath;
            //if(plyFileName[0] == '~' || plyFileName[0] == '/' )
            //{
            //    strcpy(plyFilePath, plyFileName);//abs path
            //}
            //else if( index(filepath.c_str(), '/') == NULL)
            //{
            //    strcpy(plyFilePath, plyFileName);
            //}
            //else
            //{
            //    const char* begin = filepath.c_str();
            //    const char* end = rindex(begin, '/');
            //    size_t plyPathStrSize = end - begin + 128;
            //    plyFilePath = new char[plyPathStrSize];
            //    for(size_t foo = 0; foo < plyPathStrSize; ++foo) {
            //        plyFilePath[foo] = '\0';//fill the are with nulls
            //    }
            //    strncpy(plyFilePath, begin, end - begin + 1);
            //    strncat(plyFilePath, plyFileName, 126);
            //}
            //std::cout << plyFilePath << std::endl;
            plyData obj;
            plyFilePath = navigateDirs(filepath.c_str(), plyFileName);
            std::cout << std::string(plyFilePath) << std::endl;
            parsePly(plyFilePath, obj);

            size_t numOfTotalVertices = VAO.vertexCoords.size();
            size_t numOfPlyVertices = obj.vertices.size();
            for(size_t k=0; k < numOfPlyVertices; k++)
            {
               VAO.vertexCoords.push_back(obj.vertices[k]); 
            }
            size_t numOfPlyFaces = obj.triangles.size();
            for(size_t k=0; k < numOfPlyFaces; k++)
            {
                Face* face = new Face(numOfTotalVertices + obj.triangles[k].v0_id,
                                      numOfTotalVertices + obj.triangles[k].v1_id,
                                      numOfTotalVertices + obj.triangles[k].v2_id );
                
                mesh->faces.push_back(face);
            }
        }
        else// a regular non-ply mesh
        {
            const char* vertexOffsetStr = child->Attribute("vertexOffset");
            const char* textureOffsetStr = child->Attribute("textureOffset");
            int vertexOffset = 0;
            int textureOffset = 0;
            if(vertexOffsetStr != NULL)
            {
                vertexOffset = std::stoi(vertexOffsetStr); 
            }
            if(textureOffsetStr != NULL)
            {
                textureOffset = std::stoi(textureOffsetStr); 
            }

            stream << child->GetText() << std::endl;
            Face* face;
            face = new Face();
            while (!(stream >> face->v0_id).eof())
            {
                stream >> face->v1_id >> face->v2_id;
                face->v0_id += vertexOffset;
                face->v1_id += vertexOffset;
                face->v2_id += vertexOffset;

                mesh->faces.push_back(face);
                face = new Face();
            }
            stream.clear();
        }
        


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
        child = element->FirstChildElement("Indices");
        stream << child->GetText() << std::endl;
        stream >> triangle.indices.v0_id >> triangle.indices.v1_id >> triangle.indices.v2_id;

        stream.clear();

        getObjAttributes(element, &triangle);

        //triangles.push_back(triangle);
        objects.push_back(new Triangle(triangle));//runtime polymorph
        element = element->NextSiblingElement("Triangle");
        triangle = Triangle();
    }

    //Get Spheres
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Sphere");
    Sphere sphere;
    while (element)
    {

        child = element->FirstChildElement("Center");
        stream << child->GetText() << std::endl;
        stream >> sphere.center_vertex_id;

        child = element->FirstChildElement("Radius");
        stream << child->GetText() << std::endl;
        stream >> sphere.radius;

        getObjAttributes(element, &sphere);
        stream.clear();
        

        //spheres.push_back(sphere);
        objects.push_back(new Sphere(sphere));//runtime polymorph
        sphere = Sphere();
        element = element->NextSiblingElement("Sphere");
    }

    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("MeshInstance");
    Mesh* instance = new Mesh();
    while (element)
    {
        instance->isInstanced = true;

        int baseMeshId =  element->IntAttribute("baseMeshId", -1);
        baseMeshId--;//indices doesn't start from 0 in input file
        //std::cout << "base mesh id: " << baseMeshId << std::endl;
        instance->baseMesh = meshes[baseMeshId];
        instance->material_id = instance->baseMesh->material_id;
        
        getObjAttributes(element, instance);
        //child = element->FirstChildElement("Material");
        //if(child)
        //{
        //    stream << child->GetText() << std::endl;
        //    stream >> instance->material_id;
        //}

        //child = element->FirstChildElement("MotionBlur");
        //if(child)
        //{
        //    vec3f motionBlur;
        //    stream << child->GetText() << std::endl;
        //    stream >> motionBlur.x >> motionBlur.y >> motionBlur.z;
        //    instance->motionBlur = new vec3f(motionBlur);
        //}

        //child = element->FirstChildElement("Transformations");
        //if(child)
        //{
        //    stream << child->GetText() << std::endl;
        //    std::string transformation;
        //    mat4x4 M = mat4x4(1.f);
        //    bool isTransformed = false;
        //    while(!(stream >> transformation).eof())
        //    {
        //        isTransformed = true;
        //        //std::cout << "Mesh Instance transformation: " << transformation << std::endl;
        //        //if(transformation.substr(1) == "r")
        //        if(transformation.c_str()[0] == 'r')
        //        {
        //            int id = std::stoi(transformation.substr(1, transformation.size()-1));
        //            //std::cout << "R id: " << id << std::endl;
        //            M = rotations[id] * M; 
        //        }
        //        else if(transformation.c_str()[0] == 't')
        //        {
        //            int id = std::stoi(transformation.substr(1, transformation.size()-1));
        //            //std::cout << "T id: " << id << std::endl;
        //            M = translations[id] * M; 
        //        }
        //        else if(transformation.c_str()[0] == 's')
        //        {
        //            int id = std::stoi(transformation.substr(1, transformation.size()-1));
        //            //std::cout << "S id: " << id << std::endl;
        //            M = scalings[id] * M; 
        //        }


        //    }
        //    if(isTransformed)
        //    {
        //        instance->transformation = new mat4x4(M);
        //    }
        //}
        stream.clear();

        const char* resetTransformStr = element->Attribute("resetTransform");
        
        if(resetTransformStr == NULL)
        {
            instance->resetTransform = false;     
        }
        else
        {
            if(strcmp(resetTransformStr, "true") == 0)
            {
                instance->resetTransform = true;
            }
            else
            {
                instance->resetTransform = false;
            }
        }

        

        //objects.push_back(instance);//runtime polymorph
        meshes.push_back(instance);
        objects.push_back(instance);
        instance = new Mesh();
        element = element->NextSiblingElement("MeshInstance");
    }
    delete instance;
    stream.clear();

}

void parsePly(char* filePath, plyData& plyMesh)
{
    /*
       THIS FUNCTION IS CONSTRUCTED BY USING THE CODE IN plytest.c
       by Greg Turk, March 1994
       for details see
http://paulbourke.net/dataformats/ply/
     */
    int i,j,k;
    PlyFile *ply;
    int nelems;
    char **elist;
    int file_type;
    float version;
    int nprops;
    int num_elems;
    PlyProperty **plist;
    PLY_Vertex **vlist;
    PLY_Face **flist;
    char *elem_name;
    int num_comments;
    char **comments;
    int num_obj_info;
    char **obj_info;

    PlyProperty vert_props[] = { /* list of property information for a vertex */
        {strdup("x"), PLY_FLOAT, PLY_FLOAT, offsetof(PLY_Vertex,x), 0, 0, 0, 0},
        {strdup("y"), PLY_FLOAT, PLY_FLOAT, offsetof(PLY_Vertex,y), 0, 0, 0, 0},
        {strdup("z"), PLY_FLOAT, PLY_FLOAT, offsetof(PLY_Vertex,z), 0, 0, 0, 0},
    };
    PlyProperty face_props[] = { /* list of property information for a vertex */
        {strdup("intensity"), PLY_UCHAR, PLY_UCHAR, offsetof(PLY_Face,intensity), 0, 0, 0, 0},
        {strdup("vertex_indices"), PLY_INT, PLY_INT, offsetof(PLY_Face,verts),
            1, PLY_UCHAR, PLY_UCHAR, offsetof(PLY_Face,nverts)},
        {strdup("vertex_index"), PLY_INT, PLY_INT, offsetof(PLY_Face,verts),
            1, PLY_UCHAR, PLY_UCHAR, offsetof(PLY_Face,nverts)},
    };
    /* open a PLY file for reading */
    //ply = ply_open_for_reading("hw1/inputs/ply/dragon_remeshed_fixed.ply", &nelems, &elist, &file_type, &version);
    ply = ply_open_for_reading(filePath, &nelems, &elist, &file_type, &version);

    //printf ("version %f\n", version);
    //printf ("type %d\n", file_type);
    for (i = 0; i < nelems; i++) {

        /* get the description of the first element */
        elem_name = elist[i];
        plist = ply_get_element_description (ply, elem_name, &num_elems, &nprops);

        /* print the name of the element, for debugging */
        printf ("element %s %d\n", elem_name, num_elems);

        /* if we're on vertex elements, read them in */
        if (equal_strings ("vertex", elem_name)) {

            /* create a vertex list to hold all the vertices */
            vlist = (PLY_Vertex **) malloc (sizeof (PLY_Vertex *) * num_elems);

            /* set up for getting vertex elements */

            ply_get_property (ply, elem_name, &vert_props[0]);
            ply_get_property (ply, elem_name, &vert_props[1]);
            ply_get_property (ply, elem_name, &vert_props[2]);

            /* grab all the vertex elements */
            for (j = 0; j < num_elems; j++) 
            {
                /* grab and element from the file */
                vlist[j] = (PLY_Vertex *) malloc (sizeof (PLY_Vertex));
                ply_get_element (ply, (void *) vlist[j]);

                /* print out vertex x,y,z for debugging */
                //printf ("vertex: %g %g %g\n", vlist[j]->x, vlist[j]->y, vlist[j]->z);
                plyMesh.vertices.push_back(vec3f(vlist[j]->x,vlist[j]->y,vlist[j]->z));

            }
        }

        /* if we're on face elements, read them in */
        if (equal_strings ("face", elem_name)) 
        {
            /* create a list to hold all the face elements */
            flist = (PLY_Face **) malloc (sizeof (PLY_Face *) * num_elems);

            /* set up for getting face elements */

            //ply_get_property (ply, elem_name, &face_props[0]);
            ply_get_property (ply, elem_name, &face_props[1]);
            ply_get_property (ply, elem_name, &face_props[2]);
            /* grab all the face elements */
            for (j = 0; j < num_elems; j++) 
            {
                /* grab and element from the file */
                flist[j] = (PLY_Face *) malloc (sizeof (PLY_Face));
                ply_get_element (ply, (void *) flist[j]);

                /* print out face info, for debugging */
                //printf ("face: %d, list = ", flist[j]->intensity);
                for (k = 2; k < flist[j]->nverts; k++)
                {
                    plyMesh.triangles.push_back(Face(flist[j]->verts[0  ], 
                                                     flist[j]->verts[k-1], 
                                                     flist[j]->verts[k  ])); 
                    Face f = plyMesh.triangles[ plyMesh.triangles.size()-1] ;
                    //std::cout << "ply face : " << f.v0_id << ", " <<  f.v1_id << ", " << f.v2_id << std::endl;
                    //std::cout << "END POLYGON" << std::endl;
                    //printf ("%d ", flist[j]->verts[k]);
                }
                //printf ("\n");
            }
        }

        /* print out the properties we got, for debugging */
        //for (j = 0; j < nprops; j++)
        //    printf ("property %s\n", plist[j]->name);
    }
}

void Scene::parseTextures(tinyxml2::XMLNode* sceneNode, const char* inputFileDir)
{
    std::stringstream stream;
    tinyxml2::XMLElement* root = sceneNode->FirstChildElement("Textures"); 
    root = sceneNode->FirstChildElement("Textures"); 
    if(root == NULL) {//no textures
        return;
    }
    
    tinyxml2::XMLElement* child, *element; 
    element = root->FirstChildElement("Images");
    if(element)
    {
        element = element->FirstChildElement("Image");
        Image image;
        while(element)
        {
            //std::cout << element->GetText() << std::endl;
            char* imgPath = navigateDirs(inputFileDir, element->GetText());
            //printf("Tex image path : %s\n", imgPath);
            image.data = stbi_load(imgPath, &(image.width), &(image.height), &(image.numOfChannels), 0);
            //TODO write image and texmap classes and replace this line

            images.push_back(image);
            element = element->NextSiblingElement("Image");
        }

        //get texture maps
    }

    element = root->FirstChildElement("TextureMap");
    Texture* tex = NULL;
    while(element)
    {
        stream.clear();
        //std::cout << "New Texture" << std::endl;

        const char* texMapType = element->Attribute("type");
        if(texMapType != NULL && strcmp(texMapType, "perlin") == 0)
        {
            //TODO check type of texture (perlin/img)
            float noiseScale;
            std::string noiseConversion;
            child = element->FirstChildElement("NoiseScale");
            stream << child->GetText() << std::endl;
            stream >> noiseScale;
           
            //stream >> noise->noiseScale;
            child = element->FirstChildElement("NoiseConversion");
            stream << child->GetText() << std::endl;
            stream >> noiseConversion;

            PerlinNoise* noise = new PerlinNoise(noiseScale, noiseConversion);
            tex = noise;
        }
        else if(texMapType != NULL && strcmp(texMapType, "checkerboard") == 0)
        {
            //continue;
            //std::cout << "checkerboard texture is not supported yet" << std::endl;
            //exit(-1);
            float scale, offset;
            vec3f black, white;
            child = element->FirstChildElement("Scale");
            stream << child->GetText() << std::endl;
            stream >> scale;
            child = element->FirstChildElement("Offset");
            stream << child->GetText() << std::endl;
            stream >> offset;
            child = element->FirstChildElement("BlackColor");
            stream << child->GetText() << std::endl;
            stream >> black.r >> black.g >> black.b;
            child = element->FirstChildElement("WhiteColor");
            stream << child->GetText() << std::endl;
            stream >> white.r >> white.g >> white.b;
            CheckerBoard* checker = new CheckerBoard(scale, offset, black, white);
            tex = checker;
        }
        else if(texMapType != NULL && strcmp(texMapType, "image") == 0)
        {
            ImageTexture* imgTex = new ImageTexture();
            child = element->FirstChildElement("ImageId");
            std::cout << child->GetText() << std::endl;
            stream << child->GetText() << std::endl;
            int imgId;
            stream >> imgId;
            imgTex->img = images[imgId-1];


            //TODO perlin noise do not have interpolation attribute
            child = element->FirstChildElement("Interpolation");
            std::string str;
            if(child)
            {
                stream << child->GetText() << std::endl;
            }
            else
            {
                stream << "bilinear" << std::endl;
            }
            stream >> str;

            if(str == "bilinear")
            {
                //std::cout << "INTERPOLATE BI LINEAR" << std::endl;
                imgTex->interpolationType = INTERPOLATE_BI_LINEAR;
            }
            else if(str == "nearest")
            {
                //std::cout << "INTERPOLATE NEAREST" << std::endl;
                imgTex->interpolationType = INTERPOLATE_NEAREST;
            }

            tex = imgTex;
        }
        else//undefined texture type
        {
            std::cout << "IGNORING UNKNOWN TEXTURE TYPE: " << texMapType << std::endl;
            element = element->NextSiblingElement("TextureMap");
            continue;
        }

        child = element->FirstChildElement("DecalMode");
        if(child)
        {
            stream << child->GetText() << std::endl;
        }
        else
        {
            stream << "replace_kd";//deafult value 
        }
        std::string decalMode;
        stream >> decalMode;
        if(decalMode == "replace_kd")
        {
            tex->decalMode = TEX_MODE_REPLACE_KD;
        }
        else if(decalMode == "replace_ks")
        {
            tex->decalMode = TEX_MODE_REPLACE_KS;
        }
        else if(decalMode == "blend_kd")
        {
            tex->decalMode = TEX_MODE_BLEND_KD;
        }
        else if(decalMode == "replace_background")
        {
            tex->decalMode = TEX_MODE_REPLACE_BACKGROUND;
            background_texture = tex;
        }


        //TODO write image and texmap classes and replace this line
        child = element->FirstChildElement("BumpFactor");
        if(child) {
            stream << child->GetText() << std::endl;
        }
        else {
            stream << 1;
        }
        stream >> tex->bumpFactor;

        textures.push_back(tex);
        tex = NULL;
        //next element
        element = element->NextSiblingElement("TextureMap");
    }


    
    tinyxml2::XMLNode* imgNode;
}

void Scene::getObjAttributes(tinyxml2::XMLNode* element, Object* obj)
{
    std::stringstream stream;

    auto child = element->FirstChildElement("Material");
    stream << child->GetText() << std::endl;
    stream >> obj->material_id;
    obj->material_id--;
    obj->material = materials[obj->material_id];

    child = element->FirstChildElement("MotionBlur");
    if(child)
    {
        vec3f motionBlur;
        stream << child->GetText() << std::endl;
        stream >> motionBlur.x >> motionBlur.y >> motionBlur.z;
        obj->motionBlur = new vec3f(motionBlur);
    }

    child = element->FirstChildElement("Textures");
    if(child)
    {
        stream << child->GetText() << std::endl;
        int texId = -1;
        while(!(stream >> texId).eof())
        {
            obj->textures.push_back(textures[texId-1]);
        }
    }

    stream.clear();
    child = element->FirstChildElement("Transformations");
    if(child)
    {
        stream << child->GetText() << std::endl;
        std::string transformation;
        mat4x4 M = mat4x4(1.f);
        bool isTransformed = false;
        while(!(stream >> transformation).eof())
        {
            isTransformed = true;
            //std::cout << "mesh transformation: " << transformation << std::endl;
            //if(transformation.substr(1) == "r")
            if(transformation.c_str()[0] == 'r')
            {
                int id = std::stoi(transformation.substr(1, transformation.size()-1));
                //std::cout << "R id: " << id << std::endl;
                M = rotations[id] * M; 
            }
            else if(transformation.c_str()[0] == 't')
            {
                int id = std::stoi(transformation.substr(1, transformation.size()-1));
                //std::cout << "T id: " << id << std::endl;
                M = translations[id] * M; 
            }
            else if(transformation.c_str()[0] == 's')
            {
                int id = std::stoi(transformation.substr(1, transformation.size()-1));
                //std::cout << "S id: " << id << std::endl;
                M = scalings[id] * M; 
            }
            //std::cout << " MATRIX M" << std::endl;
            //for (int i = 0; i < 4; i++) {
            //    for (int j = 0; j < 4; j++) {
            //        std::cout << M.m[i][j] << "  ";
            //    }
            //    std::cout << std::endl;
            //}
        }
        if(isTransformed)
        {
            obj->transformation = new mat4x4(M);
        }
    }
    stream.clear();
}


