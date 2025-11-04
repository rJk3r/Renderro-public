#ifndef Primes_h
#define Primes_h


RtVoid RenderroColor(RtColor color){
    current_Color[0] = color[2]*255;
    current_Color[1] = color[1]*255;
    current_Color[2] = color[0]*255;
}

void bumpy(newMesh *mesh, float x , float y, float z){
    Vector3f normal {x,y,z};
    float strenth = 0.1;
    normal.normalize();
    mesh->Points[0] += strenth * normal[0]*((float)(rand()%11)/10);
    mesh->Points[1] += strenth * normal[1]*((float)(rand()%11)/10);
    mesh->Points[2] += strenth * normal[2]*((float)(rand()%11)/10);
}

void bumpyNormal(newMesh *mesh, float x , float y, float z){
    Vector3f normal {x,y,z};
    float strenth = 0.1;
    normal.normalize();
    mesh->Points[0] += strenth * normal[0]*((float)(rand()%11)/10);
    mesh->Points[1] += strenth * normal[1]*((float)(rand()%11)/10);
    mesh->Points[2] += strenth * normal[2]*((float)(rand()%11)/10);
}


RtVoid RenderroSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax, ...)
{
    srand(time(0));
    tmax = tmax/180*Pi;
    RtFloat LamdaMin = ( zmin >= -radius ? asin(zmin/radius) : -Pi/2);
    RtFloat LamdaMax = ( zmax <= radius ? asin(zmax/radius) : Pi/2);
    
    
    int u=Mesh_Size,v=Mesh_Size;
    
    static struct newMesh my_mesh [(Mesh_Size+1)*(Mesh_Size+1)];
    
    RtFloat deLamda = (LamdaMax - LamdaMin)/ v;
    RtFloat deTheta = tmax / u;
    
    
    RtFloat x = 0.0;
    RtFloat y = 0.0;
    RtFloat z = 0.0;
    
    for( int i = 0 ; i < v+1; i++){
        for ( int j =0; j< u+1; j++){
            x = radius * cosf(j * deTheta) * cosf(LamdaMin + (i * deLamda));
            y = radius * sinf(j * deTheta) * cosf(LamdaMin + (i * deLamda));
            z = radius * sinf(LamdaMin + (i * deLamda));
            
            //printf("%f \n",cos(j * deTheta));
            
            my_mesh[(i*v + j)].Points[0] = (RtFloat)x ;
            my_mesh[(i*v + j)].Points[1] = (RtFloat)y ;
            my_mesh[(i*v + j)].Points[2] = (RtFloat)z ;
            if(Bumpy){
                bumpy(&my_mesh[(i*v + j)],x , y, z);
                my_mesh[(i*v + j)].Color = castToTexture(j, i, Mesh_Size+1, Mesh_Size+1);
            }
            else if(UsingTexture){
                my_mesh[(i*v + j)].Color = castToTexture(j, i, Mesh_Size+1, Mesh_Size+1);
            }
            else if(UsingCheckBoard)
            {
                int a = i/CHECK_SIZE_Y;
                int b = j/CHECK_SIZE_Y;
                if((a%2 == 0 && b%2 ==0)||(a%2 == 1 && b%2 ==1)){
                    my_mesh[(i*v + j)].Color[0] = 255;
                    my_mesh[(i*v + j)].Color[1] = 255;
                    my_mesh[(i*v + j)].Color[2] = 255;
                }
                else{
                    my_mesh[(i*v + j)].Color[0] = 0;
                    my_mesh[(i*v + j)].Color[1] = 0;
                    my_mesh[(i*v + j)].Color[2] = 0;
                }

            }
            else if(UsingOpicity){
                my_mesh[(i*v + j)].Color[0] = current_Color[0];
                my_mesh[(i*v + j)].Color[1] = current_Color[1];
                my_mesh[(i*v + j)].Color[2] = current_Color[2];
                my_mesh[(i*v + j)].Opcity[0] = current_Opacity[0];
                my_mesh[(i*v + j)].Opcity[1] = current_Opacity[1];
                my_mesh[(i*v + j)].Opcity[2] = current_Opacity[2];
            }
            else{
                my_mesh[(i*v + j)].Color[0] = current_Color[0];
                my_mesh[(i*v + j)].Color[1] = current_Color[1];
                my_mesh[(i*v + j)].Color[2] = current_Color[2];
            }
        }
    }
    castToProjection(my_mesh);
    formatMicopolygon(u, v,my_mesh);
}

