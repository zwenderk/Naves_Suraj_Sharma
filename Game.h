#pragma once

#include "Player.h"
#include "Enemy.h"

class Game
{
private:
    RenderWindow *window;

    // Texto
    Font font;
    std::vector<Text> followPlayerTexts;
    std::vector<Text> staticPlayerTexts;

    //Players
    std::vector<Player> players;

    // Enemigos
    std::vector<Enemy> enemies;

    // Texturas
    std::vector<Texture> textures;

public:
    Game(RenderWindow *window);
    virtual ~Game();

    //Accessors - Ojo referencia a clase window
    inline  RenderWindow& getWindow() {return *this->window;}

    //Setters

    // Funciones
    void InitUI();   // Inicializa letreros
    void UpdateUI(); // Actualiza letreros
    void Update();
    void DrawUI();   // Dibuja letreros
    void Draw();
};

