#ifndef DRAWABLE_H_INCLUDED
#define DRAWABLE_H_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Pen.h"
#include <time.h>

using namespace std;
//PenFactory _penfactory;



//struct p16fgu {p16fgu(){PEN_Init();};};p16fgu _p16fgu;

class Drawable
{
    //protected:

public:
    Pen *mypen=nullptr;
    glm::vec2 laststep = glm::vec2(0.0,0.0);
    glm::vec2 pos = glm::vec2(0.0f,0.0f);
    glm::vec2 scale = glm::vec2(0.0,0.0);

    float rotation=0;



      static float random(float a,float b){
    return (float)((b-a)/1000)*(rand()%1000)+a;
}

public:
    glm::vec3 color=glm::vec3(1,1,1);
    glm::vec2 offset = glm::vec2(0.5,0.5);

    virtual ~Drawable() {};
    Drawable(Pen * p):mypen(p) {};
    virtual void Draw()
    {
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans,glm::vec3(pos,0.0));
        trans = glm::rotate(trans,rotation,glm::vec3(0,0,1));
        trans = glm::scale(trans,glm::vec3(scale,0.0));
        mypen->Draw(trans,color);
    }

    virtual bool contain_point(glm::vec2 dot)
    {

        bool colide_x = pos.x + scale.x/2 >= dot.x &&
                        dot.x >= pos.x-scale.x/2;


        bool colide_y = pos.y + scale.y/2 >=  dot.y &&
                        dot.y >= pos.y-scale.y/2;

        return colide_x && colide_y;
    }


    virtual bool is_colide_with(Drawable *obj)
    {

        bool colide_x = pos.x + scale.x/2 >= obj->pos.x-obj->scale.x/2 &&
                        obj->pos.x + obj->scale.x/2 >= pos.x-scale.x/2;


        bool colide_y = pos.y + scale.y/2 >= obj->pos.y-obj->scale.y/2 &&
                        obj->pos.y + obj->scale.y/2 >= pos.y-scale.y/2;

        return colide_x && colide_y;
    }
    virtual glm::vec2 box_colider_correction(Drawable *obj) //only if coliding, only if it's a box
    {

        glm::vec2 corr_vector=pos;
        if((laststep.x+scale.x/2) <= obj->pos.x-obj->scale.x/2)
            corr_vector.x= obj->pos.x-obj->scale.x/2-scale.x/2-0.000001;
        if(laststep.x-scale.x/2>=(obj->pos.x+obj->scale.x/2))
            corr_vector.x=obj->pos.x+obj->scale.x/2+scale.x/2+0.000001;

        if((laststep.y+scale.y/2) <= obj->pos.y-obj->scale.y/2)
            corr_vector.y= obj->pos.y-obj->scale.y/2-scale.y/2-0.000001;
        if(laststep.y - scale.y/2 >= (obj->pos.y+obj->scale.y/2))
            corr_vector.y=obj->pos.y+obj->scale.y/2+scale.y/2+0.000001;


       // std::cout<<corr_vector.x<<" x:y "<<corr_vector.y<<"\n";
        pos=corr_vector;
        return corr_vector;
    }
    /*
     virtual glm::vec2 box_colider(Drawable *obj) //only if coliding, only if it's a box
    {
        glm::vec2 corr_vector=pos;
        if((laststep.x+scale.x) <= obj->pos.x)
        corr_vector.x= obj->pos.x-scale.x-0.000001;
        if(laststep.x>=(obj->pos.x+obj->scale.x))
        corr_vector.x=obj->pos.x+obj->scale.x+0.000001;

        if((laststep.y+scale.y) <= obj->pos.y)
        corr_vector.y= obj->pos.y-scale.y-0.000001;
        if(laststep.y>=(obj->pos.y+obj->scale.y))
        corr_vector.y=obj->pos.y+obj->scale.y+0.000001;


        std::cout<<corr_vector.x<<" x:y "<<corr_vector.y<<"\n";

    return corr_vector;
    }*/
    void setColor(float r,float g, float b)
    {
        color= glm::vec3(r,g,b);
    }

};

class Rectangle : public Drawable
{
public:
    Rectangle(float x,float y,float w,float h,Pen* p):Drawable(p)
    {
        pos = glm::vec2(x,y);
        scale = glm::vec2(w,h);
    };
};

class Ghost : public Drawable
{
    // <- 0 fel 1 -> 2 le 3
    int direction = 0;
bool cansee = true;
public:
    Ghost(float x,float y,float w,float h,Pen* p):Drawable(p)
    {
        pos = glm::vec2(x,y);
        scale = glm::vec2(w,h);
        direction = rand()%4;

    };
    void Go(float delta,vector<Rectangle*> &walls,vector<Ghost*> &others,Drawable* player)
    {
        ///TODO "AI"
        //direction = direction%4;
        if(cansee)
        {
          if( abs(pos.x-player->pos.x) < scale.y/2)
          {
              direction = (pos.y<player->pos.y)? 1:3;
              cansee=false;
          }
          if( abs(pos.y-player->pos.y) < scale.x/2)
          {
              direction = (pos.x<player->pos.x)? 2:0;
              cansee=false;
          }


        }




        laststep=pos;
        switch (direction)
        {
        case 0 ://balra
            pos.x-=0.5*delta;
            rotation = 3.1415;
            break;
        case 1 ://fel
            pos.y+=0.5*delta;
            rotation = 3.1415/2;
            break;
        case 2 ://jobra
            pos.x+=0.5*delta;
            rotation = 0;
            break;
        case 3 ://le
            pos.y-=0.5*delta;
            rotation = -3.1415/2;
            break;
        }
        for(f:walls)
        {
            if(is_colide_with(f))
            {
                box_colider_correction(f);
                direction=rand()%4;
                cansee=true;
            }
        }
        for(f:others)
        {
            if(this!=f && is_colide_with(f))
            {
                box_colider_correction(f);
                direction=rand()%4;
                cansee=true;
            }
        }

        //detect screen edge
        if(pos.x-scale.x/2<=-1)
        {
            pos.x=-1+scale.x/2+0.000001;
            direction=rand()%4;
            cansee=true;
        }
        if(pos.y-scale.x/2<=-1)
        {
            pos.y=-1+scale.x/2+0.000001;
            direction=rand()%4;
            cansee=true;
        }

        if(pos.x+scale.x/2>=1)
        {
            pos.x=1-scale.x/2-0.000001;
            direction=rand()%4;
            cansee=true;
        }
        if(pos.y+scale.y/2>=1)
        {
            pos.y=1-scale.y/2-0.000001;
            direction=rand()%4;
            cansee=true;
        }

    }

} ;

class Boom : public Drawable{

glm::vec2 particles[50];
float r,f;//R=sugár F= erõ ala sebesség

public:
    Boom(float x,float y,float r_,float f_,Pen* p):Drawable(p)
    {
        pos=glm::vec3(x,y,0);
        f=f_;
        r=r_;
        for(int i=0;i<50;i++)
        {
            particles[i]=glm::vec2(random(-1/r,1/r),random(-1/r,1/r));
        }
    }

    void Draw(){
        for(int i=0;i<50;i++)
        {
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans,glm::vec3(pos.x+particles[i].x,pos.y+particles[i].y,0.0));
        //trans = glm::rotate(trans,rotation,glm::vec3(0,0,1));
        trans = glm::scale(trans,glm::vec3(0.01,0.01,0.0));
        mypen->Draw(trans,color);
        }

    }

};




#endif // DRAWABLE_H_INCLUDED
