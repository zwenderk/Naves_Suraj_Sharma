#include "Game.h"

Game::Game(RenderWindow *window)
{
    this->window = window;
    this->window->setFramerateLimit(60);

    //Init fonts
    this->font.loadFromFile("../Fonts/Dosis-Light.ttf");

    //Init textures
    this->playerTexture.loadFromFile("../Textures/ship.png");
    this->bulletTexture.loadFromFile("../Textures/Guns/missileTex01.png");
    this->gunTexture01.loadFromFile("../Textures/Guns/gun01.png");

    //Init players
    this->players.push_back(Player(&playerTexture, &bulletTexture, &gunTexture01));

    /*this->players.push_back(Player(&playerTexture, &bulletTexture,
          Keyboard::I, Keyboard::K, Keyboard::J, Keyboard::L, Keyboard::RShift ));*/

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
        // Poner el texto en la posici�n del player
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
    for (size_t i = 0; i < this->players.size(); i++)
    {
        //UPDATE PLAYERS
        this->players[i].Update(this->window->getSize());

        //Bullets update
        for(size_t k = 0; k < this->players[i].getBullets().size(); k++)
        {
            this->players[i].getBullets()[k].Update();


            //Window bounds check
            // Si la bala supera la posici�n derecha de la ventana borrarla
            if (this->players[i].getBullets()[k].getPosition().x > this->window->getSize().x)
            {
                this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);
                break; //BREAK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            }

            //Enemy collision check
        }
    }

    //UPDATE UI
    this->UpdateUI();
}

void Game::DrawUI()
{
    for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
    {
        // Dibujar el texto en la posici�n del player
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

    this->DrawUI();

    this->window->display();
}