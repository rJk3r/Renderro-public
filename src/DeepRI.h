#ifndef DeepRI_h
#define DeepRI_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#define Mesh_Size 10
#define numOfx  400
#define numOfy  300


#define numOfM 2
#define numOfN 2

#define Pi 3.1415926

extern RtFloat aspectRatio;


extern glm::mat4 projectionMatrix;
extern glm::mat4 current_transMatrix;
extern glm::vec3 current_Color;
extern glm::vec3 current_Opacity;
extern glm::mat4 worldMatrix;

extern RtFloat yon;
extern RtFloat hither;
extern RtInt w;
extern RtInt h;

extern int numOfMicoPolygons;

extern std::vector<glm::vec3> myTexture;

struct OpcityLink{
    int ind;
    glm::vec3 Color;
    glm::vec3 Opcity;
    float depth;
    OpcityLink *next;
};


extern class newFrameBuffer{
    
public:
    int width,height;
    std::vector<glm::vec3> frame_buf;
    std::vector<float> depth_buf;
    std::vector<glm::vec3> opicity_buf;
    OpcityLink opcLink[400*300];
    
    
    newFrameBuffer(int w, int h):width(w), height(h)
    {
        frame_buf.resize(w * h);
        depth_buf.resize(w * h);
        opicity_buf.resize(w*h);
        //opicity_buf2.resize(w*h);
        for (int i =0; i<w*h; i++) {
            this->depth_buf[i]=100;
            this->opicity_buf[i][0] =1;
            this->opicity_buf[i][1] =1;
            this->opicity_buf[i][2] =1;
            
            opcLink[i].Opcity[0] = 1;
            opcLink[i].Opcity[1] = 1;
            opcLink[i].Opcity[2] = 1;
            
            opcLink[i].ind = 0;
            
            opcLink[i].depth = 0;
            opcLink[i].next = NULL;
        }
    }
    
    void set_pixel(const glm::vec3& point, const glm::vec3& color, float depth)
    {
        //old index: auto ind = point.y + point.x * width;
        //auto ind = (height-point.y-1)*width + point.x;
        auto ind = (height*(1-point.y))/2*width + (point.x+1)/2*width;
        if(ind >= width*height || ind<0){
            std::cout<<"Set Pixel out of range: "<< point.x<<", "<<point.y<<"\n";
        }
        else if(depth_buf[ind] >depth+0.001)
        {
            frame_buf[ind] = color;
            depth_buf[ind] = depth;
           
        }
    }
    
    
    void set_pixel(int x ,int y, const glm::vec3& color, float depth)
    {
        //old index: auto ind = point.y() + point.x() * width;
        //auto ind = (height-point.y()-1)*width + point.x();
        
        int ind = x + y*this->width;
        if(ind >= width*height || ind<0){
            std::cout<<"Set Pixel out of range: "<< x <<", "<<y<<"\n";
        }
        else if(depth_buf[ind] >depth+0.001){
            frame_buf[ind] = color;
            depth_buf[ind] = depth;
            
        }
        
        
    }
    
    void set_opicity(const glm::vec3& point, const glm::vec3& opicity, glm::vec3& Color, float depth){
        int ind = (height*(1-point.y))/2*width + (point.x+1)/2*width;
        if(ind >= width*height || ind<0){
            std::cout<<"Set Opicity out of range: ";
        }
        else {
            int ind =0;
            OpcityLink temp = opcLink[ind];
            static OpcityLink res;
            res.Opcity[0] = opicity[0];
            res.Opcity[1] = opicity[1];
            res.Opcity[2] = opicity[2];
            res.Color[0] = Color[0];
            res.Color[1] = Color[1];
            res.Color[2] = Color[2];
            res.depth = depth;
            res.next = NULL;
            while(temp.next != NULL){
                temp = *temp.next;
                ind++;
            }
            res.ind = ind;
            temp.next = &res;
        }
    }
    
    OpcityLink *Insert(OpcityLink *head,const glm::vec3& point, const glm::vec3& opicity, glm::vec3& Color, float depth){
        int ind = (height*(1-point.y))/2*width + (point.x+1)/2*width;
        OpcityLink* temp;
        temp = &this->opcLink[ind];
        OpcityLink *res;
        res = new OpcityLink;
        res->Opcity[0] = opicity[0];
        res->Opcity[1] = opicity[1];
        res->Opcity[2] = opicity[2];
        res->Color[0] = Color[0];
        res->Color[1] = Color[1];
        res->Color[2] = Color[2];
        res->depth = depth;
        res->ind = 1;
        res->next = nullptr;
        while(temp->next != nullptr){
            res->ind++;
            temp = temp->next;
        }
        temp->next = res;
        return head;
    }
    
    
    void set_opicity2(const glm::vec3& point, const glm::vec3& opicity, glm::vec3& Color, float depth){
        int ind = (height*(1-point.y))/2*width + (point.x+1)/2*width;
        OpcityLink* reserve;
        reserve = &this->opcLink[ind];
        if(ind >= width*height || ind<0){
            std::cout<<"Set Opicity out of range: ";
        }
        else {
            OpcityLink* temp;
            temp = &this->opcLink[ind];
            OpcityLink *res;
            res = new OpcityLink;
            res->Opcity[0] = opicity[0];
            res->Opcity[1] = opicity[1];
            res->Opcity[2] = opicity[2];
            res->Color[0] = Color[0];
            res->Color[1] = Color[1];
            res->Color[2] = Color[2];
            res->depth = depth;
            res->ind = 1;
            res->next = nullptr;
            while(temp->next != nullptr){
                res->ind++;
                temp = temp->next;
            }
            temp->next = res;
            this->opcLink[ind] = *reserve;
        }
        
    }
    
    glm::vec3 get_pixel(const glm::vec3& point)
    {
        int ind = (height-point.y)*width + point.x;
        if(ind >= width*height || ind<0){
            std::cout<<"Get Pixel out of range\n";
            return this->frame_buf[0];
        }
        else
        {
            if(opicity_buf[ind] == glm::vec3(0.0f))
                return this->frame_buf[ind];
            else
            {
                glm::vec3 temp;
                temp[0] = frame_buf[ind][0]*opicity_buf[ind][0];
                temp[1] = frame_buf[ind][1]*opicity_buf[ind][1];
                temp[2] = frame_buf[ind][2]*opicity_buf[ind][2];
                return temp;
            }
        }
    }
    
    void get_pixel2(const glm::vec3& point)
    {
        int ind = (height*(1-point.y))/2*width + (point.x+1)/2*width;
        
        
        glm::vec3 res ;
        float tempDepth=100;
        if(ind >= width*height || ind<0){
            std::cout<<"Get Pixel out of range\n";
        }
        else{
            OpcityLink *temp;
            temp = &this->opcLink[ind];
            OpcityLink *reserve;
            reserve = &this->opcLink[ind];
            temp = &this->opcLink[ind];
            glm::vec3 resColor;
            glm::vec3 tempOpc;
            glm::vec3 tempColor;
            int leastInd = 0;
            float leastDepth = 0;
            for(int i =0 ; i<6; i++){
                temp = reserve;
                do{
                    float test =depth_buf[ind];
                    if(temp->depth > leastDepth && temp->depth< this->depth_buf[ind]-0.001){
                        leastInd = temp->ind;
                        leastDepth = temp->depth;
                        tempOpc = temp->Opcity;
                        tempColor = temp->Color;
            
                    }
                    temp = temp->next;
                }while(temp != nullptr);
                //float A = this->frame_buf[ind][0];
                //float B = tempOpc[0];
                //float C = tempColor[0];
                float r = this->frame_buf[ind][0]*tempOpc[0] + tempColor[0]*(1-tempOpc[0]);
                float g = this->frame_buf[ind][1]*tempOpc[1] + tempColor[1]*(1-tempOpc[1]);
                float b = this->frame_buf[ind][2]*tempOpc[2] + tempColor[2]*(1-tempOpc[2]);
                this->frame_buf[ind][0] = r;
                this->frame_buf[ind][1] = g;
                this->frame_buf[ind][2] = b;
                this->depth_buf[ind] = leastDepth;
                //tempDepth = 0;
                
                //std::cout<<r<<std::endl;
                leastDepth = 0;
                tempOpc[0] = 1;
                tempOpc[1] = 1;
                tempOpc[2] = 1;
                tempColor[0] = 0;
                tempColor[1] = 0;
                tempColor[2] = 0;
                
                //opcLink[ind] = *reserve;
            }
        }
    }
    
    
    glm::vec3 get_pixel(int x, int y)
    {
        int ind = y*width + x;
        if(ind >= width*height || ind<0){
            std::cout<<"Get Pixel out of range\n";
            return this->frame_buf[0];
        }
        else
        {
            glm::vec3 temp;
            temp[0] = frame_buf[ind][0]*opicity_buf[ind][0];
            temp[1] = frame_buf[ind][1]*opicity_buf[ind][1];
            temp[2] = frame_buf[ind][2]*opicity_buf[ind][2];
            return temp;
        }
    }
    
    std::vector<glm::vec3>& frame_buffer() { return frame_buf; }
    
    
};

