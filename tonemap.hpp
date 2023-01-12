#ifndef HDR_TMO_DEFINED
#define HDR_TMO_DEFINED

namespace tonemap
{   
    unsigned char* photographicToneMap(float* data, int w, int h, int comp, float keyValue, float burnPercent, float saturation, float gamma); 
}

#endif
