#include "FBasePass.h"
#include "../../Public/SceneInterface.h"
#include "../../../RHI/Public/RHIResources.h"

void FBasePass::Execute(IRHICommandList&, const FView&) {}

void FBasePass::RenderGBuffer(FScene&) {}

void FBasePass::GBufferTargets(FTexture&) {}


