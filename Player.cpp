#include "Player.h"

unsigned Player::players = 0;

enum controls {UP = 0, DOWN, LEFT, RIGHT, SHOOT};
enum weapons {LASER = 0, MISSILE01, MISSILE02}; // Proyectiles

Player::Player(
        std::vector<Texture> &textures,
        int UP, int DOWN,
        int LEFT, int RIGHT,
        int SHOOT)
        :level(1), exp(0),
         hp(10), hpMax(10),
         statPoints(0), cooling(0),
         plating(0), wiring(0), power(0),
         damage(1), damageMax(2),
         score(0)
{
    // Dt (delta time)
    this->dtMultiplier = 62.5f;

    //Stats
    this->expNext = 20 + static_cast<int>(
            (50 / 3)
            * ((pow(level, 3) - 6
                    * pow(level, 2)) + 17
                    * level -12)
           );

    // Actualiza posiciones
    this->playerCenter.x = this->sprite.getPosition().x +
                           this->sprite.getGlobalBounds().width / 2;
    this->playerCenter.y = this->sprite.getPosition().y +
                           this->sprite.getGlobalBounds().height / 2;

    // Texturas y sprites
    this->sprite.setTexture(textures[0]); // Textura de la nave
    this->sprite.setScale(0.13f, 0.13f);

    this->laserTexture = &textures[1]; // Textura del proyectil laser
    this->missile01Texture = &textures[2]; // Textura del proyectil 01

    this->mainGunSprite.setTexture(textures[3]); // Textura del ca�on
    this->mainGunSprite.setOrigin( // Situa el ca�on en el centro de la nave
            this->mainGunSprite.getGlobalBounds().width / 2,
            this->mainGunSprite.getGlobalBounds().height / 2);
    this->mainGunSprite.rotate(90); // Rota el ca�on 90 grados

    this->mainGunSprite.setPosition(
            this->playerCenter.x + 20.f, // Situar el ca�on en el centro de la nave + 20 en x
            this->playerCenter.y);

    // Temporizadores
    this->shootTimerMax = 25.f;
    this->shootTimer = this->shootTimerMax;
    this->damageTimer = 10.f;
    this->damageTimer = this->damageTimer;

    // Controles
    this->controls[controls::UP] = UP;
    this->controls[controls::DOWN] = DOWN;
    this->controls[controls::LEFT] = LEFT;
    this->controls[controls::RIGHT] = RIGHT;
    this->controls[controls::SHOOT] = SHOOT;

    // Velocidad y aceleraci�n
    this->maxVelocity = 25.f;
    this->acceleration = 0.8f;
    this->stabilizerForce = 0.4f;

    // Armas
    this->currentWeapon = LASER;

    // Actualizaciones
    this->mainGunLevel = 0;
    this->dualMissiles01 = false;
    this->dualMissiles02 = false;

    // A�ade n�mero de players, playerNr es el n�mero de jugador
    this->playerNr = Player::players;
    Player::players++; // Incrementa variable player static
}

Player::~Player()
{
}

int Player::getDamage()const
{
    int damage = 0;

    switch (this->currentWeapon)
    {
        case LASER:
            damage = rand() % this->damageMax + this->damage;
            break;
        case MISSILE01:
            damage = rand() % this->damageMax + this->damage;
            damage *= 2;

            break;
        case MISSILE02:
            damage = rand() % this->damageMax + this->damage;
            damage *= 4;
            break;
        default:

            damage = rand() % this->damageMax + this->damage;

            break;
    }

    return damage;
}

void Player::UpdateLeveling()
{
    if (this->exp >= this->expNext)
    {
        this->level++;
        this->statPoints++;
        this->exp -= this->expNext;
        this->expNext = static_cast<int>((50 / 3)
                        * ((pow(level, 3) - 6
                        * pow(level, 2)) + 17
                        * level - 12)
        );
    }
}

void Player::UpdateAccesories(const float &dt)
{
    // Establece la posici�n del ca�on para que siga a la nave
    this->mainGunSprite.setPosition(
            this->mainGunSprite.getPosition().x, // El ca�on se mueve en la nave
            this->playerCenter.y); // Situa ca�on en la nave


    // Anima el ca�on principal y lo corrige despues de disparar
    if (this->mainGunSprite.getPosition().x < this->playerCenter.x + 20.f)
    {
        this->mainGunSprite.move(2.f * dt * this->dtMultiplier
           + this->currentVelocity.x * dt * this->dtMultiplier, 0.f);
    }

    if (this->mainGunSprite.getPosition().x > this->playerCenter.x + 20.f)
    {
        this->mainGunSprite.setPosition(
                this->playerCenter.x + 20.f, // Situar el ca�on en el centro de la nave + 20 en x
                this->playerCenter.y);
    }
}