RtVoid RenderroCone(RtFloat height, RtFloat radius, RtFloat tmax, ...){
    int u=Mesh_Size,v=Mesh_Size;
    static struct newMesh my_mesh [(Mesh_Size+1)*(Mesh_Size+1)];
    for (float i =0 ; i < u+1; i++) {
        for (float j = 0 ; j < v+1; j++) {
            float theta = tmax*Pi / 180 / u * j;
            float x = radius * (v - i)/v* cos(theta);
            float y = radius * (v - i)/v * sin(theta);
            float z =  height/v * i;
            
            my_mesh[(int)(i*v + j)].Points[0] = (RtFloat)x ;
            my_mesh[(int)(i*v + j)].Points[1] = (RtFloat)y ;
            my_mesh[(int)(i*v + j)].Points[2] = (RtFloat)z ;
            if(Bumpy){
                bumpy(&my_mesh[(int)(i*v + j)],x , y, z);
                Bumpy = false;
            }
            else if(UsingTexture){
                my_mesh[(int)(i*v + j)].Color = castToTexture(j, i, Mesh_Size+1, Mesh_Size+1);
            }
            else if(UsingCheckBoard)
            {
                int a = ((int)(i))/CHECK_SIZE_Y;
                int b = ((int)(j))/CHECK_SIZE_X;
                if((a%2 == 0 && b%2 ==0)||(a%2 == 1 && b%2 ==1)){
                    my_mesh[(int)(i*v + j)].Color[0] = 255;
                    my_mesh[(int)(i*v + j)].Color[1] = 255;
                    my_mesh[(int)(i*v + j)].Color[2] = 255;
                }
                else{
                    my_mesh[(int)(i*v + j)].Color[0] = 0;
                    my_mesh[(int)(i*v + j)].Color[1] = 0;
                    my_mesh[(int)(i*v + j)].Color[2] = 0;
                }
                
            }
            else{
                my_mesh[(int)(i*v + j)].Color[0] = current_Color[0];
                my_mesh[(int)(i*v + j)].Color[1] = current_Color[1];
                my_mesh[(int)(i*v + j)].Color[2] = current_Color[2];
            }
        }
    }
    castToProjection(my_mesh);
    formatMicopolygon(u, v,my_mesh);
}

