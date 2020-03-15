#include "game.h"
#include<sstream>
float random(float a,float b);//random gener�tor a-b k�z�tt 1000 felbont�ssal
glm::vec2 to_vector(unsigned int x_,unsigned int y_);//converter k�perny� -> j�t�kt�r
void Write_grid(string filename,vector<vector<bool>> &grid);//map ki�r� �s
void Read_grid(string filename,vector<vector<bool>> &grid);//map beolvas�
//globals
int sceen;//aktu�lis jelenet / menu, j�t�k, editor
double lastframe,currentframe,DELTA;//Delta sz�m�t�shoz
glm::vec2 player_direction;//merre megy a j�t�kos , gomblenyom�sb�l sz�m�tva

Game* mygame;// mutat� a j�t�kra static f�ggv�nyekhez

bool scriptstop=false;//aktu�lis script meg�ljon e //v�g�l nem ezzel lett megoldva de ne vegy�k ki
bool gameplay=true;//aktu�lis sceen eddig fut
bool iswin = false;//v�g�l sehol nem haszn�lt de ne vegy�k ki
bool show_exit = false;//v�g�l sehol nem haszn�lt de ne vegy�k ki

bool r_btn_down=false;
bool l_btn_down=false;






Game::Game()
{
    atmos.Load_sounds("Bubi_Sounds/sound_list.txt");
    atmos.Volume_atmos(20);
        atmos.Volume_bip(100);

// glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pacman2020", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    //load pens
    PEN_Init();
    //callbacks
    glfwSetKeyCallback(window, Game::key_callback);
    glfwSetMouseButtonCallback(window, Game::mouse_button_callback);

    srand (time(NULL));
    mygame=this;


}

void Game::loop()
{
    lastframe = glfwGetTime();
    sceen = 1;
    atmos.Stop();
    while(!glfwWindowShouldClose(window) && sceen!=-1)
    {
        monitor.lock();
    gameid++;
    monitor.unlock();
        switch(sceen)
        {
        case 1:
            sceen=menusceen();
            break;
        case 2:
            sceen=playsceen();
            break;
        case 3:
            sceen=editorsceen();
            break;
        default :

            break;
        }
    }
    glfwTerminate();
}


