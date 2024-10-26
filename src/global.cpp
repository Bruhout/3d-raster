#include "../include/global.h"

#define STB_IMAGE_IMPLEMENTATION
#include "/home/ishaan-user/lib-clones/stb/stb_image.h"

// texture variables
int texture_width;
int texture_height;
int texture_bpp;
unsigned char* texture_image;

// object matrix variables
float pitch;
float yaw;
la::mat4 obj_mat;

// view matrix variables
la::vec3 cam_pos;
la::vec3 cam_aim;
la::vec3 world_up;
la::mat4 view_mat;


// perspective projection matrix variables
float z_near;
float z_far;
float fov;
la::mat4 proj_mat;


void setup_globals()
{
    // texture variables
    texture_width = 10;
    texture_height = 10;
    texture_bpp = 3;
    texture_image = stbi_load(
        "container.jpg" , 
        &texture_width , &texture_height ,
        &texture_bpp , 0
    );

    // object matrix variables
    pitch = 0.0f;
    yaw = 0.0f;
    obj_mat = la::mat4();

    // view matrix variables
    cam_pos  = la::vec3(0.0f , 0.0f , 3.3f);
    cam_aim  = la::vec3(0.0f , 0.0f , 0.0f);
    world_up = la::vec3(0.0f, 1.0f , 0.0f);
    view_mat = la::mat4().LookAt(
        cam_pos ,
        cam_aim ,
        world_up
    );

    // perspective projection matrix variables
    z_near = 0.1f;
    z_far = 10.0f;
    fov = la::radians(60.0f);
    proj_mat = la::mat4().PerspectiveMat(fov , z_near , z_far);
}
