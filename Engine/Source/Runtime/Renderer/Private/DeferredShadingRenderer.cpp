#include "../Public/FDeferredShadingPass.h"
#include "Passes/FDepthPass.h"
#include "Passes/FBasePass.h"
#include "Passes/FLightingPass.h"
#include "Passes/FPostProcessPass.h"
#include "../../RHI/Public/RHICommandList.h"
#include "../Public/FView.h"
#include "../Public/SceneInterface.h"

// Orchestrates deferred shading sequence
class FDeferredShadingRenderer {
public:
    void Render(IRHICommandList& RHICmdList, FScene& Scene, const FView& View) {
        FDepthPass Depth; Depth.Execute(RHICmdList, View);
        FBasePass Base;  Base.Execute(RHICmdList, View);
        FLightingPass Light; Light.Execute(RHICmdList, View);
        FPostProcessPass Post; Post.Execute(RHICmdList, View);
    }
};


