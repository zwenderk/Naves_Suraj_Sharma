#include "Player.h"

unsigned Player::players = 0;

enum controls {UP = 0, DOWN, LEFT, RIGHT, SHOOT};

Player::Player(Texture *texture, Texture *bulletTexture, Texture *mainGunTexture,
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

    this->mainGunTexture = mainGunTexture;
    this->mainGunSprite.setTexture(*this->mainGunTexture);
    this->mainGunSprite.setOrigin( // Situa el cañon en el centro de la nave ???NO SE
            this->mainGunSprite.getGlobalBounds().width / 2,
            this->mainGunSprite.getGlobalBounds().height / 2);
    this->mainGunSprite.rotate(90); // Rota el cañon 90 grados

    this->shootTimerMax = 25;
    this->shootTimer = this->shootTimerMax;
    this->damageTimer = 10;
    this->damageTimer = this->damageTimer;

    this->controls[controls::UP] = UP;
    this->controls[controls::DOWN] = DOWN;
    this->controls[controls::LEFT] = LEFT;
    this->controls[controls::RIGHT] = RIGHT;
    this->controls[controls::SHOOT] = SHOOT;

    this->maxVelocity = 25.f;
    this->acceleration = 0.8f;
    this->stabilizerForce = 0.4f;

    // playerNr es el número de jugador
    this->playerNr = Player::players;
    Player::players++; // Incrementa variable player static
}

Player::~Player()
{
}

void Player::UpdateAccesories()
{
    this->mainGunSprite.setPosition(
            this->playerCenter.x + 20.f,
            this->playerCenter.y); // Situa cañon en la nave

}

void Player::Movement()
{
    //UP
    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::UP] )))
    {
        this->direction.x = 0.f;
        this->direction.y = -1.f;

        if (this->currentVelocity.y > -this->maxVelocity && this->direction.y < 0)
            this->currentVelocity.y += this->direction.y * this->acceleration;
    }

    //DOWN
    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::DOWN] )))
    {
        this->direction.x = 0.f;
        this->direction.y = 1.f;

        if (this->currentVelocity.y < this->maxVelocity && this->direction.y > 0)
            this->currentVelocity.y += this->direction.y * this->acceleration;
    }

    //LEFT
    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::LEFT] )))
    {
        this->direction.x = -1.f;
        this->direction.y = 0.f;

        if (this->currentVelocity.x > -this->maxVelocity && this->direction.x < 0)
            this->currentVelocity.x += this->direction.x * this->acceleration;
    }

    //RIGHT
    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::RIGHT] )))
    {
        this->direction.x = 1.f;
        this->direction.y = 0.f;

        if (this->currentVelocity.x < this->maxVelocity && this->direction.x > 0)
            this->currentVelocity.x += this->direction.x * this->acceleration;
    }

    //Drag force (para el player al soltar)
    if (this->currentVelocity.x > 0)
    {
        this->currentVelocity.x -= this->stabilizerForce;

        if (this->currentVelocity.x < 0)
            this->currentVelocity.x = 0;
    }

    else if (this->currentVelocity.x < 0)
    {
        this->currentVelocity.x += this->stabilizerForce;

        if (this->currentVelocity.x > 0)
            this->currentVelocity.x = 0;
    }
    //------------- Y ------------
    if (this->currentVelocity.y > 0)
    {
        this->currentVelocity.y -= this->stabilizerForce;

        if (this->currentVelocity.y < 0)
            this->currentVelocity.y = 0;
    }

    else if (this->currentVelocity.y < 0)
    {
        this->currentVelocity.y += this->stabilizerForce;

        if (this->currentVelocity.y > 0)
            this->currentVelocity.y = 0;
    }

    // Movimiento final
    this->sprite.move(this->currentVelocity.x, this->currentVelocity.y);

    //Update positions
    this->playerCenter.x = this->sprite.getPosition().x +
                           this->sprite.getGlobalBounds().width / 2;
    this->playerCenter.y = this->sprite.getPosition().y +
                           this->sprite.getGlobalBounds().height / 2;
}

void Player::Combat()
{   // Si ha pasado el tiempo adecuado crear proyectil y reiniciar contador de tiempo
    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::SHOOT] ))
        && this->shootTimer >= this->shootTimerMax)
    {
        this->bullets.push_back(
                Bullet(bulletTexture, // textura
                       this->playerCenter, // posición
                       Vector2f(1.f, 0.f), 2.f, // dirección, velocidad inicial
                       50.f, 1.f)); // velocidad máxima, aceleración

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




    this->Movement();
    this->UpdateAccesories(); // Actualiza cañon
    this->Combat();
}

void Player::Draw(RenderTarget &target)
{
    for (size_t i = 0; i < this->bullets.size(); i++)
    {
        this->bullets[i].Draw(target);
    }

    target.draw(this->mainGunSprite); // Dibuja cañon en la nave

    target.draw(this->sprite);
}