#include "../include/tri.h"
#include "../include/global.h"

#include <iostream>

void bounding_box(
    la::vec3 v1 , la::vec3 v2 , la::vec3 v3 ,
    int* out_x_0 , int* out_y_0,
    int* out_x_1 , int* out_y_1
)
{
    *out_x_0 = std::min(v1.x , std::min(v2.x , v3.x));
    *out_y_0 = std::min(v1.y , std::min(v2.y , v3.y));

    *out_x_1 = std::max(v1.x , std::max(v2.x , v3.x));
    *out_y_1 = std::max(v1.y , std::max(v2.y , v3.y));
}

float tri_area(
    la::vec3 v1 , la::vec3 v2 , la::vec3 v3
)
{
    return 0.5f * std::abs(
        v1.x * (v2.y - v3.y) +
        v2.x * (v3.y - v1.y) +
        v3.x * (v1.y - v2.y)
    );
}

void TRI_FillTriangle(
    la::vec4 v1 , la::vec4 v2 , la::vec4 v3 ,
    float* depth_buffer ,
    SDL_Renderer* renderer
)
{
    v1 = v1.ViewportTransform(WINDOW_WIDTH , WINDOW_HEIGHT);
    v2 = v2.ViewportTransform(WINDOW_WIDTH , WINDOW_HEIGHT);
    v3 = v3.ViewportTransform(WINDOW_WIDTH , WINDOW_HEIGHT);
    int bbox_min_x;
    int bbox_min_y;
    int bbox_max_x;
    int bbox_max_y;
    bounding_box(v1 , v2 , v3 , &bbox_min_x , &bbox_min_y , &bbox_max_x , &bbox_max_y);

    float total_area = tri_area(v1 , v2 , v3);
    
    for (int i=bbox_min_y ; i<bbox_max_y && i<WINDOW_HEIGHT ; i++)
    {
        for (int j=bbox_min_x ; j<bbox_max_x && j<WINDOW_WIDTH ; j++)
        {
            la::vec3 fragment_vec = la::vec3(j , i , 1.0f);
            float u = tri_area(fragment_vec , v2 , v3) / total_area;
            float v = tri_area(fragment_vec , v1 , v3) / total_area;
            float w = tri_area(fragment_vec , v1 , v2) / total_area;

            float fragment_w = u*v1.w + v*v2.w + w*v3.w;

            float fragment_z = (v1.z*u / fragment_w + v2.z*v / fragment_w + v3.z*w / fragment_w) * fragment_w;

            if (
                u + v + w > 0.998 && u + v + w < 1.002 &&
                depth_buffer[(i*WINDOW_WIDTH) + j] > fragment_z &&
                fragment_z > 0.2f // do not render if too close to camera
            )
            {
                SDL_RenderDrawPoint(renderer , j , i);
                depth_buffer[(i*WINDOW_WIDTH) + j] = fragment_z;    
            }
        }
    }
}
void TRI_FillTriangleInterp(
    la::vec4 v1 , la::vec4 v2 , la::vec4 v3 ,
    float* depth_buffer ,
    SDL_Renderer* renderer
)
{
    v1 = v1.ViewportTransform(WINDOW_WIDTH , WINDOW_HEIGHT);
    v2 = v2.ViewportTransform(WINDOW_WIDTH , WINDOW_HEIGHT);
    v3 = v3.ViewportTransform(WINDOW_WIDTH , WINDOW_HEIGHT);
    int bbox_min_x;
    int bbox_min_y;
    int bbox_max_x;
    int bbox_max_y;
    bounding_box(v1 , v2 , v3 , &bbox_min_x , &bbox_min_y , &bbox_max_x , &bbox_max_y);

    float total_area = tri_area(v1 , v2 , v3);
    
    for (int i=bbox_min_y ; i<bbox_max_y && i<WINDOW_HEIGHT ; i++)
    {
        for (int j=bbox_min_x ; j<bbox_max_x && j<WINDOW_WIDTH ; j++)
        {
            la::vec3 fragment_vec = la::vec3(j , i , 1.0f);
            float u = tri_area(fragment_vec , v2 , v3) / total_area;
            float v = tri_area(fragment_vec , v1 , v3) / total_area;
            float w = tri_area(fragment_vec , v1 , v2) / total_area;

            float fragment_z = (v1.z*u + v2.z*v + v3.z*w);

            if (
                u + v + w > 0.998 && u + v + w < 1.002 &&
                depth_buffer[(i*WINDOW_WIDTH) + j] > fragment_z &&
                fragment_z > 0.2f // do not render if too close to camera
            )
            {
                SDL_SetRenderDrawColor(renderer , u*255.0f , v*255.0f , w*255.0f , 255);
                SDL_RenderDrawPoint(renderer , j , i);
                depth_buffer[(i*WINDOW_WIDTH) + j] = fragment_z;    
            }
        }
    }
}

