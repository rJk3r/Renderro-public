//
//  Renderro.cpp
//  CS285_Assignment_2.2


#include "Renderro.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Renderro.hpp"
#include "DeepRI.h"
#include "math.h"
#include "Transformation.h"
#include "Primes.h"
#include "Projection.h"
#include "Display.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <opencv2/opencv.hpp>

RtFloat aspectRatio=4.0f/3.0f;
glm::mat4 projectionMatrix;
glm::mat4 transformMatrix;
glm::mat4 current_transMatrix;
glm::mat4 worldMatrix;

glm::vec3 current_Color;
glm::vec3 current_Opacity;

bool Bumpy = false;
bool UsingTexture = false;
bool UsingCheckBoard = false;
bool UsingOpicity = false;

RtFloat yon=50;
RtFloat hither=-10;
RtInt w;
RtInt h;

int CHECK_SIZE_X =5;
int CHECK_SIZE_Y =5;


RtFloat BUMP_AMPLITUDE=0;
RtFloat BUMP_MIN_FREQ_EXP=0;
RtFloat BUMP_MAX_FREQ_EXP=0;
RtToken BUMPY;
RtToken RENDERRO_PERSPECTIVE;


int numOfMicoPolygons =0;
//320 240
newFrameBuffer myBuffer(400,300);
//newFrameBuffer mySampleBuffer(640,640);
newFrameBuffer mySampleBuffer(400,300);
newMicoPolygon myMicoPolygon[100000];
newMesh myMesh[Mesh_Size*Mesh_Size];
Light myLight;
std::vector<glm::vec3> myTexture;


void formatMicopolygon(int u, int v, newMesh *m){
    
    for (int i =0; i< v; i++) {
        for (int j =0; j<u; j++) {
            /*
             micoPolygons[numOfMicoPolygons].points[0] = &m[i*v+j];
             micoPolygons[numOfMicoPolygons].points[1] = &m[i*v+j+1];
             micoPolygons[numOfMicoPolygons].points[2] = &m[(i+1)*v+j];
             micoPolygons[numOfMicoPolygons].points[3] = &m[(i+1)*v+1+j];
             */
            
            myMicoPolygon[numOfMicoPolygons].castMicoPolygon(&m[i*v+j], &m[i*v+j+1], &m[(i+1)*v+j+1], &m[(i+1)*v+j]);
            //new
            myMicoPolygon[numOfMicoPolygons].Meshs[0].Normal = myMicoPolygon[numOfMicoPolygons].getNormal(0, 0);
            numOfMicoPolygons++;
        }
    }
    
    
}

glm::vec3 castToTexture(int u, int v, int numofX, int numofY){
    float x,y;
    glm::vec3 temp(0,0,255);
    x = ((float)u/numofX);
    y = ((float)v/numofY);
    std::cout<<(int)(x*1024) <<","<<(int)(y*512)<<std::endl;
    //if((int)(x*1024+(int)(y*512*1024))<=30 ||abs((int)(x*1024+(int)(y*512*1024))-1024/4*3)<=30||abs((int)(x*1024+(int)(y*512*1024))-1024/4)<=30|| abs((int)(x*1024+(int)(y*512*1024))-1024/2)<=10)
    //return temp;
    //else
    return myTexture[((int)(x*1024)+((int)(y*512))*1024)];
}

RtVoid RenderroBegin(RtToken name){
    
}

RtVoid RtFormat (RtInt a, RtInt b, RtFloat asp){
    std::string filename = "output.png";
    glm::vec3 Pos ;
    glm::vec3 Col(0,0,0);
    for(float i =0; i<400;i++){
        for (float j =0; j<400; j++) {
            Pos = glm::vec3(i/200.0f-1.0f, 1.0f-j/200.0f, 20.0f);
            myBuffer.set_pixel(Pos, Col,255);
        }
    }
}

RtVoid RenderroDisplacement(RtToken name,...){
    if (name != NULL)
    {
        Bumpy = true;
    }
}

RtVoid RenderroOpacity(RtColor color){
    current_Opacity[0] = color[2];
    current_Opacity[1] = color[1];
    current_Opacity[2] = color[0];
}

RtVoid RenderroSurface(void (*earthShader)(void)){
    glm::vec3 lightPos(1,1,-1);
    glm::vec3 lightCol(255,255,255);
    
    myLight.setLight(lightPos, lightCol,0.9, 0.2, 1);
    UsingCheckBoard = true;
}


RtVoid RenderroMakeTexture(char *pic, int a, ...){
    UsingTexture = true;
    // cv::Mat image = cv::imread("/Users/kaxiangming/CS285_Assignment_2.1/CS285_Assignment_2.1/earth.jpg", CV_LOAD_IMAGE_UNCHANGED);
    // uchar* myimage = image.data;
    
    // myTexture.resize(1024 * 512);
    // for (int i = 0; i<512; i++) {
    //     for (int  j=0; j<1024; j++) {
    //         myTexture[i*1024+j] = glm::vec3(myimage[3*(i*1024+j)],myimage[1+3*(i*1024+j)],myimage[2+3*(i*1024+j)]);
    //     }
    // }
}

void texture(int a){
    
}

void checkBoard(){
    UsingCheckBoard = true;
    UsingTexture = false;
    std::cout <<"using checkboard"<<std::endl;
}

void (*CHECKERBOARD)(void) = checkBoard;