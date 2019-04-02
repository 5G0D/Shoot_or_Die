
#include "Engine.h"
#include "StartScreen.h"
#include <windows.h>

int main()
{
    // Убираем консоль
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
    //

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
