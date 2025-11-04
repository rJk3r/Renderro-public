#ifndef Display_h
#define Display_h
#include <opencv2/opencv.hpp>

RtVoid RenderroFormat(RtInt xres, RtInt yres, RtFloat aspect)
{
    
}

RtVoid RenderroEnd()
{
    
}

RtVoid RenderroShutter(RtFloat min, RtFloat max){
    
}


RtVoid RenderroPixelSamples(RtFloat xsamples, RtFloat ysamples){
    
    
}

RtVoid RenderroWorldEnd(void){
    
}

RtVoid RenderroDisplay(char *name, RtToken type, RtToken mode, ...){
    
}

RtVoid RenderroFrameBegin(RtInt frame){
    
    current_transMatrix.setIdentity();
    projectionMatrix.setIdentity();
    worldMatrix.setIdentity();
    
}


bool insideTriangle(float x, float y, const Vector3f* _v)
{
    // TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
    Vector3f P ( x,y,0.0 );
    Vector3f V1 ( P[0] - _v[0][0],P[1] - _v[0][1],0 );
    Vector3f V2 ( P[0] - _v[1][0],P[1] - _v[1][1],0 );
    Vector3f V3 ( P[0] - _v[2][0],P[1] - _v[2][1],0 );
    
    float A = (V1[0] * V2[1] - V1[1] * V2[0]);
    float B = (V2[0] * V3[1] - V2[1] * V3[0]);
    float C = (V3[0] * V1[1] - V3[1] * V1[0]);
    if ((A <= 0 && B <= 0 && C <= 0) || (A >= 0 && B >= 0 && C >= 0))
        return true;
    else
        return false;
}

bool insideMicoPolygon(float x, float y, newMicoPolygon MP)
{
    
    float max_X = fmax(fmax(fmax(MP.Meshs[0].Points[0],MP.Meshs[1].Points[0]),MP.Meshs[2].Points[0]),MP.Meshs[3].Points[0]);
    
    float min_X =
    fmin(fmin(fmin(MP.Meshs[0].Points[0],MP.Meshs[1].Points[0]),MP.Meshs[2].Points[0]),MP.Meshs[3].Points[0]);
    float max_Y = fmax(fmax(fmax(MP.Meshs[0].Points[1],MP.Meshs[1].Points[1]),MP.Meshs[2].Points[1]),MP.Meshs[3].Points[1]);
    float min_Y = fmin(fmin(fmin(MP.Meshs[0].Points[1],MP.Meshs[1].Points[1]),MP.Meshs[2].Points[1]),MP.Meshs[3].Points[1]);
    if( x< max_X && x> min_X && y< max_Y && y> min_Y)
    {
        Vector3f V1[3];
        V1[0]=MP.Meshs[0].Points;
        V1[1]=MP.Meshs[1].Points;
        V1[2]=MP.Meshs[2].Points;
        Vector3f V2[3];
        V2[0]=MP.Meshs[0].Points;
        V2[1]=MP.Meshs[2].Points;
        V2[2]=MP.Meshs[3].Points;
        if (insideTriangle(x, y, V1)||insideTriangle(x, y, V2)) {
            return true;}
    }
    return false;
}


