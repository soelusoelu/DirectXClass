#pragma once

#include "Component.h"
#include "../UI/Sprite.h"
#include "../Utility/Math.h"
#include <memory>

class Actor;
class Sprite;

class SpriteComponent : public Component {
public:
    SpriteComponent(Actor* owner, const char* fileName, float z);
    ~SpriteComponent();
    virtual void update() override;
    std::shared_ptr<Sprite> getSprite() const;

    //�`��D�揇��(0�`1�A0����O)
    void setPrimary(float z);
    float getDepth() const;
    //�F���A���Ԃ�0�`1
    void setColor(const Vector3& color);
    void setColor(float r, float g, float b);
    //�s�����x(0�`1�A1���s����, 0������)
    void setAlpha(float alpha);
    Vector4 getColor() const;
    //�؂���͈�(left, top, right, bottom, 0�`1)
    void setUV(float l, float t, float r, float b);
    Vector4 getUV() const;
    //��]�s�{�b�g�ʒu
    void setPivot(const Vector2& pivot);
    Vector2 getPivot() const;
    //�e�N�X�`���T�C�Y�̎擾
    Vector2 getTextureSize() const;
    //�e�N�X�`���̌��݂̃T�C�Y���擾
    Vector2 getCurrentTextureSize() const;
    //�X�N���[���\����̃T�C�Y�̎擾
    Vector2 getScreenTextureSize() const;
    //��ԊǗ�
    SpriteState getState() const;
    //World�s��
    void setWorld(const Matrix4& world);
    Matrix4 getWorld() const;
    //�e�N�X�`��
    void setTexture(const char* fileName);
    std::shared_ptr<Texture> texture() const;
    //�V�F�[�_�[�̎擾
    std::shared_ptr<Shader> shader() const;
    //�t�@�C�����̎擾
    const char* fileName() const;

private:
    std::shared_ptr<Sprite> mSprite;
};
