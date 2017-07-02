#include "Game.h"

Game::Game(RenderWindow *window)
{
    this->window = window;
    this->window->setFramerateLimit(60);

    //Init textures
    this->playerTexture.loadFromFile("../Textures/ship.png");
    this->bulletTexture.loadFromFile("../Textures/Guns/missileTex01.png");

    //Init players
    this->players.push_back(Player(&playerTexture, &bulletTexture));
    this->players.push_back(Player(&playerTexture, &bulletTexture,
          Keyboard::I, Keyboard::K, Keyboard::J, Keyboard::L, Keyboard::RShift ));
}

Game::~Game()
{
}

void Game::CombatUpdate()
{
}

void Game::Update()
{
    for (size_t i = 0; i < this->players.size(); i++)
    {
        this->players[i].Update(this->window->getSize());

        //Bullets update

        for(size_t k = 0; k < this->players[i].getBullets().size(); k++)
        {
            this->players[i].getBullets()[k].Update();


            //Window bounds check
            // Si la bala supera la posición derecha de la ventana borrarla
            if (this->players[i].getBullets()[k].getPosition().x > this->window->getSize().x)
            {
                this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);
                break; //BREAK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            }

            //Enemy collision check
        }
    }
}

void Game::Draw()
{
    this->window->clear();
    for (size_t i = 0; i < this->players.size(); i++)
    {
        this->players[i].Draw(*this->window);
    }

    this->window->display();
}