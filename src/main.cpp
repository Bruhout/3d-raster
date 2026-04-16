#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>

#include "../nimbus-linalg/include/vec.h"
#include "../nimbus-linalg/include/mat.h"

#include "../include/tri.h"
#include "../include/global.h"
#include "../include/framerate.h"

void reset_depth_buffer(float* depth_buffer);

int main(int argc, char* argv[])
{
    WINDOW_HEIGHT = atoi(argv[1]);
    WINDOW_WIDTH = atoi(argv[2]);
    CHANNELS = atoi(argv[3]);
    int total_frames = atoi(argv[4]);

    // texture variables
    texture_image = stbi_load(
        "container.jpg" , 
        &texture_width , &texture_height ,
        &texture_bpp , 0
    );

    // define and setup the vertices--------------------------------------------------------
    //--------------------------------------------------------------------------------------
    la::vec3 vertices[] = {
        la::vec3( 0.5f ,  0.5f , -0.5f),
        la::vec3( 0.5f , -0.5f , -0.5f),
        la::vec3(-0.5f , -0.5f , -0.5f),
        la::vec3(-0.5f ,  0.5f , -0.5f),
        la::vec3( 0.5f ,  0.5f ,  0.5f),
        la::vec3( 0.5f , -0.5f ,  0.5f),
        la::vec3(-0.5f , -0.5f ,  0.5f),
        la::vec3(-0.5f ,  0.5f ,  0.5f)
    };



    // define texture coordinates and texture image
    //--------------------------------------------------------------------------------------------
    la::vec3 tv[] = {
        la::vec3(1.0f , 0.0f , 1.0f),
        la::vec3(1.0f , 1.0f , 1.0f),
        la::vec3(0.0f , 1.0f , 1.0f),
        la::vec3(0.0f , 0.0f , 1.0f)
    };

    // initialise depth buffer-----------------------------------------------------------------
    // --------------------------------------------------------------------------
    float depth_buffer[WINDOW_HEIGHT * WINDOW_WIDTH] = { 1.0f };

    unsigned char* frame_image = new unsigned char[WINDOW_HEIGHT * WINDOW_WIDTH * 4];

    int game_is_running = 0;

    // final vertices
    la::vec4 fv[8];

    float last_frame_time = clock();
    float frame_time = 0;

    // while (game_is_running == 0)
    int frame_count = 0;
    while (frame_count < total_frames)
    {
        frame_count++;
        obj_mat = obj_mat.RotateMat(pitch , yaw , 0.0f);

        // apply all transforms
        for (int i=0 ; i<8 ; i++)
        {
            // apply rotation (object matrix transform)
            fv[i] = vertices[i] * obj_mat;

            // apply camera space transform (view matrix transform)
            fv[i] = fv[i] * view_mat;

            // apply perspective projection
            fv[i] = fv[i].PersProjectVec(proj_mat);

            // add frame time to maintain game speed
            pitch = frame_time * 0.5f;
            yaw = frame_time * 1.0f;
        }

        for (int i=0 ; i<WINDOW_HEIGHT * WINDOW_WIDTH * 4 ; i = i+4)
        {
            frame_image[i] = 50;
            frame_image[i + 1] = 50;
            frame_image[i + 2] = 120;
            frame_image[i + 3] = 255;
        }

        // triangle draw calls
        // face 1
        TRI_FillTriangleTex(
            fv[0] , fv[1] , fv[2] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer ,
            frame_image
        );
        TRI_FillTriangleTex(
            fv[0] , fv[3] , fv[2] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer ,
            frame_image
        );

        // face 2
        TRI_FillTriangleTex(
            fv[4] , fv[5] , fv[6] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer ,
            frame_image
        );
        TRI_FillTriangleTex(
            fv[4] , fv[7] , fv[6] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer ,
            frame_image
        );

        // face 3
        TRI_FillTriangleTex(
            fv[4] , fv[5] , fv[1] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer ,
            frame_image
        );
        TRI_FillTriangleTex(
            fv[4] , fv[0] , fv[1] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer ,
            frame_image
        );

        // face 4
        TRI_FillTriangleTex(
            fv[7] , fv[6] , fv[2] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer ,
            frame_image
        );
        TRI_FillTriangleTex(
            fv[7] , fv[3] , fv[2] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer ,
            frame_image
        );

        // face 5
        TRI_FillTriangleTex(
            fv[5] , fv[1] , fv[2] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer ,
            frame_image
        );
        TRI_FillTriangleTex(
            fv[5] , fv[6] , fv[2] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer ,
            frame_image
        );

        // face 6
        TRI_FillTriangleTex(
            fv[4] , fv[0] , fv[3] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer ,
            frame_image
        );
        TRI_FillTriangleTex(
            fv[4] , fv[7] , fv[3] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer ,
            frame_image
        );
        reset_depth_buffer(depth_buffer);
        // SDL_RenderPresent(renderer);
	
	std::setvbuf(stdout, NULL, _IONBF, 0);
	std::cout.write((char*)frame_image, WINDOW_HEIGHT*WINDOW_WIDTH*CHANNELS);	

        frame_time = get_frame_time(&last_frame_time);
    }

    return 0;

}

void reset_depth_buffer(float* depth_buffer)
{
    for (int i=0 ; i<WINDOW_HEIGHT*WINDOW_WIDTH ; i++) {
        depth_buffer[i] = 1.0f;
    }
}
