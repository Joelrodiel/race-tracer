#include <iostream>
#include <SDL2/SDL.h>

#include "Sphere.h"
#include "Hitable_list.h"
#include "float.h"
#include "Camera.h"
#include "Material.h"

Vec3 color(const Ray& r, Hitable *world, int depth);

int main(int argc, char* argv[])
{

    // SDL SETUP
    
    int width = 1200, height = 600;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "SDL Inited properly" << std::endl;
    }

    SDL_Window* window = SDL_CreateWindow(
            "Da Racer",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height,
            SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
    );

    // COLORING

    int ns = 100;
    
    Hitable *list[2];
    list[0] = new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.8, 0.3, 0.3)));
    list[1] = new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0)));

    Hitable *world = new Hitable_list(list, 2);
    
    Camera cam;

    std::cout << "Render start! Go scroll through your twitter feed for a minute..." << std::endl;

    for (int j = height-1; j >= 0; j--)
    {

        if (j == height/2)
        {
            std::cout << "Render is 50\% complete! Go get a drink or something." << std::endl;
        }

        for (int i = 0; i < width; i++)
        {
            Vec3 col(0, 0, 0);

            for (int s = 0; s < ns; s++)
            {
                float u = float(i + drand48()) / float(width);
                float v = float(j + drand48()) / float(height);

                Ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }

            col /= float(ns);
            col = Vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            uint8_t ir = int(255.99*col[0]);
            uint8_t ig = int(255.99*col[1]);
            uint8_t ib = int(255.99*col[2]);

            SDL_SetRenderDrawColor(renderer, ir, ig, ib, 1);
            SDL_RenderDrawPoint(renderer, i, height-j);
        }
    }

    std::cout << "Finished rendering in " << SDL_GetTicks()/1000 << " seconds!" << std::endl;
    SDL_RenderPresent(renderer);

    std::cout << "Press [ENTER] to end program.";

    std::cin.ignore();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

Vec3 color(const Ray& r, Hitable *world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec))
    {
        Ray scattered;
        Vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->Scatter(r, rec, attenuation, scattered))
        {
            return attenuation*color(scattered, world, depth+1);
        }
        else
        {
            return Vec3(0, 0, 0);
        }
    }
    else
    {
        Vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
    }
}