int Game::playsceen()
{
    std::cout<<"gamesceen\n";
    pont=0;
    if(player!=nullptr)
        delete player;
    if(exit!=nullptr)
        delete exit;

    player = new Rectangle(-0.5,0,0.1,0.1,playerpen);
    player->setColor(1,1,0.2);
    exit = new Rectangle(0,0,0.15,0.15,rect);
    exit->setColor(0.1,1,0.1);
    iswin=false;
    gameplay=true;

       //clear all
    for(i:Walls)
    {
        delete i;
    }
    Walls.clear();
    for(i:Enemies)
    {
        delete i;
    }
    Enemies.clear();
    for(i:Food)
    {
        delete i;
    }
    Food.clear();





vector<vector<bool>> grid;

switch(levelid)
{
case 1:
    Read_grid("map1.txt",grid);
    break;
case 2:
    Read_grid("map2.txt",grid);
    break;
case 3:
    Read_grid("map3.txt",grid);
    break;
    case 4:
    Read_grid("map4.txt",grid);
    break;
    case 5:
    Read_grid("map5.txt",grid);
    break;
    case 6:
    Read_grid("map6.txt",grid);
    break;
    case 7:
    Read_grid("map7.txt",grid);
    break;
default:
    //Read_grid("nogrid.txt",grid);
    break;
}
if(grid.size()!=29)
{
    levelid = 1;
    return 1;
}




for(i:grid)
{
    for(j:i)
    {
        std::cout<<j<<" ";
    }
    std::cout<<"\n";
}
for(int i=0; i<29; i++){
    for(int j=0; j<29; j++)
    {
        if(grid[j][i]){
        glm::vec2 position = to_vector(20+i*20,20+j*20);
        Rectangle* temp=new Rectangle(position.x,position.y,0.06,0.06,rect);
        temp->setColor(0.6,0.6,0.6);
         Walls.push_back(temp);
        }
    }

}


for(int i=0; i<29; i++){
    for(int j=0; j<29; j++)
    {
        if(!grid[j][i]){
        glm::vec2 position = to_vector(20+i*20,20+j*20);

        Rectangle* temp=new Rectangle(position.x,position.y,0.01,0.01,rect);
        temp->setColor(0.7,0.7,0.1);
         Food.push_back(temp);
        }
    }
}

    std::cout<<Food.size()<<"\n";
    scriptstop=false;
    script = new std::thread(scriptfv,this);

    int end_code=1;

    /// /////////////////////////////////////////////////////

atmos.Bubi_change_atmos("game");

    while (!glfwWindowShouldClose(window) && gameplay)//main
    {
        CalculateDelta();

        glClearColor(0.1, 0.1, 0.1, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        CalculatePlayerMove();

        monitor.lock();
        for(e:Enemies)
        {
            e->Go(DELTA,Walls,Enemies,player);
            if(player->is_colide_with(e))
            {
                gameplay=false;
                atmos.Bubibip("dead");
                end_code=1;
            }

            e->Draw();
        }
        monitor.unlock();

        for(f:Walls)
        {
            f->Draw();
            if(player->is_colide_with(f))
                player->box_colider_correction(f);
        }

        for(unsigned int i=0;i<Food.size();i++)
        {
            if(player->is_colide_with(Food[i]))
            {
                delete Food[i];
                Food[i]=Food[Food.size()-1];
                Food.pop_back();
                pont++;
                atmos.Bubibip("Pickup");
                cout<<pont<<"\n";
                i--;
            }else
            {
                Food[i]->Draw();
            }
        }
        for(f:Food)
        {
            f->Draw();
        }
        if(Food.size()<=0)
        {
            exit->Draw();
            if(exit->is_colide_with(player))
            {
                atmos.Bubibip("nyam");
                end_code=2;
                //iswin=true;
                gameplay=false;
                levelid++;
            }

        }
        //exit->Draw();
        player->Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
     std::cout<<"end_while_loop\n";
    monitor.lock();
    scriptstop=true;
    monitor.unlock();












    return end_code;
}

int Game::menusceen()
{
    std::cout<<"menusceen\n";
    atmos.Bubi_change_atmos("menu");
    int end_code=-1;
    gameplay=true;
    Rectangle* edit= new Rectangle(-0.25,0,0.25,0.5,rect);
    edit->setColor(0.5,0.5,1);
    Rectangle* play= new Rectangle(0.25,0,0.25,0.5,rect);
    play->setColor(0.5,1,0.5);


    while (!glfwWindowShouldClose(window) && gameplay)//main
    {
        CalculateDelta();
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

                if(edit->contain_point(to_vector(xpos,ypos))){
                        edit->setColor(0.1,0.1,1);
                        if(l_btn_down){
                            end_code=3;
                            gameplay=false;
                        }
                    }
                    else{
                        edit->setColor(0.5,0.5,1);
                    }
                    edit->Draw();

                    if(play->contain_point(to_vector(xpos,ypos))){
                        play->setColor(0.1,1,0.1);
                        if(l_btn_down){
                            end_code=2;
                            gameplay=false;
                        }
                    }
                    else{
                        play->setColor(0.5,1,0.5);
                    }
                    play->Draw();









        glfwSwapBuffers(window);
        glfwPollEvents();






        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
     std::cout<<"end_menu_loop\n";



    return end_code;
};

int Game::editorsceen()
{
    //clear all
    for(i:Walls)
    {
        delete i;
    }
    Walls.clear();
    for(i:Enemies)
    {
        delete i;
    }
    Enemies.clear();
    for(i:Food)
    {
        delete i;
    }
    Food.clear();


gameplay = true;


    std::cout<<"editorceen\n";
    player = new Rectangle(-0.5,0,0.1,0.1,playerpen);
    player->setColor(1,1,0.2);

    exit = new Rectangle(0,0,0.15,0.15,rect);
    exit->setColor(0.1,1,0.1);



///load new grid

vector<vector<bool>> grid;
std::cout<<"Enter the grid name\n";
                string filename;
                std::cin>>filename;
Read_grid(filename,grid);
if(grid.size()!=29){
        grid.clear();
Read_grid("grid.txt",grid);}
for(i:grid)
{
    for(j:i)
    {
        std::cout<<j<<" ";
    }
    std::cout<<"\n";
}
for(int i=0; i<29; i++){
    for(int j=0; j<29; j++)
    {
        glm::vec2 position = to_vector(20+i*20,20+j*20);
        Rectangle* temp=new Rectangle(position.x,position.y,0.07,0.07,rect);
        if(grid[j][i]){
        temp->setColor(0.8,0.8,0.8);
        }
        else
        {
            temp->setColor(0.8,0.8,0.8);
        }
        Walls.push_back(temp);
    }

}


    while (!glfwWindowShouldClose(window) && gameplay)//main
    {
        CalculateDelta();
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        CalculatePlayerMove();
         double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);


        for(int i=0; i<29; i++){
            for(int j=0; j<29; j++)
                {

                    Rectangle* f = Walls[i*29+j];
                    if(f->contain_point(to_vector(xpos,ypos))){
                        f->setColor(1,0,0);
                        if(l_btn_down)
                            grid[j][i]=false;
                        if(r_btn_down)
                            grid[j][i]=true;

                    }
                    else{
                        if(grid[j][i])
                            f->setColor(0.9,0.9,0.9);
                        else
                            f->setColor(0.2,0.2,0.2);
                    }


                    f->Draw();
                    if( grid[j][i] && player->is_colide_with(f))
                        player->box_colider_correction(f);

            }
            }




        exit->Draw();
        player->Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

        if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS )
            {
                string filename;
                std::cin>>filename;
                Write_grid(filename,grid);
                gameplay=false;
            }
        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS )
            {
                for(int i=0;i<29;i++){
                    for(int j=0;j<29;j++){
                        grid[i][j]=true;
                    }
                }

            }
        if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS )
            {
                for(int i=0;i<29;i++){
                    for(int j=0;j<29;j++){
                        grid[i][j]=false;
                    }
                }

            }


        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
     std::cout<<"end_edit_loop\n";

    return 1;
}





