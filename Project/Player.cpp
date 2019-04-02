
#include "Player.h"
#include "Engine.h"


Player::Player()
{
    setWeapon();
    HB_create();
    LoadSkin(true);
    setSounds();
}

void Player::LoadSkin(bool dir)
{

    dir ? currentSkin++ : currentSkin --;

    if (currentSkin <= 0) currentSkin = 5;
    if (currentSkin >= 6) currentSkin = 1;
    switch (currentSkin)
    {
        case 1:
            texture_skin.loadFromFile("./Resources/Textures/Adventurer/adventurer_tilesheet.png");
            break;
        case 2:
            texture_skin.loadFromFile("./Resources/Textures/Female/female_tilesheet.png");
            break;
        case 3:
            texture_skin.loadFromFile("./Resources/Textures/Male/male_tilesheet.png");
            break;
        case 4:
            texture_skin.loadFromFile("./Resources/Textures/Soldier/soldier_tilesheet.png");
            break;
        case 5:
            texture_skin.loadFromFile("./Resources/Textures/Zombie/zombie_tilesheet.png");
            break;
    }
    texture_skin.setSmooth(true);

    sprite.setTexture(texture_skin);
    sprite.setOrigin(40,40);
    sprite.setScale(0.7f, 0.7f);
    sprite.setTextureRect(IntRect(480, 221, 80 , 110));

}

void Player::HB_create ()
{
        fdef.friction = 0.5;
        fdef.density = 2.8;
        fdef.shape = &hero_body;

        fdef2.friction = 0;
        fdef2.density = 2.8;
        fdef2.shape = &hero_head;

        bdef.position.Set ((200 + rand () % 400)/SCALE, -600/SCALE);
        bdef.type = b2_dynamicBody;
        heroHB = World.CreateBody(&bdef);
        hero_head.SetAsBox(25/SCALE, 22/SCALE);
        hero_body.SetAsBox(25/SCALE, 24/SCALE, b2Vec2 (0/SCALE, 23/SCALE), 0);
        heroHB -> CreateFixture (&fdef);
        heroHB -> CreateFixture (&fdef2);
        status = true;
}

b2Vec2 Player::getSpeed ()
{
    speed = heroHB -> GetLinearVelocity();
    return speed;
}

b2Vec2 Player::getPosition ()
{
    position = heroHB -> GetPosition();
    return position;
}

Sprite Player::getSprite()
{
    return sprite;
}

bool Player::getDirection()
{
    return direction;
}

bool Player::getOnGround ()
{
    return onGround;
}

bool Player::getSit ()
{
    return Sit;
}

bool Player::getShooting()
{
    return Shooting;
}

int Player::getLifes()
{
    return Lifes;
}

string Player::getName()
{
    return(Name);
}

int Player::getCurrentSkin()
{
    return currentSkin;
}

int Player::getHP()
{
    return hp;
}

b2Body* Player::getHB()
{
    return heroHB;
}

void Player::changeHP(int HP)
{
    hp += HP;
}

void Player::setControl(bool type)
{
    control_type = type;
}

Weapon Player::getWeapon()
{
    return weapon;
}

void Player::StandHB (double x, double y)
{
    if (!status)
    {
        World.DestroyBody(heroHB);
        bdef.position.Set(x, y);
        bdef.type = b2_dynamicBody;
        heroHB = World.CreateBody(&bdef);
        hero_head.SetAsBox(25/SCALE, 22/SCALE);
        hero_body.SetAsBox(25/SCALE, 24/SCALE, b2Vec2 (0/SCALE, 23/SCALE), 0);
        heroHB -> CreateFixture (&fdef);
        heroHB-> CreateFixture (&fdef2);
        status = true;
    }
}

