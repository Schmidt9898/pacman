#ifndef VERTICES_H_INCLUDED
#define VERTICES_H_INCLUDED
#include <vector>

class VerticeObject
{

    std::vector<float> vertices;//pontok kordinátái
    std::vector<unsigned int> indices;//háromszögek indexei


public:
    VerticeObject() {};
    size_t getsize(){return vertices.size()*sizeof(float);}
    size_t getindsize(){return indices.size()*sizeof(float);}
    size_t getind_db(){return indices.size();}
    float* getvertices(){return vertices.data();}
    unsigned int* getindices(){return indices.data();}
    ~VerticeObject() {std::cout<<"VerticieObject delete.\n";}

    bool LoadVerticies(std::string filenamever, std::string filenameind)
    {
        try
        {
            std::ifstream f(filenamever);
            float temp;
            while(!f.eof())
            {
                f>>temp>>std::ws;
                vertices.push_back(temp);
            }
            f.close();
        }
        catch(std::ifstream::failure)
        {
            std::cerr << "Exception opening/reading/closing "<<filenamever<<"\n";
            return false;
        }
        if(filenameind != "none")///todo check
        {
            if(!LoadIndicies(filenameind))
            {
            //indices.resize(3);
            int def[] = {0,1,2};
            indices.insert(indices.begin(), def, def+3);
            return false;
            }
        return true;
        }
        else
        {
            //indices.resize(3);
            //std::cout<<indices.size();
            int def[] = {0,1,2};
            indices.insert(indices.begin(), def, def+3);}
            //std::cout<<indices.size();
            return true;
    }



    private:
    bool LoadIndicies(std::string filename)
    {
        try
        {
            std::ifstream f(filename);
            unsigned int temp;
            while(!f.eof())
            {
                f>>temp>>std::ws;
                indices.push_back(temp);
            }
            f.close();
            return true;
        }
        catch(std::ifstream::failure)
        {
            std::cerr << "Exception opening/reading/closing "<<filename<<"\n";
            return false;
        }
    }
    /*public:
    bool LoadVerInd(std::string filenamever,std::string filenameind)
    {
        return LoadVerticies(filenamever) && LoadIndicies(filenameind);

    }*/



};



#endif // VERTICES_H_INCLUDED
