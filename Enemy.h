#pragma once

#include "Bullet.h"

class Enemy
{
private:
    Texture *texture;
    Sprite sprite;
    Vector2u windowBounds;
    Vector2f direction;

    int type;
    int hp;
    int hpMax;
    int damageMin;
    int damageMax;
public:
    Enemy(Texture *texture, Vector2u windowBounds,
        Vector2f position, Vector2f direction,
        Vector2f scale, int type,
        int hpMax, int damageMax, int damageMin);

    virtual ~Enemy();

    //Accessors
    // Retorna da�o
    inline const int getDamage()const { return rand() % this->damageMax + this->damageMin; }
    inline const int getHP()const { return this->hp; } // Energia, HP = hit points o health points
    inline const int getHPMax()const { return this->hpMax; } // Energia m�xima
    inline const bool isDead()const { return this->hp <= 0; } // Si Energia <= 0 est� muerto
    inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
    inline Vector2f getPosition()const { return this->sprite.getPosition(); }

    // Funciones

    void takeDamage(int damage); // Crea da�o
    void Update();
    void Draw(RenderTarget &target);
};