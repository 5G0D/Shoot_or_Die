
#include "Interface.h"

PlayerStats::PlayerStats()
{
    setBarTextures();
    setTexture();
    setText();
}

void Bar::setBarTextures()
{
    emptyBarT.loadFromFile("./Resources/Textures/Bar/empty.png");
   // emptyBarT.setSmooth(true);
    emptyBar.setTexture(emptyBarT);
    emptyBar.setScale(0.8, 0.8);
    emptyBar.setTextureRect(IntRect(0, 0, 292, 124));

    HPBarT.loadFromFile("./Resources/Textures/Bar/full.png");
    HPBarT.setSmooth(true);
    HPBar.setTexture(HPBarT);
    HPBar.setScale(0.8 , 0.8);
    HPBar.setTextureRect(IntRect(0, 0, 292, 124));

    AmmoBarT.loadFromFile("./Resources/Textures/Bar/ammo.png");
    AmmoBarT.setSmooth(true);
    AmmoBar.setTexture(AmmoBarT);
    AmmoBar.setScale(0.8 , 0.8);
    AmmoBar.setTextureRect(IntRect(0, 0, 292, 124));

    LifesT.loadFromFile("./Resources/Textures/Bar/lifes.png");
    LifesT.setSmooth(true);
    Lifes.setTexture(LifesT);
    Lifes.setScale(0.8 , 0.8);
    Lifes.setTextureRect(IntRect(0, 0, 292, 124));
}

void PlayerStats::setPosition(b2Vec2 pos)
{
    PlayerStatsS.setPosition(pos.x, pos.y);
    emptyBar.setPosition(pos.x, pos.y);
    HPBar.setPosition(pos.x, pos.y);
    AmmoBar.setPosition(pos.x, pos.y);
    Lifes.setPosition(pos.x, pos.y);
}

void PlayerStats::update(float HP, int i, Ammo ammo, String name, bool reload, float ReloadTime, int lifes, int currentSkin)
{
    ////

    if (currentSkin != lastSkin)
    {
        string link = "./Resources/Textures/PlayerStats/PlayerStats";
        link += to_string(currentSkin);
        link += ".png";
        PlayerStatsT.loadFromFile(link);
        lastSkin = currentSkin;
    }

    ////
    setPosition({20 + (i*350), 20});
    Name.setPosition({100 + (i*350), 47});
    Name.setString(name);

    Reload = reload;
    if (ReloadTime > 1000)
    {
        if (ReloadTime > 2000)
        {
            if (ReloadTime > 3000) Reloading.setString("Reloading...");
            else Reloading.setString("Reloading..");
        }
        else Reloading.setString("Reloading.");
    }
    else Reloading.setString("Reloading");
    Reloading.setPosition({152 + (i*350), 88});

    HPBar.setTextureRect(IntRect(0, 0, 114 + 1.74*HP, 124));
    AmmoBar.setTextureRect(IntRect(0, 0, 164 + (115.f/ammo.max)*ammo.current, 124));
    Lifes.setTextureRect(IntRect(0, 0, 20 + (23*lifes), 124));
}

void Interface::Draw(RenderWindow& window)
{
    for (int i = 0; i<Emount; i++)
    {
        PlayerStats[i].draw(window);
    }
}

void Interface::Update(vector<Player>& player)
{
    Emount = player.size();
    for (int i = 0; i < player.size(); i++)
    {
        PlayerStats[i].update(player[i].getHP(), i, player[i].getWeapon().getAmmo(), player[i].getName(), player[i].getWeapon().getReloading(), player[i].getWeapon().getReloadTime(), player[i].getLifes(), player[i].getCurrentSkin());
    }

}

void Interface::setTextures()
{
    for (int i = 0; i < 4 ; i++)
    {
        PlayerStats[i].setTexture();
    }
}

void PlayerStats::setTexture()
{
    PlayerStatsT.setSmooth(true);
    PlayerStatsS.setTexture(PlayerStatsT);
    PlayerStatsS.setScale(0.8, 0.8);
    PlayerStatsS.setTextureRect(IntRect(0, 0, 292, 124));
}

void PlayerStats::draw(RenderWindow& window)
{
    window.draw(PlayerStatsS);
    window.draw(emptyBar);
    window.draw(HPBar);
    window.draw(AmmoBar);
    window.draw(Name);
    window.draw(Lifes);
    if (Reload) window.draw(Reloading);
}

void PlayerStats::setText()
{
    font.loadFromFile("./Resources/Fonts/ObelixPro.ttf");

    Name.setFont(font);
    Name.setColor(Color::Black);
    Name.setCharacterSize(15);

    Reloading.setFont(font);
    Reloading.setString("Reloading");
    Reloading.setCharacterSize(10);
}
