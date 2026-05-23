#include <iostream>
#include <time.h>

#include <SDL2/SDL.h>
#include <omp.h>                         // NEW: OpenMP

#include "../nimbus-linalg/include/vec.h"
#include "../nimbus-linalg/include/mat.h"

#include "../include/tri.h"
#include "../include/global.h"
#include "../include/framerate.h"

void init_window(SDL_Window** window , SDL_Renderer** renderer);
void process_input(int* game_is_running);
void reset_depth_buffer(float* depth_buffer);
void reset_pixel_buffer(Uint32* pixel_buffer);   // NEW

int main(void)
{
    // define and setup the vertices
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

    // define texture coordinates
    la::vec3 tv[] = {
        la::vec3(1.0f , 0.0f , 1.0f),
        la::vec3(1.0f , 1.0f , 1.0f),
        la::vec3(0.0f , 1.0f , 1.0f),
        la::vec3(0.0f , 0.0f , 1.0f)
    };

    // depth buffer
    float depth_buffer[WINDOW_HEIGHT * WINDOW_WIDTH];

    // pixel buffer, threads write RGBA pixels here
    Uint32 pixel_buffer[WINDOW_HEIGHT * WINDOW_WIDTH];

    // BEGIN SDL
    SDL_Window* window;
    SDL_Renderer* renderer;
    init_window(&window , &renderer);

    // streaming texture, the pixel buffer is uploaded to this each frame
    SDL_Texture* framebuffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );
    if (!framebuffer_texture) {
        printf("SDL_CreateTexture error: %s\n", SDL_GetError());
        return 1;
    }

    int game_is_running = 0;

    // final vertices
    la::vec4 fv[8];

    float last_frame_time = clock();
    float frame_time = 0;

    float total_time = 0.0f;
    while (game_is_running == 0)
    {
        obj_mat = plain_mat.RotateMat(pitch , yaw , 0.0f);
        // apply all transforms
        total_time += frame_time;
        pitch = total_time * 0.1f;
        yaw   = total_time * 0.2f;
        for (int i=0 ; i<8 ; i++)
        {
            fv[i] = vertices[i] * obj_mat;
            fv[i] = fv[i] * view_mat;
            fv[i] = fv[i].PersProjectVec(proj_mat);

        }

        process_input(&game_is_running);

        // clear both buffers
        reset_depth_buffer(depth_buffer);
        reset_pixel_buffer(pixel_buffer);

        // --- triangle draw calls ---
        // face 1
        TRI_FillTriangleTex(
            fv[0] , fv[1] , fv[2] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer , pixel_buffer
        );
        TRI_FillTriangleTex(
            fv[0] , fv[3] , fv[2] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer , pixel_buffer
        );

        // face 2
        TRI_FillTriangleTex(
            fv[4] , fv[5] , fv[6] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer , pixel_buffer
        );
        TRI_FillTriangleTex(
            fv[4] , fv[7] , fv[6] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer , pixel_buffer
        );

        // face 3
        TRI_FillTriangleTex(
            fv[4] , fv[5] , fv[1] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer , pixel_buffer
        );
        TRI_FillTriangleTex(
            fv[4] , fv[0] , fv[1] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer , pixel_buffer
        );

        // face 4
        TRI_FillTriangleTex(
            fv[7] , fv[6] , fv[2] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer , pixel_buffer
        );
        TRI_FillTriangleTex(
            fv[7] , fv[3] , fv[2] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer , pixel_buffer
        );

        // face 5
        TRI_FillTriangleTex(
            fv[5] , fv[1] , fv[2] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer , pixel_buffer
        );
        TRI_FillTriangleTex(
            fv[5] , fv[6] , fv[2] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer , pixel_buffer
        );

        // face 6
        TRI_FillTriangleTex(
            fv[4] , fv[0] , fv[3] ,
            tv[0] , tv[1] , tv[2] ,
            depth_buffer , pixel_buffer
        );
        TRI_FillTriangleTex(
            fv[4] , fv[7] , fv[3] ,
            tv[0] , tv[3] , tv[2] ,
            depth_buffer , pixel_buffer
        );

        // push the completed pixel buffer to the screen in one GPU upload
        SDL_UpdateTexture(framebuffer_texture, NULL, pixel_buffer, WINDOW_WIDTH * sizeof(Uint32));
        SDL_RenderCopy(renderer, framebuffer_texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(20);

        frame_time = get_frame_time(&last_frame_time);
    }

    SDL_DestroyTexture(framebuffer_texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}

void init_window(SDL_Window** window , SDL_Renderer** renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { printf("SDL_Init error\n", SDL_GetError()); }

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
    // parallelised clear
    #pragma omp parallel for schedule(static)
    for (int i=0 ; i<WINDOW_HEIGHT*WINDOW_WIDTH ; i++) {
        depth_buffer[i] = 1.0f;
    }
}

// fill pixel buffer with the background colour
void reset_pixel_buffer(Uint32* pixel_buffer)
{
    #pragma omp parallel for schedule(static)
    for (int i=0 ; i<WINDOW_HEIGHT*WINDOW_WIDTH ; i++) {
        pixel_buffer[i] = 0x323278FF;   // RGBA: 50, 50, 120, 255
    }
}