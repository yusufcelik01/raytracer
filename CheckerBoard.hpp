#ifndef __CHECKERBOARD__DEFINED__
#define __CHECKERBOARD__DEFINED__

#include "Texture.hpp"

class CheckerBoard : public Texture
{
    //private:
    public:
    float scale;
    float offset;
    vec3f black;
    vec3f white;
    public:
    CheckerBoard();
    CheckerBoard(float scale, float offset, vec3f black, vec3f white);
    vec3f sample(float x, float y);
    vec3f sample(vec3f pos);
};
#endif
