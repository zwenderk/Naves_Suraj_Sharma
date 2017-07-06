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

    this->enemies.push_back(Enemy(
            &this->textures[enemy01], window->getSize(), // Textura y  limites de ventana
            Vector2f(0.f, 0.f),        // Posición
            Vector2f(-1.f, 0.f),       // Dirección
            Vector2f(0.1f, 0.1f),      // Escala
            0, rand() % 3 + 1, 3, 1)); // tipo, energia máxima, daño máximo, daño minimo

    this->enemies.push_back(Enemy(
            &this->textures[enemy01], window->getSize(), // Textura y  limites de ventana
            Vector2f(0.f, 0.f),        // Posición
            Vector2f(-1.f, 0.f),       // Dirección
            Vector2f(0.1f, 0.1f),      // Escala
            0, rand() % 3 + 1, 3, 1)); // tipo, energia máxima, daño máximo, daño minimo

    this->enemies.push_back(Enemy(
            &this->textures[enemy01], window->getSize(), // Textura y  limites de ventana
            Vector2f(0.f, 0.f),        // Posición
            Vector2f(-1.f, 0.f),       // Dirección
            Vector2f(0.1f, 0.1f),      // Escala
            0, rand() % 3 + 1, 3, 1)); // tipo, energia máxima, daño máximo, daño minimo

    this->enemies.push_back(Enemy(
            &this->textures[enemy01], window->getSize(), // Textura y  limites de ventana
            Vector2f(0.f, 0.f),        // Posición
            Vector2f(-1.f, 0.f),       // Dirección
            Vector2f(0.1f, 0.1f),      // Escala
            0, rand() % 3 + 1, 3, 1)); // tipo, energia máxima, daño máximo, daño minimo



    /*this->players.push_back(Player(this->textures,
          Keyboard::Nunpad8, Keyboard::Nunpad5,
          Keyboard::Nunpad4, Keyboard::Nunpad6,
          Keyboard::Nunpad0 ));*/

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
        tempText.setColor(Color::White);
        tempText.setString(std::to_string(i));

        this->followPlayerTexts.push_back(Text(tempText));



        //Static Text Init (Arriba a la izquierda en la pantalla)
        tempText.setFont(font);
        tempText.setCharacterSize(14);
        tempText.setColor(Color::White);
        tempText.setString(std::to_string(i));

        this->staticPlayerTexts.push_back(Text(tempText));
    }
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
    for (size_t i = 0; i < this->players.size(); i++) // Para cada player
    {
        bool enemyRemoved = false;
        bool bulletRemoved = false;

        //UPDATE PLAYERS
        this->players[i].Update(this->window->getSize());

        //Bullets update (si no está borrado el proyectil)
        for(size_t k = 0; k < this->players[i].getBullets().size() && !bulletRemoved ; k++)
        {
            this->players[i].getBullets()[k].Update();

            // Prueba de colisión de enemigo si no está borrado
            for(size_t j = 0; j < this->enemies.size() && !enemyRemoved; j++)
            {
               // Si proyectil choca con enemigo, destruir proyectil y enemigo
                if (this->players[i].getBullets()[k].getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
                {
                    this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);
                    this->enemies.erase(this->enemies.begin() + j);

                    enemyRemoved = true;
                    bulletRemoved = true;
                }

                // al entrar entero en la izquierda de la ventana borrar enemigo
                else if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width)
                {
                    this->enemies.erase(this->enemies.begin() + i);
                    enemyRemoved = true;
                }
            }

            //Window bounds check
            // Si proyectil sale por la derecha y no está borrado, borrarlo
            if (!bulletRemoved && this->players[i].getBullets()[k].getPosition().x > this->window->getSize().x)
            {
                this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);

                bulletRemoved = true;
            }
        }
    }

    for(size_t i = 0; i < enemies.size(); i++)
    {
        this->enemies[i].Update();
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
        this->enemies[i].Draw(*this->window);
    }

    this->DrawUI();

    this->window->display();
}