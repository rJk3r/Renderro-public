#pragma once

class IRHICommandList;
class FView;

// Abstract base class for deferred shading passes
class FDeferredShadingPass {
public:
    virtual ~FDeferredShadingPass() = default;
    virtual void Execute(IRHICommandList& RHICmdList, const FView& View) = 0;
};