RtVoid RenderroCylinder(RtFloat radius,RtFloat zmin,RtFloat zmax,RtFloat tmax, ...){
    int u=Mesh_Size,v=Mesh_Size;
    static struct newMesh my_mesh [(Mesh_Size+1)*(Mesh_Size+1)];
    for (float i =0 ; i < u+1; i++) {
        for (float j = 0 ; j < v+1; j++) {
            float theta = tmax*Pi / 180 / u * j;
            float x = radius * cosf(theta);
            float y = radius * sinf(theta);
            float z =  (zmax-zmin)/v*i;
            
            
            my_mesh[(int)(i*v + j)].Points[0] = (RtFloat)x ;
            my_mesh[(int)(i*v + j)].Points[1] = (RtFloat)y ;
            my_mesh[(int)(i*v + j)].Points[2] = (RtFloat)z ;
            
            std::cout<< x<<","<<y<<","<<z<<std::endl;
            if(Bumpy){
                bumpy(&my_mesh[(int)(i*v + j)],x , y, z);
                Bumpy = false;
            }
            else if(UsingTexture){
                my_mesh[(int)(i*v + j)].Color = castToTexture(j, i, Mesh_Size+1, Mesh_Size+1);
            }
            else if(UsingCheckBoard)
            {
                int a = i/CHECK_SIZE_Y;
                int b = j/CHECK_SIZE_X;
                if((a%2 == 0 && b%2 ==0)||(a%2 == 1 && b%2 ==1)){
                    my_mesh[(int)(i*v + j)].Color[0] = 255;
                    my_mesh[(int)(i*v + j)].Color[1] = 255;
                    my_mesh[(int)(i*v + j)].Color[2] = 255;
                }
                else{
                    my_mesh[(int)(i*v + j)].Color[0] = 0;
                    my_mesh[(int)(i*v + j)].Color[1] = 0;
                    my_mesh[(int)(i*v + j)].Color[2] = 0;
                }
                
            }
            else{
                my_mesh[(int)(i*v + j)].Color[0] = current_Color[0];
                my_mesh[(int)(i*v + j)].Color[1] = current_Color[1];
                my_mesh[(int)(i*v + j)].Color[2] = current_Color[2];
            }
        }
    }
    
    int i =9;
    std::cout<<"before pro"<< my_mesh[i].Points[0]<<","<<my_mesh[i].Points[1]<<","<<my_mesh[i].Points[0]<<std::endl;
    formatMicopolygon(u, v,my_mesh);
    castToProjection(my_mesh);
    std::cout<<"after pro"<< my_mesh[i].Points[0]<<","<<my_mesh[i].Points[1]<<","<<my_mesh[i].Points[0]<<std::endl;
    formatMicopolygon(u, v,my_mesh);
}


RtVoid RenderroTorus(RtFloat majrad, RtFloat minrad, RtFloat phimin, RtFloat phimax, RtFloat tmax, ...){
    int u=Mesh_Size,v=Mesh_Size;
    static struct newMesh my_mesh [(Mesh_Size+1)*(Mesh_Size+1)];
    for (int i =0 ; i < u+1; i++) {
        for (int j = 0 ; j < v+1; j++) {
            float theta = tmax*Pi / 180 / u * i;
            float phi = (phimin + (phimax - phimin)/v * j)*Pi / 180;
            float r = minrad*cos(phi);
            float z = minrad*sin(phi);
            float x = (majrad + r)* cos(theta);
            float y = (majrad + r)* sin(theta);
            
            my_mesh[(j*v + i)].Points[0] = (RtFloat)x ;
            my_mesh[(j*v + i)].Points[1] = (RtFloat)y ;
            my_mesh[(j*v + i)].Points[2] = (RtFloat)z ;
            if(Bumpy){
                bumpy(&my_mesh[(i*v + j)],x , y, z);
                Bumpy = false;
            }
            else if(UsingTexture){
                my_mesh[(i*v + j)].Color = castToTexture(j, i, Mesh_Size+1, Mesh_Size+1);
            }
            else if(UsingCheckBoard)
            {
                int a = i/CHECK_SIZE_Y;
                int b = j/CHECK_SIZE_Y;
                if((a%2 == 0 && b%2 ==0)||(a%2 == 1 && b%2 ==1)){
                    my_mesh[(i*v + j)].Color[0] = 255;
                    my_mesh[(i*v + j)].Color[1] = 255;
                    my_mesh[(i*v + j)].Color[2] = 255;
                }
                else{
                    my_mesh[(i*v + j)].Color[0] = 0;
                    my_mesh[(i*v + j)].Color[1] = 0;
                    my_mesh[(i*v + j)].Color[2] = 0;
                }
                
            }
            else{
                my_mesh[(i*v + j)].Color[0] = current_Color[0];
                my_mesh[(i*v + j)].Color[1] = current_Color[1];
                my_mesh[(i*v + j)].Color[2] = current_Color[2];
            }
        }
    }
    castToProjection(my_mesh);
    formatMicopolygon(u, v,my_mesh);

}

#endif