#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "Environment.h"
#include <string.h>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Interface.h"

using namespace sf;
using namespace std;

class Engine
{
    private:

        SoundBuffer GameOverS;
        Sound GameOver;
        Clock clock;
        View view;
        b2Vec2 view_center, min_pos, max_pos;;
        Vector2u size;
        Vector2f view_size;
        Music music;
        RenderWindow* window;
        vector<Environment> env;
        vector<Player> player;
        vector<Bullet> bullets;
        Interface interface;
        Background bg;

        Texture LoadingScreenT, LoadingBarT, LoadingStatusT;
        Sprite LoadingScreen, LoadingBar, LoadingStatus;

    public:

        ~Engine();
        Engine(RenderWindow*);
        void LoadingDisplay();
        void setSounds();
        void Play();
        void Render (vector<Player>& player, vector<Bullet>& bullets, vector<Environment>& env, Background& bg,  Interface interface);

};


#endif // ENGINE_H_INCLUDED
