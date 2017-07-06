#pragma once

#include "Player.h"
#include "Enemy.h"

class Game
{
private:
    RenderWindow *window;
    bool runGame;

    // Texto
    Font font;
    std::vector<Text> followPlayerTexts;
    std::vector<Text> staticPlayerTexts;
    Text enemyText;
    //Text gameOverText;

    //Players
    std::vector<Player> players;
    int alivePlayers;

    // Enemigos
    std::vector<Enemy> enemies;
    std::vector<Enemy> enemiesSaved;
    int enemySpawnTimer; // Temporizador de salida de enemigos
    int enemySpawnTimerMax;

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

