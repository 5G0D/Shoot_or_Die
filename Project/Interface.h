#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <SFML\Graphics.hpp>
#include "Box2D.h"
#include "Player.h"
#include <vector>

using namespace std;
using namespace sf;



class Bar
{
    private:

        Texture emptyBarT, LifesT, HPBarT, AmmoBarT;;

    protected:

        Sprite emptyBar, HPBar, AmmoBar ,Lifes;

    public:

        void setBarTextures();

};

class PlayerStats : virtual public Bar
{
    private:

        Texture PlayerStatsT;
        Sprite PlayerStatsS;
        Font font;
        Text Name, Reloading;
        bool Reload;
        int lastSkin = 0;

    public:

        PlayerStats();

        void setTexture();
        void setText();
        void draw(RenderWindow& window);
        void setPosition(b2Vec2 pos);
        void update(float HP, int i, Ammo ammo, String name, bool reload, float ReloadTime, int lifes, int CurrentSkin);

};

class Interface
{
    private:

        PlayerStats PlayerStats[4];
        Font font;
        int Emount;

    public:
        void Update(vector<Player>& player);
        void Draw(RenderWindow& window);
        void setTextures();
};

#endif // INTERFACE_H_INCLUDED
