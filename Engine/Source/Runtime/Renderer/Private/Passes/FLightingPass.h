#pragma once

#include "../../Public/FDeferredShadingPass.h"

class FLightingPass : public FDeferredShadingPass {
public:
    void Execute(class IRHICommandList& RHICmdList, const class FView& View) override;

    void ComputeDeferredLighting();
    void CalculateShadows();
    void ResolveLumenGI();
};


