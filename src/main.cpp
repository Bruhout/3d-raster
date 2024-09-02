#include <iostream>
#include <time.h>

#include <SDL2/SDL.h>

#include "/home/ishaan-user/Documents/C++/nimbus-linalg/include/vec.h"
#include "/home/ishaan-user/Documents/C++/nimbus-linalg/include/mat.h"

#include "../include/tri.h"
#include "../include/global.h"
#include "../include/framerate.h"

void init_window(SDL_Window** window , SDL_Renderer** renderer);
void process_input(int* game_is_running);
void reset_depth_buffer(float* depth_buffer);

int main(void)
{
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

    // BEGIN SDL-----------------------------------------------------------------
    // --------------------------------------------------------------------------
    SDL_Window* window;
    SDL_Renderer* renderer;
    init_window(&window , &renderer);

    int game_is_running = 0;

    // final vertices
    la::vec4 fv[8];

    float last_frame_time = clock();
    float frame_time = 0;

    while (game_is_running == 0)
    {
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

            // apply viewport transform
            // removed, viewport transform now performed in the draw triangle function
            // fv[i] = fv[i].ViewportTransform(WINDOW_WIDTH , WINDOW_HEIGHT);

            // add frame time to maintain game speed
            pitch = frame_time * 0.5f;
            yaw = frame_time * 1.0f;
        }

        process_input(&game_is_running);

        SDL_SetRenderDrawColor(renderer , 50 , 50 , 120 , 255);
        SDL_RenderClear(renderer);

        // triangle draw calls

        /*
        // face 1
        SDL_SetRenderDrawColor(renderer , 200 , 50 , 50 , 255);
        TRI_FillTriangle(
            fv[0] , fv[1] , fv[2] ,
            depth_buffer ,
            renderer
        );
        TRI_FillTriangle(
            fv[0] , fv[3] , fv[2] ,
            depth_buffer ,
            renderer
        );

        // face 2
        SDL_SetRenderDrawColor(renderer , 50 , 200 , 50 , 255);
        TRI_FillTriangle(
            fv[4] , fv[5] , fv[6] ,
            depth_buffer ,
            renderer
        );
        TRI_FillTriangle(
            fv[4] , fv[7] , fv[6] ,
            depth_buffer ,
            renderer
        );

        // face 3
        SDL_SetRenderDrawColor(renderer , 152, 166, 235 , 255);
        TRI_FillTriangle(
            fv[4] , fv[5] , fv[1] ,
            depth_buffer ,
            renderer
        );
        TRI_FillTriangle(
            fv[4] , fv[0] , fv[1] ,
            depth_buffer ,
            renderer
        );

        // face 4
        SDL_SetRenderDrawColor(renderer , 169, 93, 199 , 255);
        TRI_FillTriangle(
            fv[7] , fv[6] , fv[2] ,
            depth_buffer ,
            renderer
        );
        TRI_FillTriangle(
            fv[7] , fv[3] , fv[2] ,
            depth_buffer ,
            renderer
        );

        // face 5
        SDL_SetRenderDrawColor(renderer , 0, 255, 204 , 255);
        TRI_FillTriangle(
            fv[5] , fv[1] , fv[2] ,
            depth_buffer ,
            renderer
        );
        TRI_FillTriangle(
            fv[5] , fv[6] , fv[2] ,
            depth_buffer ,
            renderer
        );

        // face 6
        SDL_SetRenderDrawColor(renderer , 230, 192, 28 , 255);
        TRI_FillTriangle(
            fv[4] , fv[0] , fv[3] ,
            depth_buffer ,
            renderer
        );
        TRI_FillTriangle(
            fv[4] , fv[7] , fv[3] ,
            depth_buffer ,
            renderer
        );
        */

        // face 1
        TRI_FillTriangleTex(
            fv[0] , fv[1] , fv[2] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer ,
            renderer
        );
        TRI_FillTriangleTex(
            fv[0] , fv[3] , fv[2] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer ,
            renderer
        );

        // face 2
        TRI_FillTriangleTex(
            fv[4] , fv[5] , fv[6] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer ,
            renderer
        );
        TRI_FillTriangleTex(
            fv[4] , fv[7] , fv[6] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer ,
            renderer
        );

        // face 3
        TRI_FillTriangleTex(
            fv[4] , fv[5] , fv[1] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer ,
            renderer
        );
        TRI_FillTriangleTex(
            fv[4] , fv[0] , fv[1] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer ,
            renderer
        );

        // face 4
        TRI_FillTriangleTex(
            fv[7] , fv[6] , fv[2] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer ,
            renderer
        );
        TRI_FillTriangleTex(
            fv[7] , fv[3] , fv[2] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer ,
            renderer
        );

        // face 5
        TRI_FillTriangleTex(
            fv[5] , fv[1] , fv[2] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer ,
            renderer
        );
        TRI_FillTriangleTex(
            fv[5] , fv[6] , fv[2] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer ,
            renderer
        );

        // face 6
        TRI_FillTriangleTex(
            fv[4] , fv[0] , fv[3] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer ,
            renderer
        );
        TRI_FillTriangleTex(
            fv[4] , fv[7] , fv[3] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer ,
            renderer
        );

        reset_depth_buffer(depth_buffer);
        SDL_RenderPresent(renderer);
        frame_time = get_frame_time(&last_frame_time);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;

}

void init_window(SDL_Window** window , SDL_Renderer** renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { printf("SLD_Init error\n"); }

    *window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );

    *renderer = SDL_CreateRenderer(
        *window,
        -1,
        0
    );
}

void process_input(int* game_is_running)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT) {
        *game_is_running = 1;
    }

    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_w) {
            cam_pos = cam_pos + (cam_aim - cam_pos).Normalize() * 0.1f;
        } else if (event.key.keysym.sym == SDLK_s) {
            cam_pos = cam_pos - (cam_aim - cam_pos).Normalize() * 0.1f;
        } else if (event.key.keysym.sym == SDLK_a) {
            la::vec3 cam_right = (world_up * cam_pos).Normalize();
            cam_pos = cam_pos - cam_right * 0.1f;
            cam_aim = cam_aim - cam_right * 0.1f;
        } else if (event.key.keysym.sym == SDLK_d) {
            la::vec3 cam_right = (world_up * cam_pos).Normalize();
            cam_pos = cam_pos + cam_right * 0.1f;
            cam_aim = cam_aim + cam_right * 0.1f;
        }

        view_mat = la::mat4().LookAt(
            cam_pos ,
            cam_aim ,
            world_up
        );
    }
}

void reset_depth_buffer(float* depth_buffer)
{
    for (int i=0 ; i<WINDOW_HEIGHT*WINDOW_WIDTH ; i++) {
        depth_buffer[i] = 1.0f;
    }
}