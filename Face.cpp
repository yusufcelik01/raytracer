#include "Face.hpp"
#include "Matrix.hpp"
#include <cmath>

Face::Face()
{
    vertexId[0] = -1;
    vertexId[1] = -1;
    vertexId[2] = -1;
    normal = NULL;
    TBN = NULL;
    //material_id = -1;
    //bbox = NULL;
}

Face::Face(size_t v0, size_t v1, size_t v2)
{
    vertexId[0] = v0;
    vertexId[1] = v1;
    vertexId[2] = v2;
    material_id = -1;
    bbox = NULL;
    TBN = NULL;
    normal = NULL;
}

Face::Face(const Face& rhs) : Object(rhs)
{
    vertexId[0] = rhs.vertexId[0];
    vertexId[1] = rhs.vertexId[1];
    vertexId[2] = rhs.vertexId[2];
    material_id = rhs.material_id;
    if(rhs.TBN == NULL){
        TBN = NULL;
    }
    else {
        TBN = new Matrix(*(rhs.TBN));
    }
    if(rhs.normal == NULL){
        normal = NULL;
    }
    else {
        normal = new vec3f(*(rhs.normal));
    }

}

Face::~Face()
{
}


Matrix Face::getTBN(const VertexBuffers& buffers)
{
    //if(TBN != NULL)
    //{
    //    return *TBN;
    //}
    //vec3f e1 = vertexBuffers.vertexCoords[vertexId[0]];
    //vec3f e2 = vertexBuffers.vertexCoords[vertexId[1]];
    //vec3f e3 = vertexBuffers.vertexCoords[vertexId[2]];

    ////cache normal
    //normal = new vec3f(norm(cross(e2-e1, e3-e2)));
    //
    ////cache TBN
    //vec2f t1 = vertexBuffers.textureCoords[vertexId[0]];
    //vec2f t2 = vertexBuffers.textureCoords[vertexId[1]];
    //vec2f t3 = vertexBuffers.textureCoords[vertexId[2]];

    ////normal map calc
    //Matrix invTex(2,2);
    //invTex.data[0][0] = t2.s - t1.s;
    //invTex.data[0][1] = t2.t - t1.t;
    //invTex.data[1][0] = t3.s - t2.s; 
    //invTex.data[1][1] = t3.t - t2.t; 

    //invTex = invTex.inverse();
    //Matrix edgeMatrix(2,3);

    //for(size_t m = 0; m < 3; m++)
    //{
    //    edgeMatrix.data[0][m] = e1[m];
    //}
    //for(size_t m = 0; m < 3; m++)
    //{
    //    edgeMatrix.data[1][m] = e2[m];
    //}

    //TBN = new Matrix(3,3);
    //Matrix tangentMat = invTex * edgeMatrix;
    //for(int i = 0; i < 3; ++i)
    //{
    //    TBN.data[i][0] = tangentMat.data[0][i];//T
    //    TBN.data[i][1] = tangentMat.data[1][i];//B
    //    TBN.data[i][2] = normal[i];//N
    //}

    return *TBN;
}

void Face::cacheData(const VertexBuffers& vertexBuffers)
{
    vec3f v1 = vertexBuffers.vertexCoords[vertexId[0]];
    vec3f v2 = vertexBuffers.vertexCoords[vertexId[1]];
    vec3f v3 = vertexBuffers.vertexCoords[vertexId[2]];

    //cache normal
    normal = new vec3f(norm(cross(v2-v1, v3-v2)));
    
    //cache TBN
    if(vertexBuffers.textureCoords.size() > 1)
    {
        vec3f e1 = v2 - v1;
        vec3f e2 = v3 - v2;


        vec2f t1 = vertexBuffers.textureCoords[vertexId[0]];
        vec2f t2 = vertexBuffers.textureCoords[vertexId[1]];
        vec2f t3 = vertexBuffers.textureCoords[vertexId[2]];

        //normal map calc
        Matrix invTex(2,2);
        invTex.data[0][0] = t2.s - t1.s;
        invTex.data[0][1] = t2.t - t1.t;
        invTex.data[1][0] = t3.s - t2.s; 
        invTex.data[1][1] = t3.t - t2.t; 

        invTex = invTex.inverse();
        Matrix edgeMatrix(2,3);

        for(size_t m = 0; m < 3; m++)
        {
            edgeMatrix.data[0][m] = e1[m];
        }
        for(size_t m = 0; m < 3; m++)
        {
            edgeMatrix.data[1][m] = e2[m];
        }

        TBN = new Matrix(3,3);
        Matrix tangentMat = invTex * edgeMatrix;
        //vec3f T = norm(vec3f(tangentMat.data[0][0], tangentMat.data[0][1], tangentMat.data[0][2]));
        //vec3f B = norm(vec3f(tangentMat.data[1][0], tangentMat.data[1][1], tangentMat.data[1][2]));
        vec3f T = (vec3f(tangentMat.data[0][0], tangentMat.data[0][1], tangentMat.data[0][2]));
        vec3f B = (vec3f(tangentMat.data[1][0], tangentMat.data[1][1], tangentMat.data[1][2]));

        for(int i = 0; i < 3; ++i)
        {
            //TBN->data[i][0] = tangentMat.data[0][i];//T
            //TBN->data[i][1] = tangentMat.data[1][i];//B
            TBN->data[i][0] = T[i];
            TBN->data[i][1] = B[i];
            TBN->data[i][2] = (*normal)[i];//N
        }
    }
}


