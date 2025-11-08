#pragma once

class FTexture;

// Dynamic RHI factory/entry points
class FDynamicRHI {
public:
    virtual ~FDynamicRHI() = default;

    virtual FTexture* RHICreateTexture2D(int width, int height, int format) = 0;
    virtual void RHICreatePipelineState() = 0;
};


