#include "Enemy.h"

enum eTypes { MOVELEFT = 0, FOLLOW, FOLLOWFAST, FOLLOWSHOOT, FOLLOWFASTSHOOT };

Enemy::Enemy(Texture *texture, Vector2u windowBounds,
             Vector2f position, Vector2f direction,
             Vector2f scale, int type,
             int hpMax, int damageMax, int damageMin)
{
    this->texture = texture;
    this->sprite.setTexture(*this->texture);
    this->sprite.setScale(scale);
    this->windowBouns = windowBounds;

    this->direction = direction;
    // Posición de enemigo en la derecha y aleatoriamente arriba y abajo
    this->sprite.setPosition(windowBounds.x,
                            (rand() % windowBounds.y) - this->sprite.getGlobalBounds().height);

    this->type = type;

    this->hpMax = hpMax;
    this->hp = this->hpMax;

    this->damageMax = damageMax;
    this->damageMin = damageMin;
}

Enemy::~Enemy()
{
}

void Enemy::takeDamage(int damage) // El enemigo recibe daño
{
    this->hp -= damage; // Baja la energia

    if (this->hp <= 0)
        this->hp = 0;
}

void Enemy::Update()
{
    switch (this->type)
    {
        case 0:
            this->sprite.move(this->direction.x * 10.f,
                              this->direction.y * 10.f);

            break;

        default:

            break;
    }
}

void Enemy::Draw(RenderTarget &target)
{
    target.draw(this->sprite);
}