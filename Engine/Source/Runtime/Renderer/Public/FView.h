#pragma once

// Minimal math placeholders
struct FMatrix { float M[16] = {0}; };

class FView {
public:
    FMatrix ViewMatrix;
    FMatrix ProjectionMatrix;
};


