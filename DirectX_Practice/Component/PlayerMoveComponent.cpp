#include "PlayerMoveComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/ComponentManagementOfActor.h"
#include "../Actor/Transform2D.h"
#include "../Component/AnimationComponent.h"
#include "../Component/SpriteComponent.h"
#include "../System/Game.h"
#include "../Utility/Input.h"

PlayerMoveComponent::PlayerMoveComponent(Actor* owner, int updateOrder) :
    Component(owner, updateOrder),
    mAnim(nullptr),
    mSprite(nullptr),
    MOVE_SPEED(4.f),
    FALL_SPEED(9.8f) {
}

PlayerMoveComponent::~PlayerMoveComponent() = default;

void PlayerMoveComponent::start() {
    mSprite = mOwner->getComponentManager()->getComponent<SpriteComponent>();
    mSprite->setUV(0.f, 0.f, 0.25f, 0.5f);

    mOwner->getTransform()->setScale(0.75f);
    auto s = mSprite->getScreenTextureSize().y;
    mOwner->getTransform()->setPosition(Vector2(Game::WINDOW_WIDTH / 2.f, Game::WINDOW_HEIGHT - mSprite->getScreenTextureSize().y));

    mAnim = mOwner->getComponentManager()->getComponent<AnimationComponent>();
}

void PlayerMoveComponent::update() {
    move();
    fall();
    posClamp();
}

void PlayerMoveComponent::move() {
    int horizontal = Input::horizontal();
    if (!Math::nearZero(horizontal)) {
        mOwner->getTransform()->translate(Vector2(horizontal, 0.f) * MOVE_SPEED);

        auto dir = horizontal < 0 ? Direction::Left : Direction::Right;

        mAnim->set(static_cast<int>(dir));
    }
}

void PlayerMoveComponent::fall() {
    //重力は常にかける
    mOwner->getTransform()->translate(Vector2(0.f, FALL_SPEED));
}

void PlayerMoveComponent::posClamp() {
    auto pos = mOwner->getTransform()->getPosition();
    auto size = mSprite->getScreenTextureSize();
    pos.x = Math::clamp<float>(pos.x, 0.f, Game::WINDOW_WIDTH - size.x);
    pos.y = Math::clamp<float>(pos.y, 0.f, Game::WINDOW_HEIGHT - size.y);

    mOwner->getTransform()->setPosition(pos);
}
