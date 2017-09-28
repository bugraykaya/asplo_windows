#include "asploshooter.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

#include <windows.h>
#include <wingdi.h>
#include <SOIL/SOIL.h>
#include "glee.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"SOIL.lib")
#pragma comment(lib, "msvcrt.lib")


void initGL(Scene &scene, HWND hwnd, HDC dc);

bool appRunning = true;
int viewportW;
int viewportH;
HWND g_hwnd;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch(Message) {
        
        case WM_DESTROY: {
            appRunning = false;
            break;
        }
        case WM_SIZE:
        {
            viewportW = 0x0000ffff & lParam;
            viewportH = (0xffff0000 & lParam)>>16;
            glViewport(0,0, viewportW , viewportH);
            
            break;
        }
        
        default:
        return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;
    
    memset(&wc,0,sizeof(wc));
    wc.cbSize		 = sizeof(WNDCLASSEX);
    wc.lpfnWndProc	 = WndProc;
    wc.hInstance	 = hInstance;
    wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
    
    wc.style = CS_OWNDC;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = "WindowClass";
    wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);
    
    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }
    
    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","lol :p",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1000,
                          900,
                          NULL,NULL,hInstance,NULL);
    
    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }
    g_hwnd = hwnd;
    HDC dc = GetDC(hwnd);
    
    Scene scene;
    initGL(scene, hwnd , dc);
    
    
    
    while(appRunning )
    {
        while( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        glClear(GL_COLOR_BUFFER_BIT);
        
        scene.update();
        scene.draw();
        SwapBuffers(dc);
    }
    return msg.wParam;
}


