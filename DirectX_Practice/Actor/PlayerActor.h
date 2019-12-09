#pragma once

#include "Actor.h"

class AnimationComponent;
class PlayerMoveComponent;
class SpriteComponent;

class PlayerActor : public Actor {
public:
    PlayerActor(const char* tag = "Player");
    ~PlayerActor();
    virtual void updateActor() override;
    virtual void drawActor() const override;

private:
    AnimationComponent* mAnimation;
    PlayerMoveComponent* mPlayerMove;
    SpriteComponent* mSprite;
};