void Player::SitHB(double x, double y)
{
    if (status)
    {
        b2Vec2 vel = heroHB -> GetLinearVelocity();

        World.DestroyBody(heroHB);
        bdef.position.Set(x, y);
        bdef.type = b2_dynamicBody;
        heroHB = World.CreateBody(&bdef);
        hero_head.SetAsBox(25/SCALE, 19.5/SCALE);
        hero_body.SetAsBox(25/SCALE, 19/SCALE, b2Vec2 (0/SCALE, 19.25/SCALE), 0);
        heroHB -> CreateFixture (&fdef);
        heroHB -> CreateFixture (&fdef2);

        heroHB -> SetLinearVelocity(vel);
        status = false;
    }
}

void Player::Update()
{
    heroHB->SetTransform( heroHB->GetPosition(), 0 ); // —тавим угол поворота = 0;

    time = clock.getElapsedTime().asMicroseconds();
    clock.restart();
    time = time/800;
    anim_time += time;

    getSpeed();
    getPosition();

    weapon.updateReloading();

    updateSound();

    ////
        if (hp <= 0) hp = 0;
        if (position.y > 40)
        {
            deleteHB();
            HB_create();
            hp = 100;
            weapon.setAmmo(true, weapon.getAmmo().max);
            Lifes--;
        }
    ////
    if (!onGround && abs(speed.y) ==0)
    {
        if (direction) heroHB->ApplyLinearImpulse (b2Vec2 (200,0), getHB()->GetWorldCenter(), true );
        else heroHB->ApplyLinearImpulse (b2Vec2 (-200,0), getHB()->GetWorldCenter(), true );
    }
    ////

    if (speed.x > 0.1)
    {
        if (anim_time < 2000/speed.x)
        {
            if (!Shooting) sprite.setTextureRect(IntRect(80, 111, 80 , 110));
            else sprite.setTextureRect(IntRect(480, 0, 80 , 110));
        }
        else anim_time = 0;
        if (anim_time < 1000/speed.x)
        {
            if (!Shooting) sprite.setTextureRect(IntRect(0, 111, 80 , 110));
            else sprite.setTextureRect(IntRect(400, 0, 80 , 110));
        }
    }

    if (speed.x < -0.1)
    {
        if (anim_time < 2000/-speed.x)
        {
            if (!Shooting) sprite.setTextureRect(IntRect(160, 111, -80 , 110));
            else sprite.setTextureRect(IntRect(560, 0, -80 , 110));
        }
        else anim_time = 0;
        if (anim_time < 1000/-speed.x)
        {
            if (!Shooting) sprite.setTextureRect(IntRect(80, 111, -80 , 110));
            else sprite.setTextureRect(IntRect(480, 0, -80 , 110));
        }
    }

    if (abs(speed.x) < 0.1)
    {
        if (!direction) sprite.setTextureRect(IntRect(400, 221, 80 , 110));
        else sprite.setTextureRect(IntRect(480, 221, -80 , 110));
    }

    if (!direction && !onGround)
    {
        sprite.setTextureRect(IntRect(80, 0, 80 , 110));
    }

    if (direction && !onGround)
    {
        sprite.setTextureRect(IntRect(160, 0, -80 , 110));
    }

    getPosition();

    if (Sit)
    {
        if (onGround)
        {
            SitHB(position.x, position.y);

            sprite.setOrigin(40,50);

            if (direction) sprite.setTextureRect(IntRect(320, 0, -80 , 110));
            else sprite.setTextureRect(IntRect(240, 0, 80 , 110));
            if (speed.x > 2)
            {
                sprite.setTextureRect(IntRect(80, 221, 80 , 110));
            }
            if (speed.x < -2)
            {
                sprite.setTextureRect(IntRect(160, 221, -80 , 110));
            }

            if (check_firstsit && abs(speed.x) > 2)
            {
                if (direction) heroHB->ApplyLinearImpulse (b2Vec2 (-2500/SCALE,0), heroHB->GetWorldCenter(), true );
                else heroHB->ApplyLinearImpulse (b2Vec2 (2500/SCALE,0), heroHB->GetWorldCenter(), true );
                check_firstsit = false;
            }
        }
    }
    else
    {
        check_firstsit = true;
        StandHB(position.x, position.y);
        sprite.setOrigin(40,40);
    }
    if (Shooting)
    {
        if (!direction)
        {
            if (speed.x < 0.1) sprite.setTextureRect(IntRect(480, 221, 80 , 110));
            if (abs (speed.y) > 0.1) sprite.setTextureRect(IntRect(320, 221, 80 , 110));
            if (Sit && abs(speed.x)< 0.2) sprite.setTextureRect(IntRect(560, 111, 80 , 110));
            weapon.setGunSpriteRect(IntRect(560, 221, 80 , 110));
        }
        else
        {
            if (speed.x > -0.1) sprite.setTextureRect(IntRect(560, 221, -80 , 110));
            if (abs (speed.y) > 0.1) sprite.setTextureRect(IntRect(400, 221, -80 , 110));
            if (Sit && abs(speed.x) < 0.2) sprite.setTextureRect(IntRect(640, 111, -80 , 110));
            weapon.setGunSpriteRect((IntRect(640, 221, -80 , 110)));
        }
    }
}

