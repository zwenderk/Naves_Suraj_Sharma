#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <math.h>

using namespace sf;

class TextTag
{
private:
    float dtMultiplier;

    Font *font;
    Text text;

    float timerMax;
    float timer;

public:
    TextTag(Font *font, std::string text, Color color,
            Vector2f position, unsigned int size);
    virtual ~TextTag();

    void Update(const float &dt);
    void Draw(RenderTarget &target);
};