extern newFrameBuffer myBuffer;
extern newFrameBuffer mySampleBuffer;

class newMesh{
public:
    glm::vec3 Points;
    glm::vec3 Color;
    glm::vec3 Normal;
    glm::vec3 Opcity;
    
    newMesh(){}
    newMesh(glm::vec3 Pos, glm::vec3 Col, glm::vec3 Nor){
        Points = Pos;
        Color = Col;
        Normal = Nor;
        //Opcity = {,0,0};
    }
    
    
};

class newMicoPolygon{
    
public:
    newMesh Meshs[4];
    newMicoPolygon(){}
    newMicoPolygon(newMesh* a,newMesh* b, newMesh* c,newMesh* d){
        this->Meshs[0] = *a;
        this->Meshs[1] = *b;
        this->Meshs[2] = *c;
        this->Meshs[3] = *d;
    }
    
    void castMicoPolygon(newMesh* a,newMesh* b, newMesh* c,newMesh* d){
        this->Meshs[0] = *a;
        this->Meshs[1] = *b;
        this->Meshs[2] = *c;
        this->Meshs[3] = *d;
    }
    
    glm::vec3 getNormal(float a, float b){
        glm::vec3 Positions[4];
        Positions[0] = glm::vec3(this->Meshs[0].Points[0],this->Meshs[0].Points[1],this->Meshs[0].Points[2]);
        Positions[1] = glm::vec3(this->Meshs[1].Points[0],this->Meshs[1].Points[1],this->Meshs[1].Points[2]);
        Positions[2] = glm::vec3(this->Meshs[2].Points[0],this->Meshs[2].Points[1],this->Meshs[2].Points[2]);
        Positions[3] = glm::vec3(this->Meshs[3].Points[0],this->Meshs[3].Points[1],this->Meshs[3].Points[2]);
        
        glm::vec3 Lines[4];
        Lines[0] = glm::cross(Positions[1]-Positions[0], Positions[2]-Positions[0]);
        Lines[1] = glm::cross(Positions[3]-Positions[1], Positions[0]-Positions[1]);
        Lines[2] = glm::cross(Positions[0]-Positions[2], Positions[3]-Positions[2]);
        Lines[3] = glm::cross(Positions[2]-Positions[3], Positions[1]-Positions[3]);
        Lines[0] = glm::normalize(Lines[0]);
        
        return Lines[0];
    }
    
