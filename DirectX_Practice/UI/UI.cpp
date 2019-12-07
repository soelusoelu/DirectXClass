#include "UI.h"
#include "Sprite.h"
#include "SpriteManager.h"

UI::UI() :
    mState(UIState::Active) {
}

UI::~UI() {
    for (auto&& sprite : mSprites) {
        Sprite::destroy(sprite);
    }
}

void UI::close() {
    mState = UIState::Closing;
}

void UI::addSprite(std::shared_ptr<Sprite> add) {
    mSprites.emplace_back(add);
    SpriteManager::add(add);
}

UIState UI::getState() const {
    return mState;
}
