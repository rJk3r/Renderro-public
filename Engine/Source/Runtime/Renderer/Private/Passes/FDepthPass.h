#pragma once

class IRHICommandList;
class FScene;
class FDeferredShadingPass;

class FDepthPass : public FDeferredShadingPass {
public:
    void Execute(IRHICommandList& RHICmdList, const class FView& View) override;
    void RenderDepth(IRHICommandList& RHICmdList);
};


