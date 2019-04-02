
#include "Engine.h"
#include "StartScreen.h"

int main()
{
    srand( time(0) );
    sf::RenderWindow window(sf::VideoMode(), "Shoot or Die", sf::Style::Fullscreen);
    window.setFramerateLimit(70);

    while (1)
    {
        StartScreen startscreen(&window);
        startscreen.Play();
        Engine engine(&window);
        engine.Play();
    }
}
