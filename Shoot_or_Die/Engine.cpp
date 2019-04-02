
#include "Player.h"
#include "Engine.h"
#include "Box2D.h"

b2Vec2 Gravity (0.f, 25.f);
b2World World(Gravity);

Engine::Engine(RenderWindow* Window)
{
    window = Window;
    size = window->getSize();

    LoadingBarT.loadFromFile("./Resources/Textures/LoadingBar.png");
    LoadingBarT.setSmooth(true);
    LoadingBar.setTexture(LoadingBarT);
    LoadingBar.setTextureRect(IntRect(0, 0, 100, 50));
    LoadingBar.setPosition(710, 928);

    LoadingStatusT.loadFromFile("./Resources/Textures/LoadingStatus.png");
    LoadingStatusT.setSmooth(true);
    LoadingStatus.setTexture(LoadingStatusT);
    LoadingStatus.setTextureRect(IntRect(0, 0, 500, 50));
    LoadingStatus.setPosition(710, 868);

    LoadingScreenT.loadFromFile("./Resources/Textures/LoadingScreen.png");
    LoadingScreenT.setSmooth(true);
    LoadingScreen.setTexture(LoadingScreenT);
    LoadingScreen.setTextureRect(IntRect(0, 0, 1920, 1080));

    cout<<"Downloading players..."<<endl;

    LoadingDisplay();

    player.clear();
    player.resize(2);
    player[0].setControl(true);
    player[1].setControl(false);

    cout<<"Downloading map..."<<endl;

    LoadingStatus.setTextureRect(IntRect(0, 50, 500, 50));
    LoadingBar.setTextureRect(IntRect(0, 0, 250, 50));
    LoadingDisplay();

    interface.setTextures();

    env.clear();
    env.resize(5);
    env[0].Create(1, 400, 400);
    env[1].Create(2, 800, 250);
    env[2].Create(2, 0, 250);
    env[3].Create(2, 600, 100);
    env[4].Create(2, 200, 100);

    cout<<"Downloading Background..."<<endl;

    LoadingStatus.setTextureRect(IntRect(0, 100, 500, 50));
    LoadingBar.setTextureRect(IntRect(0, 0, 380, 50));
    LoadingDisplay();

    bg.setBackground(0);
    setSounds();
}

void Engine::LoadingDisplay()
{
    window->clear();
    window->draw(LoadingScreen);
    window->draw(LoadingBar);
    window->draw(LoadingStatus);
    window->display();
}

void Engine::Render (vector<Player>& player, vector<Bullet>& bullets, vector<Environment>& env, Background& bg, Interface interface)
{

    size = window->getSize();
    view = window->getDefaultView();

    if (view_center.y < 0) view_center.y = 0;
    if (view_center.y > 300) view_center.y = 300;

    view.setCenter(view_center.x, view_center.y);

    float k = ((max_pos.x - min_pos.x)>(max_pos.y - min_pos.y)) ? (max_pos.x - min_pos.x) : (max_pos.y - min_pos.y);
    view_size.x = size.x*k/10;
    view_size.y = size.y*k/10;
    if (k<=10)
    {
        view_size.x = size.x*10/10;
        view_size.y = size.y*10/10;
    }
    if (k>=13)
    {
        view_size.x = size.x*13/10;
        view_size.y = size.y*13/10;
    }

    view.setSize(view_size);

    window->setView(view);

    window->clear();

    for (int i = bg.getSize()-1; i > -1; i--)
    {
        window->draw(bg.getSprite(i));
    }

    for (int i = 0; i < env.size(); i++)
        window->draw(env[i].getSprite());

    for (int i = 0; i < player.size(); i++)
    {
         if (player[i].getShooting())
            window->draw (player[i].getWeapon().getGunSprite());
            window->draw (player[i].getSprite());
    }

    for (int i = 0; i < bullets.size(); i++)
        window->draw(bullets[i].getSprite());

    /////////////////
    window->setView(window->getDefaultView());
    window->setView(View(FloatRect(0, 0, size.x, size.y)));
    interface.Draw(*window);
    /////////////////
    window->display();
}

void Engine::Play ()
{

    while (window->isOpen())
    {
        Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Resized)
            {
                FloatRect Area(0.f, 0.f, event.size.width, event.size.height);
                window->setView(sf::View(Area));
            }
            if (event.type == sf::Event::Closed)
                window->close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            return;
        }

        World.Step(1/60.f, 8, 3);
        view_center = {0,0};
        max_pos = {0,0};
        min_pos = {1000,400};
        for (int i = 0; i < player.size(); i++)
        {
            player[i].Control(bullets);
            player[i].Update();
            player[i].Connect();
            b2Vec2 pos;
            pos = player[i].getPosition();
            if (pos.x<min_pos.x) min_pos.x = pos.x;
            if (pos.y<min_pos.y) min_pos.y = pos.y;
            if (pos.x>max_pos.x) max_pos.x = pos.x;
            if (pos.y>max_pos.y) max_pos.y = pos.y;
            view_center += pos;

            if (player[i].getSpeed().x!=0)
            {
                bg.move(player[i].getSpeed().x);
            }

        }
        view_center.x = view_center.x*SCALE/player.size();
        view_center.y = view_center.y*SCALE/player.size();

        for (int i = 0; i < bullets.size(); i++)
        {
            bullets[i].update();
            if (bullets[i].checkCollision(player) || !bullets[i].getLife())
            {
                bullets[i].deleteHB();
                bullets.erase(bullets.begin() + i);
                bullets.shrink_to_fit();
            }
        }

        for (int i = 0; i < env.size(); i++)
            env[i].Connect ();

        interface.Update(player);

        Render(player, bullets, env, bg, interface);

        for (int i = 0; i < player.size(); i++)
        {
            if (player[i].getLifes() == 0)
            {
                GameOver.play();
                while (GameOver.getStatus() == 2)
                {
                }
                return;
            }
        }
    }
}


void Engine::setSounds()
{
    GameOverS.loadFromFile("./Resources/Sounds/game_over.ogg");
    GameOver.setBuffer(GameOverS);
    GameOver.setVolume(50);

    string link = "./Resources/Sounds/Music";
    link += to_string(rand() % 3);
    link += ".wav";
    music.openFromFile(link);
    music.setLoop(true);
    music.setVolume(10);
    music.play();
}

Engine::~Engine()
{
    for (int i = 0; i < env.size(); i++)
        env[i].deleteHB();
    env.clear();

    for (int i = 0; i < player.size(); i++)
        player[i].deleteHB();
    player.clear();

    for (int i = 0; i < bullets.size(); i++)
        bullets[i].deleteHB();
    bullets.clear();
}
