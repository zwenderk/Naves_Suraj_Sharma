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

    this->players.push_back(Player(this->textures,
      Keyboard::Numpad8, Keyboard::Numpad5,
      Keyboard::Numpad4, Keyboard::Numpad6,
      Keyboard::Numpad0 ));

    this->playersAlive = (int)this->players.size(); // PRUEBA puesto por mi (int)

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
        tempText.setString("");

        this->staticPlayerTexts.push_back(Text(tempText));
    }

    // Texto en enemigos
    this->enemyText.setFont(this->font);
    this->enemyText.setCharacterSize(14);
    this->enemyText.setFillColor(Color::White);

    // Texto Final de partida
    this->gameOverText.setFont(this->font);
    this->gameOverText.setFillColor(Color::Red);
    this->gameOverText.setCharacterSize(40);
    this->gameOverText.setString("GAME OVER!");
    this->gameOverText.setPosition(this->window->getSize().x/2 - 100.f, this->window->getSize().y / 2);
}

void Game::UpdateUI() // Actualiza letreros
{
    for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
    {
        // Poner el texto en la posición del player
        this->followPlayerTexts[i].setPosition(this->players[i].getPosition().x, this->players[i].getPosition().y - 20.f);
        // Letras a poner
        this->followPlayerTexts[i].setString(std::to_string(this->players[i].getPlayerNr()) + "                   " +
                                                     this->players[i].getHpAsString());
    }

    for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
    {

    }
}

void Game::Update()
{
    if (this->playersAlive > 0) // Si hay players
    {
        //Update timers
        if (this->enemySpawnTimer < this->enemySpawnTimerMax)
            this->enemySpawnTimer++;

        //Spawn enemies (lanza enemigo si ha pasado el tiempo adecuado)
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
        {
            this->enemies.push_back(Enemy(
                    &this->textures[enemy01], this->window->getSize(),
                    Vector2f(0.f, 0.f), // Posición
                    Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f), // Dirección y escala
                    0, rand() % 3 + 1, 2, 1)); // Tipo, máxima energia, maximo daño, minimo daño

            this->enemySpawnTimer = 0; //Reset timer
        }

        for (size_t i = 0; i < this->players.size(); i++) // Para cada player
        {
            if (this->players[i].isAlive())
            {
                //UPDATE PLAYERS
                this->players[i].Update(this->window->getSize());

                //Bullets update
                for (size_t k = 0; k < this->players[i].getBullets().size(); k++)
                {
                    this->players[i].getBullets()[k].Update();

                    // Prueba de colisión de enemigo y proyectil
                    for (size_t j = 0; j < this->enemies.size(); j++)
                    {
                        // Si proyectil choca con enemigo, destruir proyectil y enemigo
                        if (this->players[i].getBullets()[k].getGlobalBounds().intersects(
                                this->enemies[j].getGlobalBounds()))
                        {
                            this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);

                            if (this->enemies[j].getHP() > 0) // Si enemigo tiene mas energia que 0
                                this->enemies[j].takeDamage(
                                        this->players[i].getDamage()); // Hacerle daño y bajarle la energia
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
        }

        //Update enemies
        for (size_t i = 0; i < this->enemies.size(); i++)
        {
            this->enemies[i].Update();

            for (size_t k = 0; k < this->players.size(); k++) // Para todos los players
            {
                if (this->players[k].isAlive())
                {
                    // Si player choca con enemigo
                    if (this->players[k].getGlobalBounds().intersects(this->enemies[i].getGlobalBounds()))
                    {
                        this->players[k].takeDamage(this->enemies[i].getDamage()); // Player recibe daño y baja energia

                        if (!this->players[k].isAlive()) // Si player no está vivo
                            this->playersAlive--; // Reducir contador del players vivos

                        this->enemies.erase(this->enemies.begin() + i); // Borrar enemigo
                        return; //RETURN!!!!!!!!!!
                    }
                }
            }

            // al entrar entero en la izquierda de la ventana borrar enemigo
            if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width)
            {
                this->enemies.erase(this->enemies.begin() + i);
                return; //RETURN!!!!!!!!!!
            }
        }

        //UPDATE UI
        this->UpdateUI();
    }
}

void Game::DrawUI()
{
    for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
    {
        if (this->players[i].isAlive())
        {
            // Dibujar el texto en la posición del player si está vivo el player
            this->window->draw(this->followPlayerTexts[i]);
        }
    }

    for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
    {
        if (this->players[i].isAlive())
        {
            // Dibujar texto estático en la esquina izquierda superior si está vivo el player
            this->window->draw(this->staticPlayerTexts[i]);
        }
    }

    if (this->playersAlive <= 0)
    {
        this->window->draw(this->gameOverText);
    }
}

void Game::Draw()
{
    this->window->clear();

    for (size_t i = 0; i < this->players.size(); i++)
    {
        if (this->players[i].isAlive())
        {
            this->players[i].Draw(*this->window);
        }
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