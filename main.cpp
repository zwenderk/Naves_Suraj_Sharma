#include "Game.h"

int main()
{
    srand(time(NULL));

    // Crea ventana
    RenderWindow window(VideoMode(1920, 1080), "Wingman Game", Style::Default);

    Clock clock;
    float dt = 0.f;

    //window.setFramerateLimit(60); // Limita frames por segundo

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

        dt = clock.restart().asSeconds();
        std::cout << dt << "\n";

        game.Update(dt);
        game.Draw();
    }
    return 0;
}