void TRI_FillTriangleTex(
    la::vec4 v1 , la::vec4 v2 , la::vec4 v3 ,
    la::vec3 tv1 , la::vec3 tv2 , la::vec3 tv3 ,
    float* depth_buffer ,
    SDL_Renderer* renderer
)
{
    tv1 = tv1 / v1.w;
    tv2 = tv2 / v2.w;
    tv3 = tv3 / v3.w;

    v1 = v1.ViewportTransform(WINDOW_WIDTH , WINDOW_HEIGHT);
    v2 = v2.ViewportTransform(WINDOW_WIDTH , WINDOW_HEIGHT);
    v3 = v3.ViewportTransform(WINDOW_WIDTH , WINDOW_HEIGHT);
    int bbox_min_x;
    int bbox_min_y;
    int bbox_max_x;
    int bbox_max_y;
    bounding_box(v1 , v2 , v3 , &bbox_min_x , &bbox_min_y , &bbox_max_x , &bbox_max_y);

    float total_area = tri_area(v1 , v2 , v3);
    
    for (int i=bbox_min_y ; i<bbox_max_y && i<WINDOW_HEIGHT ; i++)
    {
        for (int j=bbox_min_x ; j<bbox_max_x && j<WINDOW_WIDTH ; j++)
        {
            la::vec3 fragment_vec = la::vec3(j , i , 1.0f);
            float u = tri_area(fragment_vec , v2 , v3) / total_area;
            float v = tri_area(fragment_vec , v1 , v3) / total_area;
            float w = tri_area(fragment_vec , v1 , v2) / total_area;

            float fragment_w = (v1.w*u + v2.w*v + v3.w*w);

            float fragment_z = (v1.z*u/fragment_w + v2.z*v/fragment_w + v3.z*w/fragment_w) * fragment_w;

            if (
                u + v + w > 0.998 && u + v + w < 1.002 &&
                depth_buffer[(i*WINDOW_WIDTH) + j] > fragment_z &&
                fragment_z > 0.2f // do not render if too close to the camera
            )
            {
                int texel_x = (u*tv1.x + v*tv2.x + w*tv3.x) * fragment_w * texture_width; 
                int texel_y = (u*tv1.y + v*tv2.y + w*tv3.y) * fragment_w * texture_height;

                if (texel_x < texture_width && texel_y < texture_height)
                {
                    unsigned char red;
                    unsigned char green;
                    unsigned char blue;

                    unsigned char* texel = texture_image + (
                        ((texture_width * texel_y) + texel_x)*texture_bpp
                    );

                    red = *texel;
                    green = *(texel + 1);
                    blue = *(texel + 2);

                    SDL_SetRenderDrawColor(renderer , red , green , blue , 255);
                    SDL_RenderDrawPoint(renderer , j , i);
                    depth_buffer[(i*WINDOW_WIDTH) + j] = fragment_z;    
                }
            }
        }
    }
}


void TRI_DrawLine(
    la::vec3 v1 , la::vec3 v2 ,
    SDL_Renderer* renderer
)
{
    bool steep = false;
    if (abs(v2.x - v1.x) < abs(v2.y - v1.y)) {
        std::swap(v1.x , v1.y);
        std::swap(v2.x , v2.y);
        steep = true;
    }

    if (v1.x > v1.x) {
        std::swap(v2 , v1);
    }

    float slope = (v1.y - v1.y) / (float)(v2.x - v1.x);
    for (int i=0 ; i<=abs(v2.x - v1.x) ; i++)
    {
        int y = slope * i;
        if (steep) {
            SDL_RenderDrawPoint(renderer , v1.y + y , v1.x + i);
        } else {
            SDL_RenderDrawPoint(renderer , v1.x + i , v1.y + y);
        }
    }
}

void TRI_DrawTriangle(
    la::vec3 v1 , la::vec3 v2 , la::vec3 v3 ,
    SDL_Renderer* renderer
)
{
    v1 = v1.ViewportTransform(WINDOW_WIDTH , WINDOW_HEIGHT);
    v2 = v2.ViewportTransform(WINDOW_WIDTH , WINDOW_HEIGHT);
    v3 = v3.ViewportTransform(WINDOW_WIDTH , WINDOW_HEIGHT);

    TRI_DrawLine(v1 , v2 , renderer);
    TRI_DrawLine(v2 , v3 , renderer);
    TRI_DrawLine(v3 , v1 , renderer);
}
