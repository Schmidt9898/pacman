#ifndef BUBI_SOUND_H_INCLUDED
#define BUBI_SOUND_H_INCLUDED

#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>



class Bubi_Sound
{
    std::map<std::string,Mix_Chunk*> sounds;
    std::map<std::string,Mix_Music*> musics;

    ///Mix_Music music;

    bool bmaster=true;
    const int max_vol=128;
    int vol_master=128;
    int vol_atmos=128;
    int vol_bip=128;



public:
    ///init
    Bubi_Sound();
    ///clean up
    ~Bubi_Sound();
    ///betölti a fájlból
    bool Load_sounds(std::string filename);
    ///hangot kér , nem szinkronizált egyből játszik
    void Bubibip(std::string bip);
    /// háttér zenéje és lejátszása függ a atmoszférátol és a jelenetől
    ///lehetséges jelenetekért lásd az enumokat lejjebb
    void Bubi_change_atmos(std::string atmos);
    ///128 a max
    void Volume(int vol);
    void Volume_atmos(int vol);
    void Volume_bip(int vol);

    void Unload();
    void Stop();




};



#endif // BUBI_SOUND_H_INCLUDED
