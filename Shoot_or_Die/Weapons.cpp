
#include "Player.h"

void Bullet::create(b2Vec2 pos, bool dir)
{

    static Texture bullet_texture;        //“екстура пули
    static bool flag = true;
    if (flag)
    {
        flag = false;
        bullet_texture.loadFromFile("./Resources/Textures/bullet.png");
        bullet_texture.setSmooth(true);
    }
    bullet_sprite.setTexture(bullet_texture);
    bullet_sprite.setOrigin(50,5);
    bullet_sprite.setScale(0.3f, 0.3f);
    bullet_sprite.setTextureRect(IntRect(0, 0, 100 , 10));

    bullet_shape.SetAsBox(15/SCALE, 1.5/SCALE);
    if (dir)
    {
        bdef.position.Set (pos.x - 1.9, pos.y + 0.41);
        bullet_sprite.rotate(180);
    }
    else bdef.position.Set (pos.x + 1.9, pos.y + 0.41);
    bullet_dir = dir;
    bdef.type = b2_dynamicBody;
    bulletHB = World.CreateBody(&bdef);
    bulletHB->SetGravityScale(0);
    fdef.density = 0;
    fdef.isSensor = true;
    fdef.shape = &bullet_shape;
    bulletHB->CreateFixture (&fdef);
    Connect();
}

void Bullet::update()
{
    float time = clock.getElapsedTime().asMicroseconds();
    clock.restart();
    time = time/800;
    life_time += time;
    if (life_time > 5000) life = false;
    Connect();
    bullet_pos = bulletHB -> GetPosition();
}

bool Bullet::checkCollision(vector<Player>& player)
{
    b2Vec2 pos = bullet_pos;

    pos.x += (bullet_dir ?  15/SCALE : -15/SCALE);

    for (b2Body* it = World.GetBodyList(); it != 0; it = it -> GetNext())
    {
        if (it != bulletHB)
        {
            for (int i = 0; i<player.size(); i++)
            {
                if (it == player[i].getHB())
                for (b2Fixture *f = it->GetFixtureList(); f != 0; f = f->GetNext())
                    if (f->TestPoint(pos))
                    {
                        player[i].changeHP(-(5 + rand() % 2));
                        if (bullet_dir) player[i].getHB()->ApplyLinearImpulse (b2Vec2 (-10000/(player[i].getHP()+20),-50), getHB()->GetWorldCenter(), true );
                        else player[i].getHB()->ApplyLinearImpulse (b2Vec2 (10000/(player[i].getHP()+20),-50), getHB()->GetWorldCenter(), true );
                        return true;
                    }
            }
            for (b2Fixture *f = it->GetFixtureList(); f != 0; f = f->GetNext())
                if (f->TestPoint(pos))
                    return true;
        }
    }

    return false;
}

void Bullet::Connect ()
{
    bullet_sprite.setPosition (bullet_pos.x * SCALE, bullet_pos.y * SCALE);
}

void Bullet::deleteHB()
{
    World.DestroyBody(bulletHB);
}

bool Bullet::getLife()
{
    return life;
}

b2Body* Bullet::getHB()
{
    return bulletHB;
}

Sprite Bullet::getSprite()
{
    return bullet_sprite;
}

void Weapon::updateGunAngle (RenderWindow& window, vector<Player>& player)
{
    Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
    Vector2f pos1 = window.mapPixelToCoords(pixelPos);
    float dX = pos1.x-player[0].getPosition().x*SCALE;//вектор , колинеарный пр€мой, котора€ пересекает спрайт и курсор
    float dY = pos1.y-player[0].getPosition().y*SCALE;//он же, координата y
    float rotation = (atan2(dY, dX)) * 180 / 3.14159265;//получаем угол в радианах и переводим его в градусы
    gunAngle = rotation;
}

