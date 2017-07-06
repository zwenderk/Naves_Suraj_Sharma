#pragma once

#include "Player.h"
#include "Enemy.h"

class Game
{
private:
    RenderWindow *window;
    float dtMultiplier;

    // Texto
    Font font;
    Text followPlayerText;
    Text staticPlayerText;
    Text enemyText;
    Text gameOverText;

    //Players
    std::vector<Player> players;
    int playersAlive;

    // Enemigos
    std::vector<Enemy> enemies;
    std::vector<Enemy> enemiesSaved;
    float enemySpawnTimer; // Temporizador de salida de enemigos
    float enemySpawnTimerMax;

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
    void UpdateUIPlayer(int index); // Actualiza letreros player
    void UpdateUIEnemy(int index); // Actualiza letreros enemigo
    void Update(const float &dt);
    void DrawUI();   // Dibuja letreros
    void Draw();
};