void Game::CalculateDelta()
{
    currentframe = glfwGetTime();
    DELTA = currentframe - lastframe;
    lastframe = currentframe;
    if(DELTA>0.1)
        DELTA=0;

}

void Game::CalculatePlayerMove()
{

    glm::vec2 norm = glm::normalize(player_direction);
    if(norm!=norm)
        norm=glm::vec2(0.0,0.0);
    player->laststep=player->pos;
    if(DELTA<0.1)
        player->pos+= glm::vec2(norm.x*DELTA,norm.y*DELTA);

     if(player->pos.x-player->scale.x/2<=-1)
        {
            player->pos.x=-1+player->scale.x/2+0.000001;

        }
        if(player->pos.y-player->scale.x/2<=-1)
        {
            player->pos.y=-1+player->scale.x/2+0.000001;

        }

        if(player->pos.x+player->scale.x/2>=1)
        {
            player->pos.x=1-player->scale.x/2-0.000001;

        }
        if(player->pos.y+player->scale.y/2>=1)
        {
            player->pos.y=1-player->scale.y/2-0.000001;

        }


}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0,width, height);
}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        gameplay=false;
        //glfwSetWindowShouldClose(window, true);
    //zeroing direction
    player_direction = glm::vec2(0.0,0.0);
    //geting direction
    if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS )
    {
        mygame->levelid++;
    }
if(mygame->player!=nullptr){

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS )
    {
        player_direction = player_direction+glm::vec2(0,1);
        mygame->player->rotation=3.1415/2;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        player_direction = player_direction+glm::vec2(-1,0);
        mygame->player->rotation=3.1415;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        player_direction = player_direction+glm::vec2(0,-1);
        mygame->player->rotation=-3.1415/2;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        player_direction = player_direction+glm::vec2(1,0);
        mygame->player->rotation=0;
    }

}






}

