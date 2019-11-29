#include "Shader.h"
#include "../System/Game.h"
#include "../System/Direct3D11.h"
#include "../Device/Renderer.h"
#include "../UI/Texture.h"
#include "../System/InputElement.h"
#include "../System/InputElementDesc.h"

Shader::Shader() {
    ZeroMemory(this, sizeof(Shader));
}

Shader::~Shader() {
    SAFE_RELEASE(mCompiledShader);
    SAFE_RELEASE(mVertexShader);
    SAFE_RELEASE(mPixelShader);
    SAFE_RELEASE(mConstantBuffer);
    SAFE_RELEASE(mBlendState);
}

ID3D11VertexShader* Shader::createVertexShader(const char* fileName, const char* funcName) {
    setShaderDirectory();
    //ブロブからバーテックスシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName, nullptr, nullptr, funcName, "vs_5_0", 0, 0, nullptr, &mCompiledShader, nullptr, nullptr))) {
        MessageBox(0, L"hlsl読み込み失敗", nullptr, MB_OK);
        return nullptr;
    }
    if (FAILED(Direct3D11::mDevice->CreateVertexShader(mCompiledShader->GetBufferPointer(), mCompiledShader->GetBufferSize(), nullptr, &mVertexShader))) {
        SAFE_RELEASE(mCompiledShader);
        MessageBox(0, L"バーテックスシェーダー作成失敗", nullptr, MB_OK);
        return nullptr;
    }

    return mVertexShader;
}

ID3D11PixelShader* Shader::createPixelShader(const char* fileName, const char* funcName) {
    ID3D10Blob* compiledShader;
    setShaderDirectory();
    //ブロブからピクセルシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName, nullptr, nullptr, funcName, "ps_5_0", 0, 0, nullptr, &compiledShader, nullptr, nullptr))) {
        MessageBox(0, L"hlsl読み込み失敗", nullptr, MB_OK);
        return nullptr;
    }
    if (FAILED(Direct3D11::mDevice->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &mPixelShader))) {
        SAFE_RELEASE(compiledShader);
        MessageBox(0, L"ピクセルシェーダー作成失敗", nullptr, MB_OK);
        return nullptr;
    }
    SAFE_RELEASE(compiledShader);

    //コンスタントバッファー作成
    D3D11_BUFFER_DESC cb;
    cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cb.ByteWidth = sizeof(TextureShaderConstantBuffer);
    cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cb.MiscFlags = 0;
    cb.StructureByteStride = 0;
    cb.Usage = D3D11_USAGE_DYNAMIC;

    if (FAILED(Direct3D11::mDevice->CreateBuffer(&cb, NULL, &mConstantBuffer))) {
        return mPixelShader;
    }

    //アルファブレンド用ブレンドステート作成
    D3D11_BLEND_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
    bd.IndependentBlendEnable = false;
    bd.AlphaToCoverageEnable = false;
    bd.RenderTarget[0].BlendEnable = true;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    if (FAILED(Direct3D11::mDevice->CreateBlendState(&bd, &mBlendState))) {
        return mPixelShader;
    }

    UINT mask = 0xffffffff;
    Direct3D11::mDeviceContext->OMSetBlendState(mBlendState, NULL, mask);

    return mPixelShader;
}

ID3D11VertexShader* Shader::getVertexShader() const {
    return mVertexShader;
}

ID3D11PixelShader* Shader::getPixelShader() const {
    return mPixelShader;
}

ID3D10Blob* Shader::getCompiledShader() const {
    return mCompiledShader;
}
