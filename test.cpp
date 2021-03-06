#include<iostream>
#include<cmath>

#include "raytracer_math.h"
//#include "vector_math.h"

void printVector(parser::Vec3f v)
{
    std::cout << v.x <<", " <<v.y <<", " << v.z  << std::endl;
}

int main()
{
    parser::Vec3f f1,f2 ,fr;
    parser::Vec3i i1,i2 ,ir;
    float ff;

    f1.x = 3; f1.y = 4; f1.z = 5;
    f2.x = 1  ; f2.y = 2; f2.z = -3;
    i1.x = 1  ; i1.y = 0; i1.z = 0;
    i2.x = 0  ; i2.y = 1; i2.z = 0;

    printVector(f1 + f2);

    f1 -= f2;
    std::cout << f1 << 5 * f1 <<std::endl;
    std::cout << f2-f1 << std::endl;

    //fr = elementViseMultiply(f1,f2);
    //std::cout << fr.x <<" " <<fr.y <<" " << fr.z  << std::endl;

    //fr = vectorSum(f1,f2);
    //std::cout << fr.x <<" " <<fr.y <<" " << fr.z  << std::endl;

    //fr = crossProduct(f2,f1);
    //std::cout <<"crossProduct "<< fr.x <<" " <<fr.y <<" " << fr.z  << std::endl;

    //ff = dotProduct(f1,f1);
    //std::cout << "dotProduct " << ff << std::endl;

    //ff = dotProduct(f1,f1);
    //std::cout << "dotProduct " << ff << std::endl;

    // 
    //fr = normalize(f1);
    //std::cout <<"normalized "<< fr.x <<" " <<fr.y <<" " << fr.z  << std::endl;
    //

    //ff = VECTOR_LENGTH(f1);
    //std::cout << "dotProduct " << ff << std::endl;

    printVector(clampColor(f1));
    printVector(clampColor(f2));
    return 0;
}