void Game::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{


    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        l_btn_down=true;
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);
        float x= 2.0/mygame->SCR_WIDTH*xpos-1.0;
        float y= 2.0/mygame->SCR_HEIGHT*ypos-1.0;
        y*=-1;
        std::cout << "Cursor Position at (" << x << " : " << y << "\n";

    }else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        l_btn_down=false;
        std::cout<<"asdasd\n";
        }
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        r_btn_down=true;
    }else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
        r_btn_down=false;

        }


}
float random(float a,float b)
{
    return (float)((b-a)/1000)*(rand()%1000)+a;
}
glm::vec2 to_vector(unsigned int x_,unsigned int y_)
{
    float x= 2.0/mygame->SCR_WIDTH*x_-1.0;
        float y= 2.0/mygame->SCR_HEIGHT*y_-1.0;
        y*=-1;
        return glm::vec2(x,y);
}


void Game::scriptfv()
{
int scriptid=gameid;
    std::this_thread::sleep_for(chrono::milliseconds(9000));

    if(scriptstop || scriptid!=gameid)
        return;
    monitor.lock();
    Enemies.push_back(new Ghost(-1,1,0.1,0.1,playerpen));
    Enemies[Enemies.size()-1]->setColor(1,0.2,0.1);
    monitor.unlock();
    std::this_thread::sleep_for(chrono::milliseconds(7000));
if(scriptstop || scriptid!=gameid)
        return;


    std::this_thread::sleep_for(chrono::milliseconds(500));
    if(scriptstop || scriptid!=gameid)
        return;
    monitor.lock();
    Enemies.push_back(new Ghost(-1,1,0.1,0.1,playerpen));
    Enemies[Enemies.size()-1]->setColor(1,0,1);
    monitor.unlock();

      std::this_thread::sleep_for(chrono::milliseconds(500));
      if(scriptstop || scriptid!=gameid)
        return;
    monitor.lock();
    Enemies.push_back(new Ghost(1,-1,0.1,0.1,playerpen));
    Enemies[Enemies.size()-1]->setColor(0,1,1);
    monitor.unlock();
/*
      std::this_thread::sleep_for(chrono::milliseconds(500));
      if(scriptstop || scriptid!=gameid)
        return;
    monitor.lock();
    Enemies.push_back(new Ghost(1,-1,0.1,0.1,rect));
    Enemies[Enemies.size()-1]->setColor(1,0.2,0.1);
    monitor.unlock();

      std::this_thread::sleep_for(chrono::milliseconds(500));
      if(scriptstop || scriptid!=gameid)
        return;
    monitor.lock();
    Enemies.push_back(new Ghost(1,1,0.1,0.1,rect));
    Enemies[Enemies.size()-1]->setColor(1,0.2,0.1);
    monitor.unlock();*/




}


void Read_grid(string filename,vector<vector<bool>> &grid)
{
    grid.clear();
    vector<bool> row;
    try
        {
            int i=0;
            std::ifstream f(filename);
            bool temp;
            while(!f.eof())
            {
                f>>temp>>std::ws;
                i++;
                row.push_back(temp);
                if(i>=29)
                {
                     grid.push_back(row);
                     i=0;row.clear();
                }
            }
            f.close();
        }
        catch(std::ifstream::failure)
        {
            std::cerr << "Exception opening/reading/closing "<<filename<<"\n";
            return;
        }
}
void Write_grid(string filename,vector<vector<bool>> &grid)
{
    //grid.clear();
    //vector<bool> row;
    try
        {
            //int i=0;
            std::ofstream f(filename);

            for(i:grid)
                {
                    for(j:i)
                    {
                        f<<j<<" ";
                    }
                    f<<"\n";
                }

            f.close();
        }
        catch(std::ifstream::failure)
        {
            std::cerr << "Exception opening/reading/closing "<<filename<<"\n";
            return;
        }
}