void initGL(Scene& scene, HWND hwnd , HDC dc)
{
    
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
        PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
        32,                        //Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,0,0,0,0, 0, 0, 0,
        24,                        //Number of bits for the depthbuffer
        8,                        //Number of bits for the stencilbuffer
        0,                        //Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    
    
    int iPixFormat = ChoosePixelFormat(dc , &pfd);
    SetPixelFormat(dc, iPixFormat, &pfd );
    
    HGLRC glContext = wglCreateContext(dc);
    
    wglMakeCurrent(dc , glContext );
    
    
    
    glClearColor(1,0,1,0);
    
    const GLubyte* renderer = glGetString (GL_RENDERER);
    const GLubyte* version = glGetString (GL_VERSION);
    std::cout << "Renderer: "<<renderer<< "\n";
    std::cout << "OpenGL version supported: "<<version<< "\n";
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    glEnable(GL_TEXTURE_2D);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    unsigned char* image;
    int imageWidth, imageHeight, channels;
    
    image =  SOIL_load_image("../images/asplosions.png", &imageWidth, &imageHeight, &channels, SOIL_LOAD_RGBA);
    glGenTextures(1, &scene.asploTexture);
    glBindTexture(GL_TEXTURE_2D, scene.asploTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    delete[] image;
    
    image =  SOIL_load_image("../images/poki.png", &imageWidth, &imageHeight, &channels, SOIL_LOAD_RGBA);
    glGenTextures(1, &scene.pokiTexture);
    glBindTexture(GL_TEXTURE_2D, scene.pokiTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    delete[] image;
    
    image =  SOIL_load_image("../images/bullet.png", &imageWidth, &imageHeight, &channels, SOIL_LOAD_RGBA);
    glGenTextures(1, &scene.bulletTexture);
    glBindTexture(GL_TEXTURE_2D, scene.bulletTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    delete[] image;
    
    image =  SOIL_load_image("../images/hp.png", &imageWidth, &imageHeight, &channels, SOIL_LOAD_RGBA);
    glGenTextures(1, &scene.hpBarTexture);
    glBindTexture(GL_TEXTURE_2D, scene.hpBarTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    delete[] image;
    
    image =  SOIL_load_image("../images/menu.png", &imageWidth, &imageHeight, &channels, SOIL_LOAD_RGBA);
    glGenTextures(1, &scene.menuTexture);
    glBindTexture(GL_TEXTURE_2D, scene.menuTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    delete[] image;
    
    
    glGenBuffers(1, &scene.dataVBO);
    
    
    float data[] = {
        -POKI_WIDTH, POKI_HEIGHT, -4.0f,
        -POKI_WIDTH, -POKI_HEIGHT,-4.0f,
        POKI_WIDTH ,-POKI_HEIGHT,-4.0f,
        POKI_WIDTH, POKI_HEIGHT,-4.0f,
        POKI_WIDTH, -POKI_HEIGHT,-4.0f,
        -POKI_WIDTH, POKI_HEIGHT,-4.0f,
        
        -ASPLO_WIDTH, ASPLO_HEIGHT, -4.0f, //asplo vertices
        -ASPLO_WIDTH, -ASPLO_HEIGHT,-4.0f,
        ASPLO_WIDTH, -ASPLO_HEIGHT,-4.0f ,
        ASPLO_WIDTH, ASPLO_HEIGHT,-4.0f ,
        ASPLO_WIDTH, -ASPLO_HEIGHT,-4.0f ,
        -ASPLO_WIDTH, ASPLO_HEIGHT, -4.0f,
        
        -BULLET_WIDTH, BULLET_HEIGHT, -4.0f , //bullet vertices
        -BULLET_WIDTH, -BULLET_HEIGHT,-4.0f,
        BULLET_WIDTH, -BULLET_HEIGHT,-4.0f ,
        BULLET_WIDTH, BULLET_HEIGHT,-4.0f ,
        BULLET_WIDTH, -BULLET_HEIGHT,-4.0f ,
        -BULLET_WIDTH, BULLET_HEIGHT, -4.0f,
        
        -HP_BAR_WIDTH, HP_BAR_HEIGHT, -4.0f , //HP bar vertices
        -HP_BAR_WIDTH, -HP_BAR_HEIGHT,-4.0f,
        HP_BAR_WIDTH, -HP_BAR_HEIGHT,-4.0f ,
        HP_BAR_WIDTH, HP_BAR_HEIGHT,-4.0f ,
        HP_BAR_WIDTH, -HP_BAR_HEIGHT,-4.0f ,
        -HP_BAR_WIDTH, HP_BAR_HEIGHT, -4.0f,
        
        0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,1.0f,1.0f,0.0f,0.0f//Texture coords
    };
    
    scene.pokiOffsetInBytes = 0;
    scene.asploOffsetInBytes = 18*sizeof(float);
    scene.bulletOffsetInBytes = 36*sizeof(float);
    scene.hpBarOffsetInBytes = 54*sizeof(float);
    scene.texCOffsetInBytes = 72*sizeof(float);
    
    glBindBuffer(GL_ARRAY_BUFFER, scene.dataVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,
            1,
            -1,
            1,
            1,
            100000);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



Scene::Scene()
{
    poki = new Actor();
}


void Scene::drawAsplo(float x, float y)
{
    
    float rad = atan2(poki->x -x , poki->y -y );
    float deg = (rad/PI)*180;
    
    
    glLoadIdentity();
    glTranslatef(x,y,0);
    glRotatef(deg,0,0,-1);
    
    glBindTexture(GL_TEXTURE_2D, asploTexture);
    glBindBuffer(GL_ARRAY_BUFFER, dataVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)asploOffsetInBytes);
    glTexCoordPointer(2, GL_FLOAT, 0, (void*)texCOffsetInBytes);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
}

void Scene::drawHpBar()
{
    glLoadIdentity();
    glTranslatef(-0.98+HP_BAR_WIDTH, 0.98-HP_BAR_HEIGHT,0);
    
    glBindTexture(GL_TEXTURE_2D, hpBarTexture);
    glVertexPointer(3, GL_FLOAT, 0, (void*)hpBarOffsetInBytes);
    glTexCoordPointer(2, GL_FLOAT, 0, (void*)texCOffsetInBytes);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    for(int i=10; i<poki->hp; i+=10)
    {
        glTranslatef(HP_BAR_WIDTH+0.04,0,0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}


void Scene::drawBullet(float x, float y, float deg)
{
    glLoadIdentity();
    glTranslatef(x,y,0);
    glRotatef(deg,0,0,-1);
    
    glBindTexture(GL_TEXTURE_2D, bulletTexture);
    glBindBuffer(GL_ARRAY_BUFFER, dataVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)bulletOffsetInBytes);
    glTexCoordPointer(2, GL_FLOAT, 0, (void*)texCOffsetInBytes);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
}


void Scene::drawPoki(float x, float y)
{
    
    float rad = atan2(realmousey -y ,realmousex -x);
    float deg = (rad/PI)*180;
    
    glLoadIdentity();
    
    glTranslatef(x,y,0);
    glRotatef(deg,0,0,1);
    
    glBindTexture(GL_TEXTURE_2D, pokiTexture);
    glBindBuffer(GL_ARRAY_BUFFER, dataVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)pokiOffsetInBytes);
    glTexCoordPointer(2, GL_FLOAT, 0, (void*)texCOffsetInBytes);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    
}

void Scene::update()
{
    
    frameCounter++;
    
    if(running)
    {
        POINT mousePoint;
        if(GetCursorPos(&mousePoint))
        {
            ScreenToClient( g_hwnd , &mousePoint);
            xmouse= mousePoint.x;
            ymouse = viewportH - mousePoint.y;
            realmousex = xmouse / (viewportW / 2) -1 ;
            realmousey = ymouse / (viewportH / 2) -1 ;
            
        }
        
        
        
        int size= enemies.size();
        
        for(int i=0; i< size; i++)
        {
            float distToPoki = sqrt((poki->x - enemies[i]->x) *(poki->x - enemies[i]->x) + (poki->y - enemies[i]->y)*(poki->y - enemies[i]->y));
            
            if(distToPoki <= 0.15 )
            {
                if(frameCounter - enemies[i]->lastAttackFrame >= 60)
                {
                    enemies[i]->lastAttackFrame = frameCounter;
                    poki->hp -= 10;
                    
                    if(poki->hp== 0)
                    {
                        for(int k=0 ; k<enemies.size(); k++ )
                        {
                            delete enemies[k];
                        }
                        enemies.clear();
                        
                        for(int k=0 ; k<bullets.size(); k++ )
                        {
                            delete bullets[k];
                        }
                        bullets.clear();
                        
                        running = false;
                        
                        return;
                    }
                    
                }
            }
            else
            {
                enemies[i]->x += (poki->x - enemies[i]->x)/(distToPoki*500);
                enemies[i]->y += (poki->y - enemies[i]->y)/(distToPoki*500);
            }
            
        }
        
        
        for(int i=0; i<bullets.size(); i++)
        {
            
            
            bullets[i]->x += bullets[i]->xSpeed;
            bullets[i]->y += bullets[i]->ySpeed;
            
            if(bullets[i]->x > 1.5 ||bullets[i]->x < -1.5 ||bullets[i]->y > 1.5 ||bullets[i]->y < -1.5    )
            {
                delete bullets[i];
                bullets.erase(bullets.begin()+i);
                i--;
            }
            
            else
            {
                for(int j=0 ; j< enemies.size(); j++)
                {
                    float distToEnemy = sqrt(pow (bullets[i]->x - enemies[j]->x ,2 ) +pow (bullets[i]->y - enemies[j]->y ,2 ));
                    
                    if(distToEnemy < 0.1)
                    {
                        
                        enemies[j]->hp -= 20;
                        if(enemies[j]->hp <= 0)
                        {
                            delete enemies[j];
                            enemies.erase(enemies.begin()+j);
                        }
                        
                        delete bullets[i];
                        bullets.erase(bullets.begin()+i);
                        i--;
                        break;
                        
                    }
                }
            }
        }
        
        
        if(GetKeyState('W') & 0xf0)
        {
            poki->y+=0.01;
        }
        if(GetKeyState('A') & 0xf0)
        {
            poki->x-=0.01;
        }
        if(GetKeyState('S') & 0xf0)
        {
            poki->y-=0.01;
        }
        if(GetKeyState('D') & 0xf0)
        {
            poki->x+=0.01;
        }
        
        
        if(frameCounter %13 == 0 && GetKeyState(VK_LBUTTON) & 0xf0 )
        {
            float rad = atan2(realmousex- poki->x  , realmousey -poki->y );
            float deg = (rad/PI)*180;
            
            float distToMouse = sqrt(pow (realmousex- poki->x ,2 ) +pow (realmousey- poki->y ,2 ));
            
            
            Bullet *bullet = new Bullet();
            
            bullet->xSpeed =( realmousex- poki->x)/(distToMouse*50);
            bullet->ySpeed =( realmousey- poki->y)/(distToMouse*50);
            bullet->x = poki->x;
            bullet->y = poki->y;
            bullet->deg = deg;
            bullets.push_back(bullet);
        }
        
        if(frameCounter % 60==0 )
        {
            Asplosions *newAsplo;
            int edge = rand()%4;
            switch(edge)
            {
                case 0://top
                newAsplo = new Asplosions( ((float)(rand()%2000))/1000 -1 , 1.5 );
                break;
                case 1://bottom
                newAsplo = new Asplosions( ((float)(rand()%2000))/1000 -1 ,-1.5 );
                break;
                case 2://left
                newAsplo = new Asplosions( -1.5 ,((float)(rand()%2000))/1000 -1 );
                break;
                case 3://right
                newAsplo = new Asplosions(  1.5 ,((float)(rand()%2000))/1000 -1 );
                break;
                
                
                
            }
            enemies.push_back(newAsplo);
            
        }
    }
    else 
    {
        if(GetKeyState( VK_RETURN ) & 0xf0)
        {
            enterDown = true;
            
        }
        else if(enterDown)
        {
            enterDown = false;
            running = true;
            poki->hp = 100;
        }
    }
    
}

void Scene::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    if(running){
        
        drawPoki(poki->x , poki->y);
        
        
        int size= enemies.size();
        for(int i=0 ; i< size; i++)
        {
            drawAsplo(enemies[i]->x,enemies[i]->y);
        }
        
        size = bullets.size();
        for(int i=0 ; i< size; i++)
        {
            drawBullet(bullets[i]->x,bullets[i]->y, bullets[i]->deg );
        }
        drawHpBar();
    }
    else {
        
        glLoadIdentity();
        glScalef(1/POKI_WIDTH,1/POKI_HEIGHT,1  );
        
        glBindTexture(GL_TEXTURE_2D, menuTexture);
        glVertexPointer(3, GL_FLOAT, 0, (void*)pokiOffsetInBytes);
        glTexCoordPointer(2, GL_FLOAT, 0, (void*)texCOffsetInBytes);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
    }
}
