#include "../include/global.h"

// texture variables
int texture_width = 10;
int texture_height = 10;
int texture_bpp = 3;
unsigned char* texture_image;

// image variables
int WINDOW_HEIGHT = 256;
int WINDOW_WIDTH = 256;
int CHANNELS = 256;

// object matrix variables
float pitch = 0.0f;
float yaw = 0.0f;
la::mat4 obj_mat = la::mat4();


// view matrix variables
la::vec3 cam_pos = la::vec3(0.0f , 0.0f , 3.3f);
la::vec3 cam_aim = la::vec3(0.0f , 0.0f , 0.0f);
la::vec3 world_up = la::vec3(0.0f, 1.0f , 0.0f);
la::mat4 view_mat = la::mat4().LookAt(
    cam_pos ,
    cam_aim ,
    world_up
);


// perspective projection matrix variables
float z_near = 0.1f;
float z_far = 10.0f;
float fov = la::radians(40.0f);
la::mat4 proj_mat = la::mat4().PerspectiveMat(fov , z_near , z_far);
