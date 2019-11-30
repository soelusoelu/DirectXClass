﻿#include "Texture.h"
#include "Sprite.h"
#include "../Device/Renderer.h"
#include "../Shader/Shader.h"
#include "../System/Buffer.h"
#include "../System/BufferDesc.h"
#include "../System/Direct3D11.h"
#include "../System/Game.h"
#include "../System/InputElement.h"
#include "../System/SubResourceDesc.h"
#include "../System/VertexStreamDesc.h"

Texture::Texture() :
    mTexture(nullptr),
    mVertexLayout(nullptr) {
}

Texture::~Texture() {
    SAFE_RELEASE(mTexture);
}

void Texture::init(const std::string& fileName) {
    if (FAILED(createTexture(fileName))) {
        MessageBox(0, L"テクスチャ作成失敗", NULL, MB_OK);
    }
}

void Texture::createVertexLayout(ID3D10Blob* compiledShader) {
    static constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    mVertexLayout = Renderer::createInputLayout(layout, numElements, compiledShader);
}

void Texture::end() {
    SAFE_RELEASE(mSampleLinear);
}

void Texture::drawAll(std::list<std::shared_ptr<Sprite>>* sprites) {
    if (!mVertexBuffer || sprites->size() < 1) {
        return;
    }
    //プロジェクション
    Matrix4 proj = Matrix4::identity;
    //原点をスクリーン左上にするために平行移動
    proj.mat[3][0] = -1.f;
    proj.mat[3][1] = 1.f;
    //ピクセル単位で扱うために
    proj.mat[0][0] = 2.f / Game::WINDOW_WIDTH;
    proj.mat[1][1] = -2.f / Game::WINDOW_HEIGHT;

    //プリミティブ・トポロジーをセット
    Renderer::setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_STRIP);
    //バーテックスバッファーをセット
    VertexStreamDesc stream;
    stream.buffer = mVertexBuffer;
    stream.offset = 0;
    stream.stride = sizeof(TextureVertex);
    Renderer::setVertexBuffer(stream);
    //サンプラーのセット
    Direct3D11::mDeviceContext->PSSetSamplers(0, 1, &mSampleLinear);

    for (auto itr = sprites->begin(); itr != sprites->end(); ++itr) {
        auto sprite = (*itr);
        if (sprite->getState() == SpriteState::Dead) {
            continue;
        }
        sprite->onceToDead();

        auto shader = sprite->shader();
        //自身を使用するシェーダーとして登録
        shader->setVSShader();
        shader->setPSShader();
        //コンスタントバッファーを使うシェーダーの登録
        shader->setVSConstantBuffers();
        shader->setPSConstantBuffers();
        //頂点レイアウトをセット
        Renderer::setInputLayout(sprite->texture()->mVertexLayout);

        //シェーダーのコンスタントバッファーに各種データを渡す
        D3D11_MAPPED_SUBRESOURCE pData;
        if (SUCCEEDED(Direct3D11::mDeviceContext->Map(shader->getConstantBuffer()->buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
            TextureShaderConstantBuffer cb;
            //ワールド、カメラ、射影行列を渡す
            cb.mWorld = sprite->getWorld();
            cb.mWorld.transpose();
            cb.mProjection = proj;
            cb.mProjection.transpose();
            cb.mColor = sprite->getColor();
            cb.mUV = sprite->getUV();
            memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
            Direct3D11::mDeviceContext->Unmap(shader->getConstantBuffer()->buffer(), 0);
        }
        //テクスチャーをシェーダーに渡す
        shader->setPSTextures(sprite->texture());
        //プリミティブをレンダリング
        Renderer::draw(4);
    }
}

ID3D11ShaderResourceView* Texture::texture() const {
    return mTexture;
}

HRESULT Texture::createTexture(const std::string& fileName) {
    if (!mVertexBuffer) {
        //バーテックスバッファー作成
        TextureVertex vertices[] = {
            Vector3(0.f, 0.f, 0.f), Vector2(0.f, 0.f), //左上
            Vector3(1.f, 0.f, 0.f), Vector2(1.f, 0.f), //右上
            Vector3(0.f, 1.f, 0.f), Vector2(0.f, 1.f), //左下
            Vector3(1.f, 1.f, 0.f), Vector2(1.f, 1.f), //右下
        };

        BufferDesc bd;
        bd.size = sizeof(TextureVertex) * 4;
        bd.usage = BufferUsage::BUFFER_USAGE_DEFAULT;
        bd.type = BufferType::BUFFER_TYPE_VERTEX;
        bd.cpuAccessFlags = CPUAccessFlag::CPU_ACCESS_NONE;
        bd.miscFlags = 0;
        bd.structureByteStride = 0;

        SubResourceDesc sub;
        sub.data = vertices;
        mVertexBuffer = Renderer::createBuffer(bd, &sub);

        //テクスチャー用サンプラー作成
        D3D11_SAMPLER_DESC SamDesc;
        ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
        SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        Direct3D11::mDevice->CreateSamplerState(&SamDesc, &mSampleLinear);
    }
    //テクスチャー作成
    setTextureDirectory();
    if (FAILED(D3DX11CreateShaderResourceViewFromFileA(Direct3D11::mDevice, fileName.c_str(), NULL, NULL, &mTexture, NULL))) {
        return E_FAIL;
    }

    return S_OK;
}

ID3D11SamplerState* Texture::mSampleLinear = nullptr;
std::shared_ptr<Buffer> Texture::mVertexBuffer = nullptr;