void Player::check_on()
{
    position.y += 55/SCALE;

    onGround = false;

    for (b2Body* it = World.GetBodyList(); it != 0; it = it -> GetNext())
        if (it != heroHB)
        for (b2Fixture *f = it->GetFixtureList(); f != 0; f = f->GetNext())
            if (f->TestPoint(position))
            {
                onGround = true;
                SecondJump = true;
            }
        else // ≈сли игрок будет сто€ть на другом игроке его будет скидывать
        for (b2Fixture *f = it->GetFixtureList(); f != 0; f = f->GetNext())
            if (f->TestPoint(position))
            {
                if (direction) heroHB->ApplyLinearImpulse (b2Vec2 (-20/SCALE,0), heroHB->GetWorldCenter(), true );
                else heroHB->ApplyLinearImpulse (b2Vec2 (20/SCALE,0), heroHB->GetWorldCenter(), true );
                onGround = true;
            }

    if (onGround == false) onJump = true;
    if (onGround && onJump)
    {
        timer = 100;
        onJump = false;
    }

    position.y -= 55/SCALE;

}

void Player::Control(vector<Bullet>& bullets)
{
    time = clocks.getElapsedTime().asMicroseconds();
    clocks.restart();
    time = time/800;
    timer += time;
    weapon_timer += time;
    check_on();
    Shooting = false;
    if (control_type)
    {
        if (Keyboard::isKeyPressed(Keyboard::G) && (timer >= 300))
        {
            LoadSkin(true);
            timer = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            if (onGround) timer = 0;

            if (!Sit && onGround && speed.y<300)
                heroHB->ApplyLinearImpulse (b2Vec2 (0,-50), heroHB->GetWorldCenter(), true );

            if (SecondJump == true && !onGround && (timer>500))
            {
                timer = 0;
                SecondJump = false;
                heroHB->SetLinearVelocity(b2Vec2 (0,-14));
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::A) && (speed.x > -5))
        {
            if (!Sit) heroHB->ApplyLinearImpulse (b2Vec2 (-400/SCALE,0), heroHB->GetWorldCenter(), true );
            direction = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::D) && (speed.x < 5))
        {
            if (!Sit) heroHB->ApplyLinearImpulse (b2Vec2 (400/SCALE,0), heroHB->GetWorldCenter(), true );
            direction = false;
        }
        if (Keyboard::isKeyPressed(Keyboard::Space) && !weapon.getReloading())
        {
            if (weapon_timer > 300 && (!Sit || abs(speed.x) < 0.2))
            {
                shot.play();
                weapon.setAmmo(true, -1);
                Bullet bullet(position, direction);
                if (direction) bullet.getHB()->SetLinearVelocity(b2Vec2 (-1000/SCALE,0));
                else bullet.getHB()->SetLinearVelocity(b2Vec2 (1000/SCALE,0));
                bullets.push_back(bullet);
                weapon_timer = 0;
            }
             if (!(Sit && abs(speed.x) > 0.2)) Shooting = true;
        }

        (Keyboard::isKeyPressed(Keyboard::S)) ? Sit = true : Sit = false;

    }
    else
    {
        if (Keyboard::isKeyPressed(Keyboard::L) && (timer >= 300))
        {
            LoadSkin(true);
            timer = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            if (onGround) timer = 0;

            if (!Sit && onGround && speed.y<300)
                heroHB->ApplyLinearImpulse (b2Vec2 (0,-50), heroHB->GetWorldCenter(), true );

            if (SecondJump == true && !onGround && (timer>500))
            {
                timer = 0;
                SecondJump = false;
                heroHB->SetLinearVelocity(b2Vec2 (0,-14));
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Left) && (speed.x > -5))
        {
            if (!Sit) heroHB->ApplyLinearImpulse (b2Vec2 (-400/SCALE,0), heroHB->GetWorldCenter(), true );
            direction = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) && (speed.x < 5))
        {
            if (!Sit) heroHB->ApplyLinearImpulse (b2Vec2 (400/SCALE,0), heroHB->GetWorldCenter(), true );
            direction = false;
        }
        if (Keyboard::isKeyPressed(Keyboard::Add) && !weapon.getReloading())
        {
            if (weapon_timer > 300 && (!Sit || abs(speed.x) < 0.2))
            {
                shot.play();
                weapon.setAmmo(true, -1);
                Bullet bullet(position, direction);
                if (direction) bullet.getHB()->SetLinearVelocity(b2Vec2 (-1000/SCALE,0));
                else bullet.getHB()->SetLinearVelocity(b2Vec2 (1000/SCALE,0));
                bullets.push_back(bullet);
                weapon_timer = 0;
            }
             if (!(Sit && abs(speed.x) > 0.2)) Shooting = true;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))

            Sit = true;

        else

            Sit = false;
    }
}

