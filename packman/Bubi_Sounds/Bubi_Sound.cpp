#include "Bubi_Sound.h"



Bubi_Sound::Bubi_Sound(){

        if(SDL_Init(SDL_INIT_AUDIO)==-1)
        {
            printf("SDL_Init: %s\n", SDL_GetError());
        }
        if(Mix_Init(MIX_INIT_MP3)==-1)
        {
            printf("mix_Init: %s\n", SDL_GetError());
        }
        if((Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,2048)) <0)
        {
            printf("mix_openaudio: %s\n", SDL_GetError());
        }
       // Volume(120);

    }


Bubi_Sound::~Bubi_Sound()
    {

        ///for_each

        for (std::map<std::string,Mix_Music*>::iterator it = musics.begin(); it != musics.end(); ++it)
        {

            Mix_FreeMusic(it->second);
            // cout<<it->first<<" free"<<endl;
        }
         for (std::map<std::string,Mix_Chunk*>::iterator it = sounds.begin(); it != sounds.end(); ++it)
        {
            Mix_FreeChunk(it->second);
             //cout<<it->first<<" free"<<endl;
        }


        Mix_Quit();
        SDL_Quit();
    }



  bool Bubi_Sound::Load_sounds(std::string filename)
    {

        /// sounds feltöltése
        std::string path_head="Bubi_Sounds/";
        std::ifstream file;
        file.open(filename);
        if(!file.good())
        {
            std::cout<<"File can't be openned.:"<<filename<<std::endl;
            return false;
        }
        std::string line;

        while(getline(file, line))
        {
            std::cout<<line;

            if(line.find(".mp3")<=line.length())
            {
                std::cout<<" mp3 ";///betölt
                Mix_Music* temp=Mix_LoadMUS((path_head+line).c_str());
                if(!temp)
                {///Hiba
                    printf("Mix_LoadMUS(%s): %s\n  ",line.c_str(),Mix_GetError());
                    continue;
                }
                ///".mp3" törlés
                line.erase(line.end()-4,line.end());
                ///return value
                 std::pair<std::map<std::string,Mix_Music*>::iterator,bool> ret;



                ///insert
                ret = musics.insert(std::pair<std::string,Mix_Music*>(line,temp));
                if (ret.second==false) {///már benne volt
                    Mix_FreeMusic(temp);///felszabadít
                     std::cout<<"already added."<<std::endl;
                    continue;
                }

                ///minden szupi :)
                std::cout<<line<<" added to musics tree"<<std::endl;
            }
            else if(line.find(".wav")<=line.length())
            {
                std::cout<<" wav ";///betölt
                Mix_Chunk * temp=Mix_LoadWAV((path_head+line).c_str());
                if(!temp)
                {///Hiba
                    printf("Mix_LoadWAV(%s): %s\n  ",line.c_str(),Mix_GetError());
                    continue;
                }
                ///".wav" törlés
                line.erase(line.end()-4,line.end());
                ///return value
                std::pair<std::map<std::string,Mix_Chunk*>::iterator,bool> ret;



                ret=sounds.insert(std::pair<std::string,Mix_Chunk *>(line,temp));
                  if (ret.second==false) {///már benne volt
                    Mix_FreeChunk(temp);///felszabadít
                     std::cout<<"already added."<<std::endl;
                    continue;
                }

                ///minden szupi :)

                std::cout<<line<<" added to sounds tree"<<std::endl;
            }
            else
            {
                std::cout<<" is not recognized format"<<std::endl;


            }
        }

return true;
    }


void Bubi_Sound::Volume(int vol)
{
    vol_master= (vol>max_vol) ? max_vol : ((vol<0)? 0:vol);

    /// new vol=vol*master/max_master

    Mix_Volume(-1, vol_bip*vol_master/max_vol);
    Mix_VolumeMusic(vol_atmos*vol_master/max_vol);


}


void Bubi_Sound::Volume_atmos(int vol)
{
    vol_atmos = (vol>max_vol) ? max_vol : ((vol<0)? 0:vol);

    /// new vol=vol*master/max_master

   // Mix_Volume(-1, vol_bip*vol_master/max_vol);
    Mix_VolumeMusic(vol_atmos*vol_master/max_vol);


}


void Bubi_Sound::Volume_bip(int vol)
{
    vol_bip= (vol>max_vol) ? max_vol : ((vol<0)? 0:vol);

    /// new vol=vol*master/max_master

    Mix_Volume(-1, vol_bip*vol_master/max_vol);
    //Mix_VolumeMusic(vol_atmos*vol_master/max_vol);


}


void Bubi_Sound::Bubibip(std::string bip)
{
     std::map<std::string,Mix_Chunk*>::iterator it;
      it = sounds.find(bip);
  if (it == sounds.end())
    return;


    for(int chanel=0;chanel<10;chanel++)
    {
        if(Mix_Playing(chanel)==0)
        {
            Mix_PlayChannel(chanel,sounds[bip],0);
           // std::cout<<chanel<<std::endl;
            break;
        }
    }

}



 void  Bubi_Sound::Bubi_change_atmos(std::string music)
 {
    Mix_Paused(-1);
    Mix_FadeOutMusic(1000);

    std::map<std::string,Mix_Music*>::iterator it;
    it = musics.find(music);
    if (it == musics.end())
        return;
    Mix_FadeInMusic(it->second,-1,200);
    //Mix_PlayMusic(it->second,-1);

/*
    switch(atmos)
    {
    case menu:
    {
        std::map<std::string,Mix_Music *>::iterator it;
        it = musics.find("menu");
        if (it == musics.end())
        {
            break;
        }

        Mix_PlayMusic(it->second,-1);
    }
    break;
    case loading:
    {
        std::map<std::string,Mix_Music *>::iterator it;
        it = musics.find("loading");
        if (it == musics.end())
        {
            break;
        }

        Mix_PlayMusic(it->second,-1);
    }
    break;
    case game:
    {
        std::map<std::string,Mix_Music *>::iterator it;
        it = musics.find("game");
        if (it == musics.end())
        {
            break;
        }

        Mix_PlayMusic(it->second,-1);
    }
    break;
    default:
        break;
    }
    */

}



void Bubi_Sound::Unload()
{

            for (std::map<std::string,Mix_Music*>::iterator it = musics.begin(); it != musics.end(); ++it)
        {

            Mix_FreeMusic(it->second);
            // cout<<it->first<<" free"<<endl;
        }
         for (std::map<std::string,Mix_Chunk*>::iterator it = sounds.begin(); it != sounds.end(); ++it)
        {
            Mix_FreeChunk(it->second);
             //cout<<it->first<<" free"<<endl;
        }

    musics.clear();
    sounds.clear();

}

void Bubi_Sound::Stop()
{
    Mix_FadeOutChannel(-1,500);
    Mix_FadeOutMusic(500);
}





