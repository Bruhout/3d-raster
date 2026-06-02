#include "../include/tri.h"
#include "../include/global.h"
#include <iostream>
#include <omp.h>

// each entry of PixelBuffer is RGBA packed as Uint32
void bounding_box(
    la::vec3 v1, la::vec3 v2, la::vec3 v3,
    int* out_x_0, int* out_y_0,
    int* out_x_1, int* out_y_1
)
{
    *out_x_0 = std::max(0, (int)std::min(v1.get_x(), std::min(v2.get_x(), v3.get_x())));
    *out_y_0 = std::max(0, (int)std::min(v1.get_y(), std::min(v2.get_y(), v3.get_y())));
    *out_x_1 = std::min(WINDOW_WIDTH,  (int)std::max(v1.get_x(), std::max(v2.get_x(), v3.get_x())));
    *out_y_1 = std::min(WINDOW_HEIGHT, (int)std::max(v1.get_y(), std::max(v2.get_y(), v3.get_y())));
}

float tri_area(la::vec3 v1, la::vec3 v2, la::vec3 v3)
{
    return 0.5f * std::abs(
        v1.get_x() * (v2.get_y() - v3.get_y()) +
        v2.get_x() * (v3.get_y() - v1.get_y()) +
        v3.get_x() * (v1.get_y() - v2.get_y())
    );
}


void TRI_FlushPixelBuffer(Uint32* pixel_buffer, SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_UpdateTexture(texture, NULL, pixel_buffer, WINDOW_WIDTH * sizeof(Uint32));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void TRI_ResetPixelBuffer(Uint32* pixel_buffer)
{
    // background color: RGB(50, 50, 120) = 0x323278FF
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++)
        pixel_buffer[i] = 0x3232'78FF;
}



void TRI_FillTriangleTex(
    la::vec3 v1, la::vec3 v2, la::vec3 v3,
    la::vec3 tv1, la::vec3 tv2, la::vec3 tv3,
    float* depth_buffer,
    Uint32* pixel_buffer        // NEW: replaces SDL_Renderer* renderer
)
{
    v1 = v1.ViewportTransform(WINDOW_WIDTH, WINDOW_HEIGHT);
    v2 = v2.ViewportTransform(WINDOW_WIDTH, WINDOW_HEIGHT);
    v3 = v3.ViewportTransform(WINDOW_WIDTH, WINDOW_HEIGHT);

    int bbox_min_x, bbox_min_y, bbox_max_x, bbox_max_y;
    bounding_box(v1, v2, v3, &bbox_min_x, &bbox_min_y, &bbox_max_x, &bbox_max_y);
    float total_area = tri_area(v1, v2, v3);
    if (total_area < 0.0001f) return;

    #pragma omp parallel for schedule(dynamic, 4)
    for (int i = bbox_min_y; i < bbox_max_y; i++)
    {
        for (int j = bbox_min_x; j < bbox_max_x; j++)
        {
            la::vec3 frag = la::vec3(j, i, 1.0f);
            float u = tri_area(frag, v2, v3) / total_area;
            float v = tri_area(frag, v1, v3) / total_area;
            float w = tri_area(frag, v1, v2) / total_area;
            float fz = v1.get_z()*u + v2.get_z()*v + v3.get_z()*w;

            if (u + v + w > 0.998f && u + v + w < 1.002f)
            {
                int idx = i * WINDOW_WIDTH + j;
                if (depth_buffer[idx] > fz)
                {
                    int texel_x = (int)((u*tv1.get_x() + v*tv2.get_x() + w*tv3.get_x()) * texture_width);
                    int texel_y = (int)((u*tv1.get_y() + v*tv2.get_y() + w*tv3.get_y()) * texture_height);

                    if (texel_x >= 0 && texel_x < texture_width &&
                        texel_y >= 0 && texel_y < texture_height)
                    {
                        unsigned char* texel = texture_image +
                            ((texture_width * texel_y) + texel_x) * texture_bpp;

                        // Pack RGBA into Uint32 (SDL default: RGBA8888)
                        Uint32 packed = ((Uint32)texel[0] << 24) |
                                        ((Uint32)texel[1] << 16) |
                                        ((Uint32)texel[2] <<  8) | 0xFF;

                        depth_buffer[idx] = fz;
                        pixel_buffer[idx] = packed;
                    }
                }
            }
        }
    }
}