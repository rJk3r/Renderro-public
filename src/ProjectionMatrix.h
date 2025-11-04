#ifndef Projection_h
#define Projection_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "DeepRI.h"

RtVoid RenderroProjection(RtToken name, ...){
    projectionMatrix = glm::mat4(1.0f); 
    float fov = 180;
    
    RtFloat A = 1.0f / (RtFloat)tan((fov/2.0f)/180.0f*Pi);
    RtFloat B = 1.0f / (RtFloat)tan((fov/2.0f/aspectRatio)/180.0f*Pi);
    RtFloat C = yon/(yon-hither);
    RtFloat D = (-yon*hither)/(yon-hither);
    
    glm::mat4 persp = glm::mat4(
        A, 0, 0, 0,
        0, B, 0, 0,
        0, 0, C, 1,
        0, 0, D, 0
    );
    projectionMatrix = persp;
}

#endif