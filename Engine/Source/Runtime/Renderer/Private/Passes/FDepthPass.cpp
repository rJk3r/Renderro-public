#include "FDepthPass.h"
#include "../../Public/FDeferredShadingPass.h"
#include "../../../RHI/Public/RHICommandList.h"

void FDepthPass::Execute(IRHICommandList& RHICmdList, const FView&) {
    RenderDepth(RHICmdList);
}

void FDepthPass::RenderDepth(IRHICommandList&) {}


