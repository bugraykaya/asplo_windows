
#ifndef ASPLOSHOOTER_H
#define ASPLOSHOOTER_H

#include <vector>


#define PI 3.14159265
#define ASPLO_WIDTH 0.090f
#define ASPLO_HEIGHT 0.030f
#define POKI_WIDTH 0.15f
#define POKI_HEIGHT 0.15f
#define BULLET_WIDTH 0.04f
#define BULLET_HEIGHT 0.04f
#define HP_BAR_HEIGHT 0.02f
#define HP_BAR_WIDTH 0.04f

class MainWindow;
class Poki;
class Asplosions;
class Actor;
class Bullet;

class Scene
{
    
    
    public:
    Scene();
    ~Scene(){}
    
    void drawAsplo(float x, float y);
    void drawPoki(float x, float y);
    void drawBullet(float x, float y, float deg);
    void drawHpBar();
    void update();
    void draw();
    
    MainWindow *parent;
    
    int frameCounter=0;
    
    
    int pokiOffsetInBytes;
    int asploOffsetInBytes;
    int texCOffsetInBytes;
    int bulletOffsetInBytes;
    int hpBarOffsetInBytes;
    
    unsigned int dataVBO;
    unsigned int asploTexture;
    unsigned int pokiTexture;
    unsigned int bulletTexture;
    unsigned int hpBarTexture;
    unsigned int menuTexture;
    
    std::vector<Asplosions*> enemies;
    std::vector<Bullet*> bullets;
    
    Actor *poki;
    
    float xmouse, ymouse, realmousex, realmousey;
    
    bool running = true;
    bool enterDown = false;
};



struct Actor
{
    public:
    Actor( float _x=0, float _y=0): x(_x),y(_y) 
    {
        hp = 100;
    }
    
    
    ~Actor(){}
    float x, y;
    int hp;
};

struct Asplosions: public Actor
{
    public: 
    Asplosions(float _x=0 , float _y= 0) : Actor(_x , _y)
    {
        hp=60;
    }
    ~Asplosions(){}
    
    size_t lastAttackFrame=0;
    
};



struct Bullet: public Actor
{
    public: 
    Bullet(float _x=0 , float _y= 0) : Actor(_x , _y)
    {
        
    }
    ~Bullet(){}
    
    float deg, xSpeed, ySpeed;
};


#endif // MAINWINDOW_H

