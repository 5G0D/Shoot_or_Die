#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <iostream>;
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Box2D.h"
#include <vector>
#include <ctime>

using namespace sf;
using namespace std;

class Player;

struct Ammo
{
    int max;
    int current;
};

class Weapon
{
    private:

        Ammo ammo = {10,10};
        bool Reloading = false;
        Clock clock;
        double ReloadTime, time;;
        Sprite gun;

    protected:

        b2Vec2 gun_position = {40, 0}; //Позиция оружия относительно игрока
        int gunAngle = 0;
        int damage;

    public:

        void setGunSprite(Texture& texture);
        Sprite getGunSprite();
        void setGunSpriteRect(IntRect rectangle);
        void updateGunAngle (RenderWindow& window, vector<Player>& player);
        void setGunAngle (vector<Player>& player);
        Ammo getAmmo();
        b2Vec2 getPosition();
        bool getReloading();
        float getReloadTime();
        void setAmmo(bool current, int n);
        void updateReloading();
        void setPosition(float, float);
};

class Bullet
{
    private:

        b2Vec2 bullet_pos;
        double bullet_dir;
        Clock clock;
        float life_time = 0;
        bool life = true;
        b2Body* bulletHB;
        b2PolygonShape bullet_shape;
        b2BodyDef bdef;
        b2FixtureDef fdef;
        Sprite bullet_sprite;
        void Connect();
        void create(b2Vec2 pos, bool dir);

    public:

        Bullet(b2Vec2 pos = {0,0}, bool dir = false);
        void deleteHB();
        Sprite getSprite();
        void update();
        bool checkCollision(vector<Player>& player);
        b2Body* getHB();
        bool getLife();
};

class Player
{
    private:
        double timer, time, anim_time = 0;;
        Clock clock, clocks;
        Texture texture_skin;
        int currentSkin = 0;
        SoundBuffer glide_s, step_s, shot_s, reload_s, fall_s, hit_s;
        Sound hit, glide, reload, shot, step, fall;
        bool check_firstsit = true, onJump = false, WithGun = true, onGround = false, Sit = false, direction = false, control_type, Shooting = false;
        bool status = false; // Сидит или стоит игрок
        b2PolygonShape hero_head, hero_body;    //
        b2BodyDef bdef;                         //  Box2D
        b2FixtureDef fdef, fdef2;               //
        b2Body *heroHB; // Тело Box2D (Хитбокс)
        Sprite sprite;
        b2Vec2 speed, position;
        int hp = 100, oldHP = 100, Lifes = 3;
        double weapon_timer = 301;
        Weapon weapon;
        string Name = "Player";
        bool oldYPosition = false; //Предыдущая позиция игроке // falls - в воздухе // true - на земле

    public:

        Player();
        bool getShooting();
        void deleteHB();
        void HB_create ();
        void LoadSkin (bool dir);
        void setSpeed (float x, float y);
        void setAcceleration (float x, float y);
        void setControl (bool type);
        void changeHP(int HP);
        b2Vec2 getSpeed ();
        bool getOnGround ();
        bool getSit ();
        bool SecondJump = false;
        bool getDirection ();
        int getHP();
        int getLifes();
        int getCurrentSkin();
        b2Body* getHB();
        b2Vec2 getPosition ();
        Sprite getSprite();
        Weapon getWeapon();
        string getName();
        void setWeapon();
        void SitHB(double x = 0, double y = 0);
        void StandHB(double x = 0, double y = 0);

        void updateSound();
        void setSounds();
        void Control(vector<Bullet>& bullets);
        void Update ();
        void check_on ();  //Различные проверки
        void Connect (); // Соединение спрайтов с хитбоксами
};
#endif // PLAYER_H_INCLUDED
