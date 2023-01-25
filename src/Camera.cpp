#include "Camera.hpp"

Camera:: Camera()
{
    position = vec3f(std::nan("1"));
    gaze = vec3f(std::nan("1"));
    up = vec3f(std::nan("1"));
    near_plane = vec4f(std::nan("1"));
    near_distance = std::nan("1");

    image_width = image_height = -1;
    image_name = "";
    numSamples = 1;
    focusDistance = -1;
    apertureSize = -1;

    TMOArgs.TMO = TMO_PHOTOGRAPHIC_GLOBAL;
    TMOArgs.keyValue = 0.18;
    TMOArgs.burnPercent = 0.1;
    TMOArgs.saturation = 1.0f;
    TMOArgs.gamma = 2.2f;

    rendererParams.pathTracing = false;
    rendererParams.nextEvent = false;
    rendererParams.importanceSampling = false;
    rendererParams.russianRoulette = false;
}
