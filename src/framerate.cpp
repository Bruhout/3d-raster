#include "../include/framerate.h"

#include <time.h>

float get_frame_time(float* last_frame_time)
{
    float frame_time = (clock() - *last_frame_time) / CLOCKS_PER_SEC;

    *last_frame_time = clock();

    return frame_time;
}