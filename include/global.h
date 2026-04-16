#ifndef GLOBAL_H
#define GLOBAL_H

#include "../nimbus-linalg/include/vec.h"
#include "../nimbus-linalg/include/mat.h"

// texture variables
extern int texture_width;
extern int texture_height;
extern int texture_bpp;
extern unsigned char* texture_image;

// windows variables
extern int WINDOW_HEIGHT;
extern int WINDOW_WIDTH; 
extern int CHANNELS;

// object matrix variables
extern float pitch;
extern float yaw;
extern la::mat4 obj_mat;


// view matrix variables
extern la::vec3 cam_pos;
extern la::vec3 cam_aim;
extern la::vec3 world_up;
extern la::mat4 view_mat;


// perspective projection matrix variables
extern float z_near;
extern float z_far;
extern float fov;
extern la::mat4 proj_mat;


#endif