//bool Face::intersectRay(const std::vector<vec3f>& VAO, const Ray& r, IntersectionData& intData)
bool Face::intersectRay(const VertexBuffers& vertexBuffers, const Ray& r, IntersectionData& intData)
{
        //vec3f a = VAO[v[0] -1];
        //vec3f b = VAO[v[1] -1];
        //vec3f c = VAO[v[2] -1];
        vec3f a = vertexBuffers.vertexCoords[vertexId[0]];
        vec3f b = vertexBuffers.vertexCoords[vertexId[1]];
        vec3f c = vertexBuffers.vertexCoords[vertexId[2]];
        
        float ax_bx = a.x - b.x;
        float ay_by = a.y - b.y;
        float az_bz = a.z - b.z;

        float ax_cx = a.x - c.x;
        float ay_cy = a.y - c.y;
        float az_cz = a.z - c.z;

        float ax_ox = a.x - r.o.x;
        float ay_oy = a.y - r.o.y;
        float az_oz = a.z - r.o.z;

        

        float detA = ax_bx * (ay_cy * r.d.z - az_cz * r.d.y) 
                   + ay_by * (r.d.x * az_cz - ax_cx * r.d.z) 
                   + az_bz * (ax_cx * r.d.y - ay_cy * r.d.x);
        if( detA == 0)
        {
            return false;
        }

        float beta = ( ax_ox * (ay_cy * r.d.z - r.d.y * az_cz)
                     + ay_oy * (r.d.x * az_cz - ax_cx * r.d.z)
                     + az_oz * (ax_cx * r.d.y - ay_cy * r.d.x)
                     ) / detA;

        float gamma = (  ax_bx * (ay_oy * r.d.z - az_oz * r.d.y)
                       + ay_by * (r.d.x * az_oz - ax_ox * r.d.z)
                       + az_bz * (ax_ox * r.d.y - ay_oy * r.d.x)
                      ) / detA;

        float t = ( ax_bx * (ay_cy * az_oz - ay_oy * az_cz)
                  + ay_by * (ax_ox * az_cz - ax_cx * az_oz)
                  + az_bz * (ax_cx * ay_oy - ay_cy * ax_ox)
                  )/ detA;
        if( beta + gamma <= 1.f
            && 0.f <= beta
            && 0.f <= gamma
            && t > 0.f) 
        {
            intData.t = t;
            //intData.vertexId[0] = vertexId[0];
            //intData.vertexId[1] = vertexId[1];
            //intData.vertexId[2] = vertexId[2];
            intData.material_id = this->material_id;
            intData.intersectionPoint = r.o + r.d * t;


            if(normal == NULL)
            {
                vec3f v1 = vertexBuffers.vertexCoords[vertexId[0]];
                vec3f v2 = vertexBuffers.vertexCoords[vertexId[1]];
                vec3f v3 = vertexBuffers.vertexCoords[vertexId[2]];

                intData.normal = norm(cross(v2-v1, v3-v2));
            }
            else
            {
                intData.normal = *normal;
            }

            if(vertexBuffers.textureCoords.size() > 1)
            {
                vec2f t1 = vertexBuffers.textureCoords[vertexId[0]];
                vec2f t2 = vertexBuffers.textureCoords[vertexId[1]];
                vec2f t3 = vertexBuffers.textureCoords[vertexId[2]];

                intData.texCoord = (1-beta-gamma) * t1 + 
                                    beta * t2 + 
                                    gamma * t3;
                intData.TBN = *TBN;
                //TODO move these lines to processTexture
                vec3f T = vec3f((*TBN).data[0][0],
                                (*TBN).data[1][0],
                                (*TBN).data[2][0]);

                vec3f B = vec3f((*TBN).data[0][1],
                                (*TBN).data[1][1],
                                (*TBN).data[2][1]);

                intData.dp_du = intData.intersectionPoint + vertexBuffers.epsilon * T;
                intData.dp_dv = intData.intersectionPoint + vertexBuffers.epsilon * B;
                /////////////////////////////////////////
            }

            
            
            return true;
        }

    
    return false;
}
vec3f Face::getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const 
{
    vec3f a = VAO[vertexId[0]];
    vec3f b = VAO[vertexId[1]];
    vec3f c = VAO[vertexId[2]];

    return norm(cross(b-a, c-b));
}

int Face::getMaterialId()
{
    return this->material_id;
}
BoundingBox* Face::getBoundingBox(const std::vector<vec3f>& VAO)
{
    if(bbox != NULL){
        return bbox;
    }

    vec3f v0 = VAO[vertexId[0]],
          v1 = VAO[vertexId[1]],
          v2 = VAO[vertexId[2]];
    
    bbox = new BoundingBox();
    bbox->xmin = std::min(std::min(v0.x, v1.x), v2.x);
    bbox->xmax = std::max(std::max(v0.x, v1.x), v2.x);

    bbox->ymin = std::min(std::min(v0.y, v1.y), v2.y);
    bbox->ymax = std::max(std::max(v0.y, v1.y), v2.y);

    bbox->zmin = std::min(std::min(v0.z, v1.z), v2.z);
    bbox->zmax = std::max(std::max(v0.z, v1.z), v2.z);

    return bbox;
}

BoundingBox* Face::getBoundingBox() const
{
    return bbox;
}
