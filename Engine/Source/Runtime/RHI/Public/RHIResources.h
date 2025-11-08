#pragma once

enum class ETextureType {
    Unknown = 0,
    Color2D,
    DepthStencil
};

// Base RHI resource for textures
class FTexture {
public:
    explicit FTexture(ETextureType inType = ETextureType::Unknown) : Type(inType) {}
    virtual ~FTexture() = default;

    virtual void Bind(int slot) {}

    ETextureType Type;
};


