#pragma once

#include "../System/DirectXIncLib.h"
#include <string>

enum class ShaderType {
    SHADER_TYPE_VERTEX,
    SHADER_TYPE_PIXEL
};

class Shader {
public:
    Shader();
    ~Shader();

    ID3D11VertexShader* createVertexShader(const char* fileName, const char* funcName);
    ID3D11PixelShader* createPixelShader(const char* fileName, const char* funcName);

    ID3D11VertexShader* getVertexShader() const;
    ID3D11PixelShader* getPixelShader() const;
    ID3D10Blob* getCompiledShader() const;
    ID3D11Buffer* mConstantBuffer;

private:
    ID3D10Blob* mCompiledShader;
    ID3D11VertexShader* mVertexShader;
    ID3D11PixelShader* mPixelShader;
    ID3D11BlendState* mBlendState; //アルファブレンディング
};

