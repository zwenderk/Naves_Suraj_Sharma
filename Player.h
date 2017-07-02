#pragma once

#include "Bullet.h"

class Player
{
private:
    unsigned playerNr;

    Vector2f playerCenter;

    int shootTimer;
    int shootTimerMax;
    int damageTimer;
    int damageTimerMax;

    Texture *texture;
    Sprite sprite;
    RectangleShape hitBox;

    Texture *bulletTexture;
    std::vector<Bullet> bullets;

    int controls[5];

    int level;
    int exp;
    int expNext;

    int hp;
    int hpMax;

    int damage;
    int damageMax;

    int score;

public:
    Player(Texture *texture, Texture *bulletTexture,
           int UP = 22, int DOWN = 18, // n�meros seg�n el enum key en Keyboard.hpp
           int LEFT = 0, int RIGHT = 3,
           int SHOOT = 57);
    virtual ~Player();

    //Accesors
    inline std::vector<Bullet>& getBullets() { return this->bullets;}
    inline const Vector2f& getPosition()const { return this->sprite.getPosition();}
    inline const String getHpAsString()const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax);}

    //Functions
    void Combat();
    void Movement();
    void Update(Vector2u windowBounds);
    void Draw(RenderTarget &target);

    //Statics
    static unsigned players;

};

