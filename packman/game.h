#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <thread>
#include  <mutex>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Drawable.h"
#include <time.h>
#include "Bubi_Sound.h"



class Game{

Bubi_Sound atmos;
//float screenscale=1;

GLFWwindow* window;

//Pens
Pen* tri;
Pen* rect;
Pen* playerpen;

//float r = 0, g = 0, b = 0;

//float trans_x = 0, trans_y=0;
//float x=0,y=0;

//entities
Rectangle* player=nullptr;
Rectangle* exit=nullptr;
int pont=0;
vector<Ghost*> Enemies;
vector<Rectangle*> Walls;
vector<Rectangle*> Food;
//vector<Drawable*>
///scripting
std::thread *script=nullptr;
std::mutex monitor;


void PEN_Init()
{
tri = new Pen("triangle","files/triangle.vs","files/triangle.fs","files/triangle.txt","none");
rect = new Pen("rectangle","files/rectangle.vs","files/rectangle.fs","files/rectangle.txt","files/rectangle.ind");
playerpen = new Pen("player","files/player.vs","files/player.fs","files/player.txt","files/player.ind");
}


//callbacks
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
///TODO resize callback
//sceens

int menusceen();//1
int playsceen();//2
int editorsceen();//3
int load1();

//delta
void CalculateDelta();
//player movement
void CalculatePlayerMove();
//




void scriptfv();

public:
Game();//init
void loop();//loop
int gameid=0;
int levelid=1;


const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;




};


#endif // GAME_H_INCLUDED
