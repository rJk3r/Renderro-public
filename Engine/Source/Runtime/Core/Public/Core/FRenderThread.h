#pragma once

// Core Render thread responsible for executing render commands
class FRenderThread {
public:
    void EnqueueRenderCommand();
    void WaitForFence();
};