RtVoid RenderroFrameEnd(){
    MyDisplay();
    int key =0,frame_count=0;
    while(key != 27)
    {//240 320
        cv::Mat image(300, 400, CV_32FC3, myBuffer.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        //cv::Mat image(512, 1024, CV_32FC3, myTexture().data());
        //image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);
        
        //std::cout << "frame count: " << frame_count++ << '\n';
        
    }
}

Vector3f colorInterpolation(newMicoPolygon M, Vector3f Pos){
    Vector3f Color[4];
    Color[0]<<M.Meshs[0].Color;
    Color[1]<<M.Meshs[1].Color;
    Color[2]<<M.Meshs[2].Color;
    Color[3]<<M.Meshs[3].Color;
    Pos[2]=0;
    Vector3f Triangle1[3];
    Triangle1[0]<<M.Meshs[0].Points;
    Triangle1[1]<<M.Meshs[1].Points;
    Triangle1[2]<<M.Meshs[3].Points;
    Triangle1[0][2] =0;
    Triangle1[1][2] =0;
    Triangle1[2][2] =0;
    Vector3f Triangle2[3];
    Triangle2[0] << M.Meshs[1].Points ;
    Triangle2[1] << M.Meshs[2].Points ;
    Triangle2[2] << M.Meshs[3].Points;
    Triangle2[0][2] =0;
    Triangle2[1][2] =0;
    Triangle2[2][2] =0;
    if(insideTriangle(Pos[0], Pos[1],Triangle1 )){
        float A = abs(((Triangle1[0] - Triangle1[1]).cross(Triangle1[2]-Triangle1[1])).norm());
        float A1 = abs(((Triangle1[0] - Pos).cross(Triangle1[2]-Pos)).norm());
        float A2 = abs(((Triangle1[1] - Pos).cross(Triangle1[2]-Pos)).norm());
        float A3 = abs(((Triangle1[0] - Pos).cross(Triangle1[1]-Pos)).norm());
        return (A1*Color[1]+A2*Color[0]+A3*Color[3])/A;
    }
    else {
        float A = abs(((Triangle2[0] - Triangle2[1]).cross(Triangle2[2]-Triangle2[1])).norm());
        float A1 = abs(((Triangle2[0] - Pos).cross(Triangle2[2]-Pos)).norm());
        float A2 = abs(((Triangle2[1] - Pos).cross(Triangle2[2]-Pos)).norm());
        float A3 = abs(((Triangle2[1] - Pos).cross(Triangle2[0]-Pos)).norm());
        return (A1*Color[2]+A2*Color[1]+A3*Color[3])/A;
    }
    return M.Meshs[0].Color;
    std::cout <<"no" <<std::endl;
}

Vector3f normalInterpolation(newMicoPolygon M, Vector3f Pos){
    Vector3f Normal[4];
    Normal[0]<<M.Meshs[0].Normal;
    Normal[1]<<M.Meshs[1].Normal;
    Normal[2]<<M.Meshs[2].Normal;
    Normal[3]<<M.Meshs[3].Normal;
    
    Vector3f Triangle1[3];
    Triangle1[0]<<M.Meshs[0].Points;
    Triangle1[1]<<M.Meshs[1].Points;
    Triangle1[2]<<M.Meshs[3].Points;
    Triangle1[0][2] =0;
    Triangle1[1][2] =0;
    Triangle1[2][2] =0;
    Vector3f Triangle2[3];
    Triangle2[0] << M.Meshs[1].Points;
    Triangle2[1] << M.Meshs[2].Points;
    Triangle2[2] << M.Meshs[3].Points;
    Triangle2[0][2] =0;
    Triangle2[1][2] =0;
    Triangle2[2][2] =0;
    if(insideTriangle(Pos[0], Pos[1],Triangle1 )){
        float A = abs(((Triangle1[0] - Triangle1[1]).cross(Triangle1[2]-Triangle1[1])).norm());
        float A1 = abs(((Triangle1[0] - Pos).cross(Triangle1[2]-Pos)).norm());
        float A2 = abs(((Triangle1[1] - Pos).cross(Triangle1[2]-Pos)).norm());
        float A3 = abs(((Triangle1[0] - Pos).cross(Triangle1[1]-Pos)).norm());
        return (A1*Normal[1]+A2*Normal[0]+A3*Normal[3])/A;
    }
    else {
        float A = abs(((Triangle2[0] - Triangle2[1]).cross(Triangle2[2]-Triangle2[1])).norm());
        float A1 = abs(((Triangle2[0] - Pos).cross(Triangle2[2]-Pos)).norm());
        float A2 = abs(((Triangle2[1] - Pos).cross(Triangle2[2]-Pos)).norm());
        float A3 = abs(((Triangle2[1] - Pos).cross(Triangle2[0]-Pos)).norm());
        return (A1*Normal[2]+A2*Normal[1]+A3*Normal[3])/A;
    }
}

void MyDisplay(){
    Vector3f Color = {0,0,0};
    for(int P=0;P<numOfMicoPolygons;P++){
        std::cout<<"Rendering: "<<(float)P/numOfMicoPolygons*100<<"%\n";
        for (float j =0 ; j<myBuffer.height ; j++) {
            for(float i = 0 ; i<myBuffer.width ; i++){
                //Vector3f screenPos = {0.2,0.2,0};
                Vector3f screenPos = {i/myBuffer.width*2-1,1-j/myBuffer.height*2,0};
                if(insideMicoPolygon(screenPos[0], screenPos[1], myMicoPolygon[P])
                   ){
                    Vector3f NormalIther = myMicoPolygon[P].getNormal(0,0);
                    Vector3f ColorIther = colorInterpolation(myMicoPolygon[P], screenPos);
                    Color = myLight.ambientLighting(ColorIther)+myLight.diffuseLighting(myMicoPolygon[P].Meshs[0].Points, NormalIther,ColorIther)+myLight.specularLighting(myMicoPolygon[P].Meshs[0].Points, NormalIther, ColorIther);
                    myBuffer.set_pixel(screenPos, Color,myMicoPolygon[P].Meshs[0].Points[2]);

                }
                //else
                //std::cout<<"Not Point\n";
            }
        }}

}


Vector3f downSample(int x, int y){
    Vector3f Color;
    Color += mySampleBuffer.get_pixel(x*2, y*2);
    Color += mySampleBuffer.get_pixel(x*2+1, y*2);
    Color += mySampleBuffer.get_pixel(x*2, y*2+1);
    Color += mySampleBuffer.get_pixel(x*2+1, y*2+1);
    return Color;
}

void jitterDisplay(){
    srand(time(0));
    Vector3f Color = {0.0,0.0,0.0};
    for(int P=0;P<numOfMicoPolygons;P++){
        std::cout<<"Rendering: "<<(float)P/numOfMicoPolygons*100<<"%\n";
        for (float j =0 ; j<mySampleBuffer.height ; j++) {
            for(float i = 0 ; i<mySampleBuffer.width ; i++){
                //Vector3f screenPos = {0.2,0.2,0};
                Vector3f screenPos = {i/myBuffer.width*2-1,1-j/myBuffer.height*2,0};
                Vector3f sample = {screenPos[0]-2/mySampleBuffer.width+rand()%11/10/myBuffer.width*2,
                    screenPos[1]-2/mySampleBuffer.height+rand()%11/10/mySampleBuffer.height*2,0};
                if(insideMicoPolygon(sample[0], sample[1], myMicoPolygon[P])
                   ){
                    Color = myLight.normalLighting(myMicoPolygon[P].Meshs[0].Points, myMicoPolygon[P].Meshs[1].Normal, myMicoPolygon[P].Meshs[0].Color);
                }
                mySampleBuffer.set_pixel(screenPos, Color,myMicoPolygon[P].Meshs[0].Points[2]);
            }
        }}
    for(int j =0 ; j<myBuffer.height;j++){
        for(int i =0 ; i<myBuffer.width;i++){
            myBuffer.set_pixel(i, j, downSample(i, j), 1);
        }
        
    }
    
}

#endif