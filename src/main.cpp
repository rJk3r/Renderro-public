/********************************************/
// main.cpp
// Renderro render developed by rJk3r
// first simple render by deatermia

/********************************************/

//  Copyright © 2025 rJk3r. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// #include "Renderro.hpp"
// #include "My_Ri.h"
// #include "math.h"
// #include <eigen3/Eigen/Eigen>
// #include <cmath>
// #include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "DeepRI.h"
#include "Primes.h"
#include "Transforms.h"
#include "ProjectionMatrix.h"
#include "Display.h"

int main() {
    // Image / framebuffer setup
    RtInt width = 400;
    RtInt height = 300;
    RtFloat aspect = (RtFloat)width / (RtFloat)height;

    // Configure render target
    RenderroFormat(width, height, aspect);

    // Begin frame
    RenderroFrameBegin(0);

    // Enter world coordinate space
    RenderroWorldBegin();

    // Setup projection (uses defaults inside the implementation)
    RenderroProjection(NULL);

    // Transform stack: place the cylinder slightly in front of the camera
    RenderroTransformBegin();
    RenderroTranslate(0.0f, 0.0f, 5.0f);

    // Render a cylinder: radius, zmin, zmax, tmax(degrees)
    RenderroCylinder(1.0f, 0.0f, 2.0f, 360.0f);

    RenderroTransformEnd();

    // End frame and show the result (Display handles the window loop)
    RenderroFrameEnd();

    return 0;
}
