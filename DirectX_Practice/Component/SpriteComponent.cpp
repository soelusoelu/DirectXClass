#include "SpriteComponent.h"
#include "../Actor/Actor.h"
#include "../UI/Sprite.h"
#include "../UI/SpriteManager.h"

SpriteComponent::SpriteComponent(Actor* owner, const char* fileName, float z) :
    Component(owner),
    mSprite(std::make_shared<Sprite>(fileName, z, false)) {
    SpriteManager::add(mSprite);
}

SpriteComponent::~SpriteComponent() {
    Sprite::destroy(mSprite);
}

void SpriteComponent::update() {
}

std::shared_ptr<Sprite> SpriteComponent::getSprite() const {
    return mSprite;
}
