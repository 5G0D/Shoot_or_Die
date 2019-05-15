
#include "StartScreen.h"

StartScreen::StartScreen(RenderWindow* window)
{
    this->window = window;
    bgT.loadFromFile("./Resources/Textures/StartScreen.png");
    bgT.setSmooth(true);
    bg.setTexture(bgT);
    bg.setTextureRect(IntRect(0, 0, 1920, 1080));

    font.loadFromFile("./Resources/Fonts/ObelixPro.ttf");

    text.setFont(font);
    text.setColor(Color::White);
    text.setCharacterSize(30);
    text.setPosition(960, 860);
    text.setString("Press any key to start");
    text.setOrigin(220, 15);
}

void StartScreen::Play()
{
    while (window->isOpen())
    {
        Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Resized)
            {
                FloatRect Area(0.f, 0.f, event.size.width, event.size.height);
                visibleArea = Area;
                window->setView(sf::View(visibleArea));
            }

            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (Keyboard::isKeyPressed(Keyboard::Escape))
                {
                    exit(0);
                }
                return;
            }
        }


        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/800;
        timer += time;

        if (textDir)
        {
            text.rotate(-0.5);
            if (text.getRotation() == 350) textDir = false;
        }

        else
        {
            text.rotate(0.5);
            if (text.getRotation() == 10) textDir = true;
        }
        view.setSize(1920, 1080);
        view.setCenter(960, 540);
        window->setView(view);
        window->clear();
        window->draw(bg);
        window->draw(text);
        window->display();
    }
}
