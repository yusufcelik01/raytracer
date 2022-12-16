#include "CheckerBoard.hpp"

CheckerBoard::CheckerBoard()
{
    black = vec3f(0.f);
    white = vec3f(1.f);
    scale = 1.f;
    offset = 1.f;
}

CheckerBoard:: CheckerBoard(float s, float o, vec3f b, vec3f w)
{
    black = b;
    white = w;
    scale = s;
    offset = o;
}

vec3f CheckerBoard::sample(float x, float y)
{
    return sample(vec3f(x, y, 0.f));
}

vec3f CheckerBoard::sample(vec3f pos)
{
    bool x = (int) ((pos.x + offset) * scale) % 2;
    bool y = (int) ((pos.y + offset) * scale) % 2;
    bool z = (int) ((pos.z + offset) * scale) % 2;
    bool xorXY = x != y;
    if (xorXY != z)
    {
        return black;
    }
    else
    {
        return white;
    }
}
