#include "fractol.h"
//BLUE
// int trippy_color(int iteration)
// {
//     double t = (double)iteration / 200.0; // Normalize between 0 and 1
//     int r = (int)(9 * (1 - t) * t * t * t * 255);
//     int g = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
//     int b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
    
//     return (r << 16) | (g << 8) | b;
// }

// GREEN
int trippy_color(int iteration)
{
    // double t = (double)iteration / MAX_ITER; // Normalize to [0, 1]

    int r = (int)(sin(0.16 * iteration) * 127 + 128);
    int g = (int)(sin(0.16 * iteration + 2) * 127 + 128);
    int b = (int)(sin(0.16 * iteration + 4) * 127 + 128);

    return (r << 16) | (g << 8) | b;
}

// int trippy_color(int iteration)
// {
//     double t = (double)iteration / MAX_ITER;

//     int r = (int)(9 * (1 - t) * t * t * t * 255);
//     int g = (int)(10 * (1 - t) * (1 - t) * t * t * 255);
//     int b = (int)(6 * (1 - t) * (1 - t) * (1 - t) * t * 255);

//     return (r << 16) | (g << 8) | b;
// }


