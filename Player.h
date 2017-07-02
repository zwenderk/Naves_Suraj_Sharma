#pragma once

#include "Bullet.h"

class Player
{
private:
    unsigned playerNr;

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
           int UP = 22, int DOWN = 18, // números según el enum key en Keyboard.hpp
           int LEFT = 0, int RIGHT = 3,
           int SHOOT = 57);
    virtual ~Player();

    void Movement();
    void Update();
    void Draw(RenderTarget &target);

    //Statics
    static unsigned players;

};

