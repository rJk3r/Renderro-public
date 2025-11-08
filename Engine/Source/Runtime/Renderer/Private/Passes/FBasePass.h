#pragma once

#include "../../Public/FDeferredShadingPass.h"

class FScene;
class FTexture;

class FBasePass : public FDeferredShadingPass {
public:
    void Execute(class IRHICommandList& RHICmdList, const class FView& View) override;

    void RenderGBuffer(FScene& Scene);
    void GBufferTargets(FTexture& Target);
};


