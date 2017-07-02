#pragma once

#include "Player.h"

class Game
{
private:
    RenderWindow *window;

    //Text
    Font font;
    std::vector<Text> followPlayerTexts;
    std::vector<Text> staticPlayerTexts;

    //Players
    std::vector<Player> players;

    //Textures
    Texture playerTexture;
    Texture bulletTexture;
public:
    Game(RenderWindow *window);
    virtual ~Game();

    //Accessors
    // Ojo referencia a clase window
    inline  RenderWindow& getWindow() {return *this->window;}

    //Setters

    //Functions
    void InitUI(); // Inicializa letreros
    void UpdateUI(); // Actualiza letreros
    void Update();
    void DrawUI(); // Dibuja letreros
    void Draw();
};

