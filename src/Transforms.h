
#ifndef Transforms_h
#define Transforms_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "DeepRI.h"

RtVoid RenderroTransformBegin(){
    current_transMatrix = worldMatrix * current_transMatrix;
};

RtVoid RenderroTransformEnd(){
    current_transMatrix = glm::mat4(1.0f); // Identity matrix
    //projectionMatrix = glm::mat4(1.0f);
    UsingCheckBoard = false;
};


glm::mat4 Rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz){
    angle = angle/180.0f*Pi;
    RtFloat d1 = sqrtf(dy*dy + dz*dz) ;
    RtFloat a1 = d1 != (RtFloat)0.0 ? dx/d1 : 1 ;
    RtFloat b1 = d1 != (RtFloat)0.0 ? dy/d1 : 0 ;
    RtFloat c1 = d1 != (RtFloat)0.0 ? dz/d1 : 1 ;
    
    RtFloat d2 = sqrtf(dy*dy + dz*dz + dx*dx);
    RtFloat a2 = d2 != (RtFloat)0.0 ? dx/d2 : 1 ;
    RtFloat b2 = d2 != (RtFloat)0.0 ? dy/d2 : 0 ;
    RtFloat c2 = d2 != (RtFloat)0.0 ? dz/d2 : 1 ;
    RtFloat s = sqrtf(d2*d2 - dx*dx);
    s = s/d2;
    double r = angle ;
    glm::mat4 m1 = glm::mat4(
        1 , 0 , 0 , 0 ,
        0 , c1 , -b1, 0 ,
        0 , b1 , c1 ,0 ,
        0 , 0 , 0 , 1 
    );
    
    glm::mat4 m2 = glm::mat4(
        s , 0 , -a2, 0 ,
        0 , 1 , 0 , 0 ,
        a2 , 0 , s , 0 ,
        0 , 0 , 0 , 1
    );
    
    glm::mat4 m3 = glm::mat4(
        cos(r),-sin(r),0,0,
        sin(r),cos(r), 0,0,
        0     , 0     ,1,0,
        0     , 0     ,0,1
    );
    
    glm::mat4 m4 = glm::mat4(
        s , 0 , a2, 0 ,
        0 , 1 , 0 , 0 ,
        -a2 , 0 , s , 0 ,
        0 , 0 , 0 , 1
    );
    
    glm::mat4 m5 = glm::mat4(
        1 , 0 , 0 , 0 ,
        0 , c1 , b1 ,0 ,
        0 , -b1 , c1 ,0 ,
        0 , 0 , 0 , 1
    );
    
    glm::mat4 resMat;
    resMat = m5*m4*m3*m2*m1;
    return resMat;
}

glm::mat4 Translate(RtFloat dx, RtFloat dy, RtFloat dz){
    glm::mat4 tempTrans = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        dx, dy, dz, 1
    );
    return tempTrans;
}

RtVoid RenderroRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
    current_transMatrix=current_transMatrix*Rotate(angle,dx, dy, dz);
}

RtVoid RenderroTranslate(RtFloat dx, RtFloat dy, RtFloat dz){
    current_transMatrix=Translate(dx, dy, dz)*current_transMatrix;
}

RtVoid RenderroScale(RtFloat sx, RtFloat sy, RtFloat sz){
    glm::mat4 tempTrans = glm::mat4(
        sx, 0, 0, 0,
        0, sy, 0, 0,
        0, 0, sz, 0,
        0, 0, 0, 1
    );
    current_transMatrix = current_transMatrix * tempTrans;
}

RtVoid RenderroWorldBegin(){
    worldMatrix = current_transMatrix;
    current_transMatrix = glm::mat4(1.0f); // Identity matrix
    projectionMatrix = glm::mat4(1.0f); // Identity matrix
}

void castToProjection(newMesh *M){
    
    for (int i= 0;i< (Mesh_Size+1)*(Mesh_Size+1); i++) {
        glm::vec4 temp(M[i].Points[0], M[i].Points[1], M[i].Points[2], 1.0f);
        glm::vec4 res = current_transMatrix * temp;
        M[i].Points[0]  = res[0] ;
        M[i].Points[1]  = res[1] ;
        M[i].Points[2]  = res[2] ;
        
        //new
        temp = glm::vec4(M[i].Points[0], M[i].Points[1], M[i].Points[2], 1.0f);
        //std::cout<<"z" << res[2]<<std::endl;
        res = projectionMatrix * temp;
        res[0] = res[0]/res[3]/2.0f;
        res[1] = res[1]/res[3]/2.0f;
        res[2] = res[2]/res[3]/2.0f;
        //std::cout<<"z" << res[2]<<std::endl;
        M[i].Points[0]  = res[0] ;
        M[i].Points[1]  = res[1] ;
        M[i].Points[2]  = res[2] ;
    }
}

#endif