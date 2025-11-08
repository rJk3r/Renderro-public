#pragma once

// Core Engine thread responsible for game-side updates
class FGameThread {
public:
    void Tick();
    void SendSceneData();
};


