#ifndef STARTSCREEN_H_INCLUDED
#define STARTSCREEN_H_INCLUDED

#include <string.h>
#include <iostream>
#include <cmath>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

class StartScreen
{
    private:

        View view;
        Texture bgT;
        Sprite bg;
        FloatRect visibleArea;
        RenderWindow* window;
        Font font;
        Text text;
        Clock clock;
        double timer = 0;
        bool textDir = true;

    public:

        StartScreen(RenderWindow*);
        void Play();
};

#endif // STARTSCREEN_H_INCLUDED
