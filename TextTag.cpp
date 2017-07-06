#include "TextTag.h"

TextTag::TextTag(Font *font, std::string text, Color color,
                 Vector2f position, unsigned int size)
{
    this->dtMultiplier = 62.5;

    this->text.setFont(*font);
    this->text.setCharacterSize(size);
    this->text.setPosition(position);
    this->text.setString(text);
    //this->text.setColor(color); // Obsoleto
    this->text.setOutlineColor(color);

    this->timerMax = 10.f;
    this->timer = this->timerMax;

}

TextTag::~TextTag()
{

}

void TextTag::Update(const float &dt)
{
    //Update timer
    if (this->timer > 0.f)
        this->timer += 1.f * dt * this->dtMultiplier;
}

void TextTag::Draw(RenderTarget &target)
{
    target.draw(this->text);
}