void Weapon::setGunAngle(vector<Player>& player) // ‘ункци€ дл€ поворота оружи€ в сторону курсора ( дл€ доп. режима)
{

    // “аймер
    static Clock clocks;
    static double Timer = 200 ;
    double Time;
    Time = clocks.getElapsedTime().asMicroseconds();
    clocks.restart();
    Time = Time/800;
    Timer += Time;

    // Ќиже представлен участок кода, отвечающий за поворот персонажа с использованием мышки.
    // ќднако, он не доделан

//
//    if (gunAngle <= 90 && gunAngle > 0)
//    {
//        if (oldDirection == true)
//        gunAngle = 39;
//        direction = false;
//        gun_sprite.setRotation( gunAngle );
//    }
//
//    if (gunAngle > -90 && gunAngle < 0)
//    {
//        if (oldDirection == true)
//        gunAngle = -39;
//        direction = false;
//        gun_sprite.setRotation( gunAngle );
//    }
//
//    if (gunAngle > 90 && gunAngle < 180)
//    {
//        if (oldDirection == false)
//        gunAngle = 141;
//        direction = true;
//        gun_sprite.setRotation( gunAngle );
//    }
//
//    if (gunAngle < -90 && gunAngle > -180)
//    {
//        if (oldDirection == false)
//        gunAngle = -141;
//        direction = true;
//        gun_sprite.setRotation( gunAngle );
//    }
//


    //  остыль дл€ фикса бага с позицией текстуры рпи повороте
    static bool oldDirection = player[0].getDirection();

    if (oldDirection !=  player[0].getDirection())
    {
        if (! player[0].getDirection()) gunAngle = 0;
        else gunAngle = -180;

    }

    oldDirection =  player[0].getDirection();

    // ѕремещение спрайта с оружием в сторону курсора ( диапазон: +- 40 градусов с каждой стороны)
    if (gunAngle < 40 && gunAngle >= 0 && Timer > 15 && ! player[0].getDirection())
    {
        gun_position.x = 40 + gunAngle * 0.1;
        gun_position.y = gunAngle * 0.6;
        gun.setRotation( gunAngle );
        Timer = 0;
    }

    if (gunAngle < 0 && gunAngle > -40 && Timer > 15 && ! player[0].getDirection())
    {
        gun_position.x = 40 + gunAngle * 0.55;
        gun_position.y = gunAngle * 0.5;
        gun.setRotation( gunAngle );
        Timer = 0;
    }

    if (gunAngle > 140 && gunAngle <= 179 && Timer > 15 &&  player[0].getDirection())
    {
        gun_position.x = 40 + (gunAngle - 180) * -0.1;
        gun_position.y = (gunAngle - 180) * -0.6;
        gun.setRotation( gunAngle - 180 );
        Timer = 0;
    }

    if (gunAngle < -140 && gunAngle >= -180 && Timer > 15 &&  player[0].getDirection())
    {
        gun_position.x = 40 + (gunAngle + 180) * -0.55;
        gun_position.y = (gunAngle + 180) * -0.5;
        gun.setRotation( (gunAngle + 180) );
        Timer = 0;
    }

}

Sprite Weapon::getGunSprite ()
{
    return gun;
}

void Weapon::setGunSpriteRect(IntRect rectangle)
{
    gun.setTextureRect(rectangle);
}

Ammo Weapon::getAmmo()
{
    return ammo;
}

float Weapon::getReloadTime()
{
    return ReloadTime;
}

b2Vec2 Weapon::getPosition()
{
    return gun_position;
}

bool Weapon::getReloading()
{
    return Reloading;
}

void Weapon::setAmmo(bool current, int n)
{
    if (current)
    {
        if (ammo.current > 0) ammo.current += n;
        else Reloading = true;
        if (ammo.current > ammo.max) ammo.current = ammo.max;
    }
    else ammo.max = n;
}

void Weapon::updateReloading()
{
    time = clock.getElapsedTime().asMicroseconds();
    clock.restart();
    time = time/800;
    ReloadTime += time;
    if (!Reloading) ReloadTime = 0;
    else if (ReloadTime >= 4000)
    {
        Reloading = false;
        ammo.current = ammo.max;
    }
}

void Weapon::setGunSprite(Texture& texture)
{
    gun.setTexture(texture);
    gun.setOrigin(40,40);
    gun.setScale(0.7f, 0.7f);
    gun.setTextureRect(IntRect(560, 221, 80 , 110));
}

void Weapon::setPosition(float x, float y)
{
    gun.setPosition(x,y);
}

Bullet::Bullet(b2Vec2 pos, bool dir)
{
    create(pos, dir);
    update();
}
