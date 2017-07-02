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

    Texture *texture; // Nave
    Sprite sprite;
    RectangleShape hitBox;

    //Accesorios
    Texture *mainGunTexture; // Ca��n
    Sprite mainGunSprite;


    Texture *bulletTexture;
    std::vector<Bullet> bullets;

    int controls[5];
    Vector2f currentVelocity;
    float maxVelocity;
    float acceleration;
    Vector2f direction;
    float stabilizerForce;

    int level;
    int exp;
    int expNext;

    int hp;
    int hpMax;

    int damage;
    int damageMax;

    int score;

public:
    Player(Texture *texture, Texture *bulletTexture, Texture *mainGunTexture,
           int UP = 22, int DOWN = 18, // n�meros seg�n el enum key en Keyboard.hpp
           int LEFT = 0, int RIGHT = 3,
           int SHOOT = 57);
    virtual ~Player();

    //Accesors
    inline std::vector<Bullet>& getBullets() { return this->bullets;}
    inline const Vector2f& getPosition()const { return this->sprite.getPosition();}
    inline const String getHpAsString()const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax);}

    //Funciones
    void UpdateAccesories();
    void Combat();
    void Movement();
    void Update(Vector2u windowBounds);
    void Draw(RenderTarget &target);

    //Statics
    static unsigned players;

};

