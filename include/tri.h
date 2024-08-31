#include "/home/ishaan-user/Documents/C++/nimbus-linalg/include/vec.h"
#include "/home/ishaan-user/Documents/C++/nimbus-linalg/include/mat.h"

#include <SDL2/SDL.h>

void bounding_box(
    la::vec3 v1 , la::vec3 v2 , la::vec3 v3 ,
    int* out_x_0 , int* out_y_0,
    int* out_x_1 , int* out_y_1
);

float tri_area(
    la::vec3 v1 , la::vec3 v2 , la::vec3 v3
);

void TRI_FillTriangle(
    la::vec3 v1 , la::vec3 v2 , la::vec3 v3 ,
    float* depth_buffer ,
    SDL_Renderer* renderer
);

void TRI_FillTriangleTex(
    la::vec3 v1 , la::vec3 v2 , la::vec3 v3 ,
    float tx0 , float ty0 ,
    float tx1 , float ty1 ,
    float tx2 , float ty2 ,
    SDL_Renderer* renderer
);

void TRI_FillTriangleInterp(
    la::vec3 v1 , la::vec3 v2 , la::vec3 v3 ,
    SDL_Renderer* renderer
);

void TRI_DrawLine(
    int x0 , int y0,
    int x1 , int y1,
    SDL_Renderer* renderer
);

void TRI_DrawTriangle(
    la::vec3 v1 , la::vec3 v2 , la::vec3 v3 ,
    SDL_Renderer* renderer
);
