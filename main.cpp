#include "Game.h"

int main()
{
    // Crea ventana
    RenderWindow window(VideoMode(1920, 1080), "Wingman Game", Style::Default);
    window.setFramerateLimit(60); // Limita frames por segundo

    Game game(&window); // Crea objeto game y le envia el objeto window

    // Game loop (bucle principal)
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                window.close();
        }


        game.Update();
        game.Draw();
    }
    return 0;
}