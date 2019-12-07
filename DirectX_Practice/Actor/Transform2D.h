#pragma once

#include "../Utility/Math.h"

class Transform2D {
public:
    Transform2D();
    ~Transform2D();

    //���[���h�s��X�V
    bool computeWorldTransform(const Vector2& texSize, const Vector2& pivot, float z);
    Matrix4 getWorldTransform() const;

    //�s�N�Z���P�ʂňʒu�w��
    void setPosition(const Vector2& pos);
    Vector2 getPosition() const;
    void translate(const Vector2& translation);

    //��]
    void setRotation(float angle);
    Quaternion getRotation() const;
    void rotate(float angle);

    //�{���Ŋg�k
    void setScale(const Vector2& scale);
    void setScale(float scale);
    Vector2 getScale() const;

private:
    Matrix4 mWorldTransform;
    Vector2 mPosition;
    Quaternion mRotation;
    Vector2 mScale;
    bool mIsRecomputeTransform;
};

