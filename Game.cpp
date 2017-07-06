#include "Game.h"

enum textures{player = 0, laser01, missile01, mainGun01, enemy01};

Game::Game(RenderWindow *window)
{
    this->window = window;
    this->window->setFramerateLimit(60);

    //Inicializa fonts
    this->font.loadFromFile("../Fonts/Dosis-Light.ttf");

    //Inicializa texturas
    this->textures.push_back(Texture()); // Textura de la nave
    this->textures[player].loadFromFile("../Textures/ship.png");

    this->textures.push_back(Texture()); // Textura del proyectil rayo 01
    this->textures[laser01].loadFromFile("../Textures/Guns/rayTex01.png");

    this->textures.push_back(Texture()); // Textura del proyectil 01
    this->textures[missile01].loadFromFile("../Textures/Guns/missileTex01.png");

    this->textures.push_back(Texture()); // Textura cañon
    this->textures[mainGun01].loadFromFile("../Textures/Guns/gun01.png");

    this->textures.push_back(Texture()); // Textura enemigo
    this->textures[enemy01].loadFromFile("../Textures/enemy.png");

    //Inicializa players
    this->players.push_back(Player(this->textures));

    // Inicializa enemigos
    Enemy e1(
            &this->textures[enemy01], window->getSize(), // Textura y  limites de ventana
            Vector2f(0.f, 0.f),        // Posición
            Vector2f(-1.f, 0.f),       // Dirección
            Vector2f(0.1f, 0.1f),      // Escala
            0, rand() % 3 + 1, 3, 1);  // tipo, energia máxima, daño máximo, daño minimo

    this->enemiesSaved.push_back(Enemy(e1)); // Mete enemigo en vector enemiesSaved

    this->enemySpawnTimerMax = 20; // Tiempo máximo entre enemigos
    this->enemySpawnTimer = this->enemySpawnTimerMax;


    /*this->players.push_back(Player(this->textures,
          Keyboard::Numpad8, Keyboard::Numpad5,
          Keyboard::Numpad4, Keyboard::Numpad6,
          Keyboard::Numpad0 ));*/

    this->InitUI();
}

Game::~Game()
{
}

void Game::InitUI() // Inicializa letreros
{
    Text tempText;

    for (size_t i = 0; i < this->players.size(); i++)
    {
        //Follow Text Init (texto en el player)

        tempText.setFont(font);
        tempText.setCharacterSize(14);
        //tempText.setColor(Color::White); // Obsoleto
        tempText.setOutlineColor(Color::White);
        tempText.setString(std::to_string(i));

        this->followPlayerTexts.push_back(Text(tempText));

        //Static Text Init (Arriba a la izquierda en la pantalla)
        tempText.setFont(font);
        tempText.setCharacterSize(14);
        //tempText.setColor(Color::White); // Obsoleto
        tempText.setOutlineColor(Color::White);
        tempText.setString(std::to_string(i));

        this->staticPlayerTexts.push_back(Text(tempText));
    }

    // Texto en enemigos
    this->enemyText.setFont(this->font);
    this->enemyText.setCharacterSize(14);
    this->enemyText.setFillColor(Color::White);
}

void Game::UpdateUI() // Actualiza letreros
{
    for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
    {
        // Poner el texto en la posición del player
        this->followPlayerTexts[i].setPosition(this->players[i].getPosition().x, this->players[i].getPosition().y - 20.f);
        // Letras a poner
        this->followPlayerTexts[i].setString(std::to_string(i) + "                   " + this->players[i].getHpAsString());
    }

    for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
    {

    }
}


void Game::Update()
{
    //Update timers
    if (this->enemySpawnTimer < this->enemySpawnTimerMax)
        this->enemySpawnTimer++;

    //Spawn enemies
    if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
    {
        this->enemies.push_back(Enemy(
                &this->textures[enemy01], this->window->getSize(),
                Vector2f(0.f, 0.f), // Posición
                Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f), // Dirección y escala
                0, rand() % 3 + 1, 3, 1)); // Tipo, máxima energia, maximo daño, minimo daño

        this->enemySpawnTimer = 0; //Reset timer
    }

    for (size_t i = 0; i < this->players.size(); i++) // Para cada player
    {
        //UPDATE PLAYERS
        this->players[i].Update(this->window->getSize());

        //Bullets update
        for(size_t k = 0; k < this->players[i].getBullets().size(); k++)
        {
            this->players[i].getBullets()[k].Update();

            // Prueba de colisión de enemigo
            for(size_t j = 0; j < this->enemies.size(); j++)
            {
                // Si proyectil choca con enemigo, destruir proyectil y enemigo
                if (this->players[i].getBullets()[k].getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
                {
                    this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);

                    if (this->enemies[j].getHP() > 0) // Si enemigo tiene mas energia que 0
                        this->enemies[j].takeDamage(this->players[i].getDamage()); // Hacerle daño y bajarle la energia
                    if (this->enemies[j].getHP() <= 0) // Si enemigo tiene energia 0 o negativa
                        this->enemies.erase(this->enemies.begin() + j); // Borrarlo
                    return; //RETURN!!!!!!!!
                }
            }

            //Window bounds check
            // Si proyectil sale por la derecha, borrarlo
            if (this->players[i].getBullets()[k].getPosition().x > this->window->getSize().x)
            {
                this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);
                return; //RETURN!!!!!!!!!!!!
            }
        }
    }

    for(size_t i = 0; i < enemies.size(); i++)
    {
        this->enemies[i].Update();

        // al entrar entero en la izquierda de la ventana borrar enemigo
        if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width)
        {
            this->enemies.erase(this->enemies.begin() + i);
            break; //RETURN!!!!!!!!!!
        }
    }

    //UPDATE UI
    this->UpdateUI();
}

void Game::DrawUI()
{
    for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
    {
        // Dibujar el texto en la posición del player
        this->window->draw(this->followPlayerTexts[i]);

    }

    for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
    {
        this->window->draw(this->staticPlayerTexts[i]);
    }
}

void Game::Draw()
{
    this->window->clear();
    for (size_t i = 0; i < this->players.size(); i++)
    {
        this->players[i].Draw(*this->window);
    }

    for (size_t i = 0; i < this->enemies.size(); i++)
    {
        // Posiciona el texto del enemigo
        this->enemyText.setPosition(this->enemies[i].getPosition());
        this->enemyText.setString(
                std::to_string(this->enemies[i].getHP()) +
                "/" +
                std::to_string(this->enemies[i].getHPMax()));

        this->enemies[i].Draw(*this->window); // Dibuja enemigo
        this->window->draw(this->enemyText); // Dibuja texto de enemigo

    }

    this->DrawUI();

    this->window->display();
}