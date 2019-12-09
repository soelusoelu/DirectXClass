#include "GamePlay.h"
#include "../Actor/ActorManager.h"
#include "../Actor/PlayerActor.h"
#include "../Device/Physics.h"
#include "../Device/Renderer.h"
#include "../System/Game.h"
#include "../UI/Pause.h"
#include "../UI/Sprite.h"
#include "../UI/SpriteManager.h"
#include "../UI/UIManager.h"
#include "../Utility/Input.h"
#include "../Utility/Math.h"

GamePlay::GamePlay() :
    SceneBase(),
    mActorManager(std::make_shared<ActorManager>()),
    mState(GameState::PLAY) {
    new PlayerActor();
}

GamePlay::~GamePlay() {
    mActorManager->clear();
    Renderer::clear();
    Physics::clear();
}

void GamePlay::updateScene() {
    if (mState == GameState::PLAY) {
        mActorManager->update();

        Physics::sweepAndPrune();

        //if (Input::getKeyDown(KeyCode::Alpha1)) {
        //    mUIManager->add(new Pause(shared_from_this()));
        //}
    }
}

void GamePlay::drawScene() const {
    mActorManager->draw();
}

GameState GamePlay::getState() const {
    return mState;
}

void GamePlay::setState(GameState state) {
    mState = state;
}
