#ifndef ENVIRONMENT_H_INCLUDED
#define ENVIRONMENT_H_INCLUDED

#include "Player.h"

class Environment
{

    private:

        b2PolygonShape shape; //
        b2BodyDef bdef;    //  Ёллементы Box2D
        b2FixtureDef fdef;  //
        b2Body  *platformHB; // ’итбоксы платформ
        Texture texture;
        Sprite sprite;
        b2Vec2 pos;

    public:

        void create(); // —оздание хитбоксов платформ
        void Connect (); // —оедин€ет хитбоксы со спрайтами
        void Create(int texture_type, int x = 0, int y = 0, int kx = 1, int ky = 1);
        void deleteHB();
        void setTexture(int texture_type, int x, int y, int kx, int ky);
        Sprite getSprite();
};

class Background
{
    private:

        vector<Texture> bg_texture;
        vector<Sprite> bg_sprite;

    public:

        ~Background();
        int getSize();
        Sprite getSprite(int i);
        void setBackground(int type);
        void move(float speed);


};
#endif // ENVIRONMENT_H_INCLUDED
