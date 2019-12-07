#include "Physics.h"
#include "../Actor/Actor.h"
#include "../Actor/ComponentManagementOfActor.h"
#include "../Actor/ActorManager.h"
#include "../Component/CircleCollisionComponent.h"
#include "../Component/Collider.h"
#include "../Utility/Collision.h"
#include <algorithm>

Physics::Physics() = default;
Physics::~Physics() = default;

void Physics::sweepAndPrune() {
    //mCenter.x - mCenter.mRadiusが小さい順にソート
    std::sort(mColliders.begin(), mColliders.end(), [](Collider* a, Collider* b) {
        auto circleA = dynamic_cast<CircleCollisionComponent*>(a);
        auto circleB = dynamic_cast<CircleCollisionComponent*>(b);
        return circleA->getCircle()->mCenter.x - circleA->getCircle()->mRadius < circleB->getCircle()->mCenter.x - circleB->getCircle()->mRadius;
    });

    for (size_t i = 0; i < mColliders.size(); i++) {
        auto a = dynamic_cast<CircleCollisionComponent*>(mColliders[i]);
        if (!a->getEnable()) {
            continue;
        }
        auto ac = a->getCircle();
        //mCircles[i]の中心+半径を取得
        float max = ac->mCenter.x + ac->mRadius;
        for (size_t j = i + 1; j < mColliders.size(); j++) {
            auto b = dynamic_cast<CircleCollisionComponent*>(mColliders[j]);
            if (!b->getEnable()) {
                continue;
            }
            auto bc = b->getCircle();
            //もしmCircles[j]の中心-半径が、mCircles[i]の中心+半径を超えていたら、
            //mCircles[i]と交差する可能性があるボックスは存在しない
            if (bc->mCenter.x - bc->mRadius > max) {
                break;
            } else if (intersect(*ac, *bc)) {
                a->addHitCollider(b);
                b->addHitCollider(a);
            }
        }
    }
}

void Physics::add(Collider* circle) {
    mColliders.emplace_back(circle);
}

void Physics::remove(Collider* circle) {
    auto itr = std::find(mColliders.begin(), mColliders.end(), circle);
    if (itr != mColliders.end()) {
        std::iter_swap(itr, mColliders.end() - 1);
        mColliders.pop_back();
        return;
    }
}

void Physics::clear() {
    mColliders.clear();
}

std::vector<Collider*> Physics::mColliders;
