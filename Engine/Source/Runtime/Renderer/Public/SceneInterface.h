#pragma once

#include <vector>

class FPrimitiveSceneProxy;

// Scene container
class FScene {
public:
    std::vector<void*> Lights; // placeholder type for lights
    std::vector<FPrimitiveSceneProxy*> PrimitiveSceneProxies;
};


