#pragma once

#include "BufferDesc.h"
#include "DirectXIncLib.h"

class Buffer {
public:
    Buffer(const BufferDesc& desc, const D3D11_SUBRESOURCE_DATA* data = nullptr);
    virtual ~Buffer();
    //ディスクリプタの取得
    const BufferDesc& desc() const;
    //データの設定
    void setData(const void* data);
    //データの取得
    void getData(void* data) const;
    //バッファの取得
    ID3D11Buffer* buffer() const;

private:
    //自作バッファーデスクリプタを変換
    D3D11_BUFFER_DESC toBufferDesc(const BufferDesc& desc) const;
    //バッファのタイプを変換
    unsigned toTarget(BufferType type) const;
    //バッファの使用方法を変換
    D3D11_USAGE toUsage(BufferUsage usage) const;
    //CPUアクセス権限を変換
    unsigned toCPUAccess(CPUAccessFlag flag) const;

    //コピー禁止
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

private:
    //ディスクリプタ
    BufferDesc mDesc;
    //バッファオブジェクト
    ID3D11Buffer* mBuffer;
    //ターゲット
    //unsigned mTarget;
    //使用方法
    //D3D11_USAGE mUsage;
};

