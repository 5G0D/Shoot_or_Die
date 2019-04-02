
#include "Environment.h"

Sprite Environment::getSprite()
{
    return sprite;
}

void Environment::Connect ()
{
    pos = platformHB->GetPosition();
    sprite.setPosition (pos.x * SCALE, pos.y * SCALE);
}

void Environment::setTexture (int texture_type, int x, int y, int kx, int ky)
{
    switch (texture_type)
    {
        case 0:

            break;
        case 1:
            texture.loadFromFile("./Resources/Textures/Platforms/ground_stone.png");
            sprite.setTextureRect(IntRect(0, 0, 800, 60));
            sprite.setOrigin(400 , 30);
            shape.SetAsBox(400/SCALE, 30/SCALE);
            break;
        case 2:
            texture.loadFromFile("./Resources/Textures/Platforms/ground_grass.png");
            sprite.setTextureRect(IntRect(0, 0, 150, 37));
            sprite.setOrigin(75 , 18.5);
            shape.SetAsBox(75/SCALE, 18.5/SCALE);
            break;
        case 3:

            break;
        case 4:

            break;
    }
    sprite.setTexture(texture);
    bdef.position.Set (x/SCALE, y/SCALE);
    platformHB = World.CreateBody(&bdef);
    fdef.friction = 0.4;
    fdef.shape = &shape;
    platformHB->CreateFixture (&fdef);
}

void Environment::Create (int texture_type, int x, int y, int kx, int ky)
{
    setTexture(texture_type, x, y, kx, ky);
}

void Environment::deleteHB()
{
    World.DestroyBody(platformHB);
}

void Background::setBackground(int type)
{
    switch (type)
    {
        case 0:
            bg_texture.resize(11);
            bg_sprite.resize(11);
            for (int i = 1; i<12; i++)
            {
                string s = "./Resources/Textures/Backgrounds/";
                s += to_string(i);
                s += ".png";
                cout<<s<<endl;;
                bg_texture[i-1].loadFromFile(s);
                bg_texture[i-1].setRepeated(true);
                bg_texture[i-1].setSmooth(true);
                bg_sprite[i-1].setTexture(bg_texture[i-1]);
                bg_sprite[i-1].setScale(1.2,1.2);
                bg_sprite[i-1].setTextureRect(IntRect(0, 0, 4915, 1855));
                bg_sprite[i-1].setPosition(-3000, -800);
            }
            break;
    }
}

void Background::move(float speed)
{
    for (int i = 0; i < bg_sprite.size(); i++)
    {
        bg_sprite[i].move(-speed/((i+1)*10), 0);
    }
}

int Background::getSize()
{
    return bg_sprite.size();
}

Sprite Background::getSprite(int i)
{
    return bg_sprite[i];
}

Background::~Background()
{
    bg_sprite.clear();
    bg_texture.clear();
}