void Player::Connect ()
{
    sprite.setPosition (position.x * SCALE, position.y * SCALE);
    if (direction) weapon.setPosition (position.x * SCALE - weapon.getPosition().x, position.y * SCALE + weapon.getPosition().y);
    else weapon.setPosition (position.x * SCALE + weapon.getPosition().x, position.y * SCALE + weapon.getPosition().y);
}

void Player::setSounds()
{
    step_s.loadFromFile("./Resources/Sounds/footstep.ogg");
    shot_s.loadFromFile("./Resources/Sounds/shot.wav");
    reload_s.loadFromFile("./Resources/Sounds/reload.wav");
    fall_s.loadFromFile("./Resources/Sounds/fall.wav");
    glide_s.loadFromFile("./Resources/Sounds/glide.wav");
    hit_s.loadFromFile("./Resources/Sounds/hit.wav");

    hit.setBuffer(hit_s);
    hit.setVolume(50);

    step.setBuffer(step_s);
    step.setVolume(50);

    shot.setBuffer(shot_s);
    shot.setVolume(20);

    reload.setBuffer(reload_s);
    reload.setVolume(20);

    fall.setBuffer(fall_s);
    fall.setVolume(50);

    glide.setBuffer(glide_s);
    glide.setVolume(20);
}

void Player::deleteHB()
{
    World.DestroyBody(heroHB);
}

void Player::setWeapon()
{
    weapon.setGunSprite(texture_skin);
}

void Player::updateSound()
{
    if (weapon.getReloading() && reload.getStatus() == 0)
    {
        reload.play();
    }

    if (oldYPosition == false && onGround == true)
        fall.play();

    if (hp != oldHP)
    {
        hit.play();
    }

    oldHP = hp;
    oldYPosition = onGround;

    step.pause();
    if (abs(speed.x) > 1 && onGround && !Sit)
        step.play();

    glide.pause();
    if (abs(speed.x) > 1 && onGround && Sit)
        glide.play();
    else glide.stop();
}
