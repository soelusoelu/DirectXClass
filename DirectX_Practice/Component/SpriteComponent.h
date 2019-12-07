#pragma once

#include "Component.h"
#include <memory>

class Actor;
class Sprite;

class SpriteComponent : public Component {
public:
    SpriteComponent(Actor* owner, const char* fileName, float z);
    ~SpriteComponent();
    virtual void update() override;
    std::shared_ptr<Sprite> getSprite() const;

private:
    std::shared_ptr<Sprite> mSprite;
};
