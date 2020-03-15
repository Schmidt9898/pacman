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

Bubi_Sound atmos; // Saját hang könyvtár haladó c++ projektbõl
//Már nem emlékszem hogy mûködik.
//float screenscale=1;

GLFWwindow* window; // az ablak

//Pens // toll gyûjtemény
Pen* tri;
Pen* rect;
Pen* playerpen;

//float r = 0, g = 0, b = 0;

//float trans_x = 0, trans_y=0;
//float x=0,y=0;

//entities
//több helyen is kell ezért nem lehet localis
Rectangle* player=nullptr;
Rectangle* exit=nullptr;
int pont=0;

vector<Ghost*> Enemies;//szellemek
vector<Rectangle*> Walls;//falak
vector<Rectangle*> Food;//felvehetõ pontok
//vector<Drawable*>
///scripting
/*
egyszerû scriptelés külön szálon, szálbiztos
*/
std::thread *script=nullptr;
std::mutex monitor;


void PEN_Init()//tollak betöltése a memóriába és a videókártyára
{
tri = new Pen("triangle","files/triangle.vs","files/triangle.fs","files/triangle.txt","none");
rect = new Pen("rectangle","files/rectangle.vs","files/rectangle.fs","files/rectangle.txt","files/rectangle.ind");
playerpen = new Pen("player","files/player.vs","files/player.fs","files/player.txt","files/player.ind");
}


//callbacks az órai
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
///TODO resize callback
//sceens

int menusceen();//1
int playsceen();//2
int editorsceen();//3
//int load1();//nem használtam

//delta
void CalculateDelta(); // legutóbbi képkocka után eltelt idõ
//player movement
void CalculatePlayerMove(); // player mozgás számítása lenyomott billentyûkbõl
//




void scriptfv();//script függvény

public:
Game();//init
void loop();//loop
int gameid=0;//ahoz hogy a scripp ne befolyásolja az új játékot ha meghalunk és még a script élne
int levelid=1;// szintlépéshez 7 szint van a játékban


const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;




};


#endif // GAME_H_INCLUDED
