#include "Player.h"

unsigned Player::players = 0;

enum controls {UP = 0, DOWN, LEFT, RIGHT, SHOOT};

Player::Player(Texture *texture, Texture *bulletTexture,
               int UP, int DOWN,
               int LEFT, int RIGHT,
               int SHOOT)
        :level(1), exp(0), expNext(100),
         hp(10), hpMax(10),
         damage(1), damageMax(2),
         score(0)
{
    this->texture = texture;
    this->bulletTexture = bulletTexture;
    this->sprite.setTexture(*this->texture);
    this->sprite.setScale(0.13f, 0.13f); // Reduce el tamaño del sprite

    this->shootTimerMax = 25;
    this->shootTimer = this->shootTimerMax;
    this->damageTimer = 10;
    this->damageTimer = this->damageTimer;

    this->controls[controls::UP] = UP;
    this->controls[controls::DOWN] = DOWN;
    this->controls[controls::LEFT] = LEFT;
    this->controls[controls::RIGHT] = RIGHT;
    this->controls[controls::SHOOT] = SHOOT;

    // playerNr es el número de jugador
    this->playerNr = Player::players;
    Player::players++; // Incrementa variable player static
}

Player::~Player()
{
}

void Player::Movement()
{
    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::UP] )))
        this->sprite.move(0.f, -10.f);

    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::DOWN] )))
        this->sprite.move(0.f, 10.f);

    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::LEFT] )))
        this->sprite.move(-10.f, 0.f);

    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::RIGHT] )))
        this->sprite.move(10.f, 0.f);
}

void Player::Combat() // Si ha pasado el tiempo adecuado crear proyectil
{
    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::SHOOT] ))
        && this->shootTimer >= this->shootTimerMax)
    {
        this->bullets.push_back(
                Bullet(bulletTexture, // textura
                       this->playerCenter, // posición
                       Vector2f(1.f, 0.f), 5.f, // dirección, velocidad inicial
                       35.f, 0.5f)); // velocidad máxima, aceleración

        this->shootTimer = 0; //Reset Timer! (poner contador al inicio)
    }
}

void Player::Update(Vector2u windowBounds) // recibe limites en windowBounds
{
    //Update timers
    if (this->shootTimer < this->shootTimerMax)
        this->shootTimer++;

    if (this->damageTimer < this->damageTimerMax)
        this->damageTimer++;

    //Update positions
    this->playerCenter.x = this->sprite.getPosition().x +
            this->sprite.getGlobalBounds().width / 2;
    this->playerCenter.y = this->sprite.getPosition().y +
            this->sprite.getGlobalBounds().height / 2;


    this->Movement();
    this->Combat();
}

void Player::Draw(RenderTarget &target)
{
    for (size_t i = 0; i < this->bullets.size(); i++)
    {
        this->bullets[i].Draw(target);
    }

    target.draw(this->sprite);
}