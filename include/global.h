#ifndef GLOBAL_H
#define GLOBAL_H

// instead of using variables i have used defines because these remain constant throughout the program
#define WINDOW_HEIGHT 512 
#define WINDOW_WIDTH 512  

#include "/home/ishaan-user/Documents/C++/nimbus-linalg/include/vec.h"
#include "/home/ishaan-user/Documents/C++/nimbus-linalg/include/mat.h"

// texture variables
extern int texture_width;
extern int texture_height;
extern int texture_bpp;
extern unsigned char* texture_image;


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
