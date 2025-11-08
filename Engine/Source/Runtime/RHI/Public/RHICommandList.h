#pragma once

class FTexture; // forward declare

// Rendering Hardware Interface command list abstraction
class IRHICommandList {
public:
    virtual ~IRHICommandList() = default;

    virtual void DrawIndexedPrimitive(int indexCount, int startIndex) = 0;
    virtual void SetRenderTarget(FTexture* colorTarget) = 0;
    virtual void DispatchComputeShader(int threadGroupX, int threadGroupY, int threadGroupZ) = 0;
};