    glm::vec3 getBumNormal(float a, float b)
    {
        srand(time(0));
        float strengh = 0.01;
        
        glm::vec3 Positions[4];
        Positions[0] = glm::vec3(this->Meshs[0].Points[0],this->Meshs[0].Points[1],this->Meshs[0].Points[2]);
        Positions[1] = glm::vec3(this->Meshs[1].Points[0],this->Meshs[1].Points[1],this->Meshs[1].Points[2]);
        Positions[2] = glm::vec3(this->Meshs[2].Points[0],this->Meshs[2].Points[1],this->Meshs[2].Points[2]);
        Positions[3] = glm::vec3(this->Meshs[3].Points[0],this->Meshs[3].Points[1],this->Meshs[3].Points[2]);
        
        glm::vec3 Lines[4];
        Lines[0] = glm::cross(Positions[1]-Positions[0], Positions[2]-Positions[0]);
        Lines[1] = glm::cross(Positions[3]-Positions[1], Positions[0]-Positions[1]);
        Lines[2] = glm::cross(Positions[0]-Positions[2], Positions[3]-Positions[2]);
        Lines[3] = glm::cross(Positions[2]-Positions[3], Positions[1]-Positions[3]);
        
        Lines[0].x += strengh * (float)(rand()%11)/10;
        Lines[0].y += strengh * (float)(rand()%11)/10;
        Lines[0].z += strengh * (float)(rand()%11)/10;
        Lines[0] = glm::normalize(Lines[0]);
        
        return Lines[0];
    }
    
