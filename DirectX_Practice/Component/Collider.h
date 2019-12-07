#pragma once

#include "Component.h"
#include <list>

class Actor;

class Collider : public Component {
public:
    virtual void start() override;
    virtual void startCollider() = 0;
    virtual void update() override;
    virtual void updateCollider() = 0;
    void enabled();
    void disabled();
    bool getEnable() const;
    void addHitCollider(Collider* hit);
    std::list<Collider*> onCollisionEnter();
    std::list<Collider*> onCollisionStay();
    std::list<Collider*> onCollisionExit();

protected:
    Collider(Actor* owner);
    virtual ~Collider();

private:
    bool mEnable;
    std::list<Collider*> mPreviousCollider;
    std::list<Collider*> mCurrentCollider;
};

