#pragma once

#include "../../Public/FDeferredShadingPass.h"

class FPostProcessPass : public FDeferredShadingPass {
public:
    void Execute(class IRHICommandList& RHICmdList, const class FView& View) override;

    void ApplyTAA();
    void Tonemap();
};


