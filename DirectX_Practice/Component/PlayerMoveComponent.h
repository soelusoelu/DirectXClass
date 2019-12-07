#pragma once

#include "Component.h"
#include <memory>

enum class Direction {
    Right = 0,
    Left = 1
};

class Actor;
class AnimationComponent;
class SpriteComponent;

class PlayerMoveComponent : public Component {
public:
    PlayerMoveComponent(Actor* owner, int updateOrder = 10);
    ~PlayerMoveComponent();
    virtual void start() override;
    virtual void update() override;

private:
    void move();
    void fall();
    void posClamp();

private:
    std::shared_ptr<SpriteComponent> mSprite;
    std::shared_ptr<AnimationComponent> mAnim;
    const float MOVE_SPEED;
    const float FALL_SPEED;
};