    std::vector<glm::vec3> frame_buf;
    
};

extern newMesh myMesh[Mesh_Size*Mesh_Size];
extern newMicoPolygon myMicoPolygon[100000];

class Light{
public:
    glm::vec3 Position;
    glm::vec3 Color;
    float lightStrength;
    float ambientStrength;
    float specularStrength;
    
    Light(){
        Position = glm::vec3(0,0,0);
        Color = glm::vec3(0,0,0);
        lightStrength = 0;
        ambientStrength= 0.0;
        specularStrength =0;
    }
    
    void setLight( glm::vec3 Pos, glm::vec3 Col,float lightStg, float ambi, float specularStg){
        this->Position = Pos;
        this->Color = Col;
        this->lightStrength = lightStg;
        this->ambientStrength= ambi;
        this->specularStrength = specularStg;
    }
    
    glm::vec3 ambientLighting(glm::vec3 objectCol){
        return objectCol * this->ambientStrength;
    }
    
    glm::vec3 diffuseLighting(glm::vec3 objectPos, glm::vec3 objectNor, glm::vec3 objectCol){
        glm::vec3 lightDir = objectPos - this->Position;
        lightDir = glm::normalize(lightDir);
        float diffuse = glm::dot(objectNor, -lightDir);
        diffuse = diffuse > 0? diffuse : 0;
        
        return diffuse*objectCol*lightStrength;
    }
    
    glm::vec3 specularLighting(glm::vec3 objectPos, glm::vec3 objectNor, glm::vec3 objectCol){
        glm::vec3 lightDir = objectPos - this->Position;
        lightDir = glm::normalize(lightDir);
        glm::vec3 viewDir = glm::normalize(-objectPos);
        glm::vec3 reflectDir = lightDir - 2.0f*objectNor*glm::dot(objectNor, lightDir);
        //glm::vec3 reflectDir = lightDir-2*objectNor*glm::dot(objectNor, objectNor);
        float spec = pow(fmax(glm::dot(viewDir, -reflectDir), 0.0f), 32);
        glm::vec3 specular = specularStrength * spec * Color;
        return specular;
    }
    
    glm::vec3 normalLighting(glm::vec3 objectPos, glm::vec3 objectNor, glm::vec3 objectCol){
        glm::vec3 lightDir = glm::normalize(objectPos - this->Position);
        glm::vec3 viewDir = glm::normalize(-objectPos);
        glm::vec3 reflectDir = glm::normalize(lightDir - 2.0f*objectNor*glm::dot(objectNor, lightDir));
        
        float diffuse = glm::dot(objectNor, lightDir);
        diffuse = diffuse > 0? diffuse : 0;
        
        float spec = pow(fmax(glm::dot(viewDir, reflectDir), 0.0f), 32);
        
        return objectCol*this->ambientStrength + diffuse*lightStrength*objectCol + specularStrength * spec * Color;
    }
};

extern Light myLight;


extern void formatMicopolygon(int u, int v, newMesh *m);
//extern void castToProjection(newMesh *myMesh);

extern int micoPolygonContainPoint(struct microPolygon M, RtPoint P);

extern void printMico(int i);
extern void testMatrix(RtMatrix a);
extern void printMesh(struct Mesh mesh);
extern void MyDisplay();
extern bool insideTriangle(float x, float y, const glm::vec3* _v);
extern bool insideMicoPolygon(float x, float y, newMicoPolygon MP);
extern glm::vec3 castToTexture(int u, int v, int numofX, int numofY);
extern void castToProjection(newMesh *M);

extern float BUMP_AMPLITUDE;
extern float BUMP_MIN_FREQ_EXP;
extern float BUMP_MAX_FREQ_EXP;

extern RtToken BUMPY;
extern RtToken RENDERRO_PERSPECTIVE;

extern bool Bumpy ;
extern bool UsingTexture ;
extern bool UsingCheckBoard;
extern bool UsingOpicity;

extern void texture(int a);

extern int CHECK_SIZE_X ;
extern int CHECK_SIZE_Y ;
extern void (*CHECKERBOARD)(void);
#endif /* My_Ri_h */