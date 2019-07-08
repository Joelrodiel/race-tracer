#include <iostream>
#include <SDL2/SDL.h>

#include "Sphere.h"
#include "Hitable_list.h"
#include "float.h"
#include "Camera.h"

Vec3 random_in_unit_sphere();
Vec3 color(const Ray& r, Hitable *world);

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

    int ns = 10;
    
    Hitable *list[2];
    list[0] = new Sphere(Vec3(0, 0, -1), 0.5);
    list[1] = new Sphere(Vec3(0, -100.5, -1), 100);

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
                Vec3 p = r.point_at_t(2.0);
                col += color(r, world);
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

Vec3 random_in_unit_sphere()
{
    Vec3 p;
    do {
        p = 2.0*Vec3(drand48(), drand48(), drand48()) - Vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}

Vec3 color(const Ray& r, Hitable *world)
{
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec))
    {
        Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5*color( Ray(rec.p, target-rec.p), world);
    }
    else
    {
        Vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
    }
}
