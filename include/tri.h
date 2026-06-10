#include "../nimbus-linalg/include/vec.h"
#include "../nimbus-linalg/include/mat.h"

#include "SDL2/SDL.h"

void bounding_box(
    la::vec3 v1 , la::vec3 v2 , la::vec3 v3 ,
    int* out_x_0 , int* out_y_0,
    int* out_x_1 , int* out_y_1
);

float tri_area(
    la::vec3 v1 , la::vec3 v2 , la::vec3 v3
);


void TRI_FillTriangleTex(
    la::vec3 v1, la::vec3 v2, la::vec3 v3,
    la::vec3 tv1, la::vec3 tv2, la::vec3 tv3,
    float* depth_buffer,
    Uint32* pixel_buffer
);