void Player::Movement(const float &dt)
{
    //UP
    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::UP] )))
    {
        this->direction.x = 0.f;
        this->direction.y = -1.f;

        if (this->currentVelocity.y > -this->maxVelocity && this->direction.y < 0)
            this->currentVelocity.y += this->direction.y * this->acceleration
            * dt * this->dtMultiplier;
    }

    //DOWN
    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::DOWN] )))
    {
        this->direction.x = 0.f;
        this->direction.y = 1.f;

        if (this->currentVelocity.y < this->maxVelocity && this->direction.y > 0)
            this->currentVelocity.y += this->direction.y * this->acceleration
            * dt * this->dtMultiplier;
    }

    //LEFT
    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::LEFT] )))
    {
        this->direction.x = -1.f;
        this->direction.y = 0.f;

        if (this->currentVelocity.x > -this->maxVelocity && this->direction.x < 0)
            this->currentVelocity.x += this->direction.x * this->acceleration
            * dt * this->dtMultiplier;
    }

    //RIGHT
    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::RIGHT] )))
    {
        this->direction.x = 1.f;
        this->direction.y = 0.f;

        if (this->currentVelocity.x < this->maxVelocity && this->direction.x > 0)
            this->currentVelocity.x += this->direction.x * this->acceleration
            * dt * this->dtMultiplier;
    }

    //Drag force (para el player al soltar)
    if (this->currentVelocity.x > 0)
    {
        this->currentVelocity.x -= this->stabilizerForce
            * dt * this->dtMultiplier;

        if (this->currentVelocity.x < 0)
            this->currentVelocity.x = 0;
    }

    else if (this->currentVelocity.x < 0)
    {
        this->currentVelocity.x += this->stabilizerForce
            * dt * this->dtMultiplier;

        if (this->currentVelocity.x > 0)
            this->currentVelocity.x = 0;
    }
    //------------- Y ------------
    if (this->currentVelocity.y > 0)
    {
        this->currentVelocity.y -= this->stabilizerForce
            * dt * this->dtMultiplier;

        if (this->currentVelocity.y < 0)
            this->currentVelocity.y = 0;
    }

    else if (this->currentVelocity.y < 0)
    {
        this->currentVelocity.y += this->stabilizerForce
            * dt * this->dtMultiplier;

        if (this->currentVelocity.y > 0)
            this->currentVelocity.y = 0;
    }

    // Movimiento final
    this->sprite.move(this->currentVelocity.x * dt * this->dtMultiplier,
                      this->currentVelocity.y * dt * this->dtMultiplier);

    // Actualiza posiciones
    this->playerCenter.x = this->sprite.getPosition().x +
                           this->sprite.getGlobalBounds().width / 2;
    this->playerCenter.y = this->sprite.getPosition().y +
                           this->sprite.getGlobalBounds().height / 2;
}

void Player::Combat(const float &dt)
{   // Si se ha presionado tecla de disparo y ha pasado el tiempo adecuado
    // crear proyectil y reiniciar contador de tiempo
    if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::SHOOT] ))
        && this->shootTimer >= this->shootTimerMax)
    {
        if (this->currentWeapon == LASER)
        {
            // Crea proyectil laser
            if (this->mainGunLevel == 0)
            {
                this->bullets.push_back(
                        Bullet(laserTexture, // textura
                               Vector2f(this->playerCenter.x + 50.f,
                                        this->playerCenter.y), // posici�n inicial del proyectil
                               Vector2f(0.2f, 0.2f), // escala
                               Vector2f(1.f, 0.f),   // direcci�n,
                               20.f, 60.f, 5.f));    // velocidad inicial, velocidad m�xima, aceleraci�n
            }
            else if (this->mainGunLevel == 1)
            {

            }
            else if (this->mainGunLevel == 2)
            {

            }
            // Animar ca�on
            this->mainGunSprite.move(-30.f, 0.f);

        }
        else if (this->currentWeapon == MISSILE01) // Lanza dos proyectiles
        {
            // Crea proyectil superior

            this->bullets.push_back(
                    Bullet(missile01Texture, // textura proyectil 01
                           Vector2f(this->playerCenter.x,
                                    this->playerCenter.y - 25.f), // posici�n inicial del proyectil
                           Vector2f(0.05f, 0.05f), // escala
                           Vector2f(1.f, 0.f),     // direcci�n,
                           2.f, 50.f, 1.f));       // velocidad inicial, velocidad m�xima, aceleraci�n

            if (this->dualMissiles01)
            {
                // Crea proyectil inferior
                this->bullets.push_back(
                        Bullet(missile01Texture, // textura proyectil 01
                               Vector2f(this->playerCenter.x,
                                        this->playerCenter.y + 25.f), // posici�n inicial del proyectil
                               Vector2f(0.05f, 0.05f), // escala
                               Vector2f(1.f, 0.f),     // direcci�n,
                               2.f, 50.f, 1.f));       // velocidad inicial, velocidad m�xima, aceleraci�n
            }
        }
        else if (this->currentWeapon == MISSILE02)
        {
            if (this->dualMissiles02)
            {
            }
        }

        this->shootTimer = 0; //Reset Timer! (poner contador al inicio)
    }
}

void Player::Update(Vector2u windowBounds, const float &dt) // recibe limites en windowBounds
{
    //Update timers
    if (this->shootTimer < this->shootTimerMax)
        this->shootTimer += 1.f * dt *this->dtMultiplier;

    if (this->damageTimer < this->damageTimerMax)
        this->damageTimer += 1.f * dt *this->dtMultiplier;

    this->Movement(dt);
    this->UpdateAccesories(dt); // Actualiza ca�on
    this->Combat(dt);
}

void Player::Draw(RenderTarget &target)
{
    for (size_t i = 0; i < this->bullets.size(); i++)
    {
        this->bullets[i].Draw(target);
    }

    target.draw(this->mainGunSprite); // Dibuja ca�on en la nave

    target.draw(this->sprite);
}