#include "../include/global.h"

#define STB_IMAGE_IMPLEMENTATION
#include "/home/ishaan-user/libs-clones/stb/stb_image.h"


extern int texture_width = 10;
extern int texture_height = 10;
extern int texture_bpp = 10;
extern unsigned char* texture_image = stbi_load("container.jpg" , &texture_width , &texture_height , &texture_bpp , 0);

// object matrix variables
extern float pitch = 0.0f;
extern float yaw = 0.0f;
extern la::mat4 obj_mat = la::mat4();


// view matrix variables
extern la::vec3 cam_pos = la::vec3(0.0f , 0.0f , 3.3f);
extern la::vec3 cam_aim = la::vec3(0.0f , 0.0f , 0.0f);
extern la::vec3 world_up = la::vec3(0.0f, 1.0f , 0.0f);
extern la::mat4 view_mat = la::mat4().LookAt(
    cam_pos ,
    cam_aim ,
    world_up
);


// perspective projection matrix variables
extern float z_near = 0.1f;
extern float z_far = 10.0f;
extern float fov = la::radians(40.0f);
extern la::mat4 proj_mat = la::mat4().PerspectiveMat(fov , z_near , z_far);
