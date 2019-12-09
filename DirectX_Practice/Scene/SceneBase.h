#pragma once

#include <memory>

enum class Scene {
    TITLE,
    SOUSA,
    GAME_PLAY,

    NONE
};

class UIManager;
class SpriteManager;

class SceneBase {
public:
    SceneBase();
    virtual ~SceneBase();
    void update();
    virtual void updateScene() = 0;
    void draw();
    virtual void drawScene() const = 0;
    void next(Scene next);
    Scene getNextScene() const;

protected:
    std::unique_ptr<UIManager> mUIManager;
    std::unique_ptr<SpriteManager> mSpriteManager;

private:
    Scene mNext;
};
