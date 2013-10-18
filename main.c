/*
Copyright 2013 Torin Cooper-Bennun

This file is part of SDLbrot.

SDLbrot is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SDLbrot is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SDLbrot.  If not, see <http://www.gnu.org/licenses/>.
*/
 
#include "main.h"

FILE *debug_file = NULL;
 
/*
 * main
 * Program entry point
 */
int main(int argc, char **argv)
{
    debug_file = fopen("debug.txt", "w");

    /* General variables */
    int return_val = 0;
    cmp_num top_left = c_new(-0.743650449, 0.1318314485), bottom_right = c_new(-0.743635821, 0.1318204775);

    /* Loop variables */
    int fps_counter = 0, ticks_passed = 0, closed = 0;
    SDL_Event event;

    /* SDL variables */
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    /* Initialise SDL */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed\n");
        exit(1);
    }
   
    /* Create window */
    window = SDL_CreateWindow("SDLbrot",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              800, 600, 0);
    if (window == NULL) {
        printf("Window creation failed\n");
        exit(1);
    }

    /* Create rendering context */
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        printf("Creating rendering context failed\n");
        exit(1);
    }

    /* Generate initial Mandelbrot set image */
    SDL_Texture *mandelbrot = compute_set(top_left, bottom_right, 800, 600, renderer);
    if (mandelbrot == NULL) {
        printf("Mandelbrot generation failed\n");
        exit(1);
    }
    
    fclose(debug_file);

    /* Event and render loop */
    while (!closed) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    closed = 1;
                    break;
            }
        }

        ticks_passed = SDL_GetTicks() - fps_counter;
        if (ticks_passed < 16) {
            SDL_Delay(16 - ticks_passed);
        } else {
            fps_counter = SDL_GetTicks();
            if (SDL_RenderClear(renderer) != 0) {
                printf("Render clear failed\n");
                exit(1);
            }
            if (SDL_RenderCopy(renderer, mandelbrot, NULL, NULL) != 0) {
                printf("Render copy failed\n");
                exit(1);
            }
            SDL_RenderPresent(renderer);
        }
    }
   
    /* Finish up */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return return_val;
}

/*
 * c_new
 * Returns new complex number struct
 */
cmp_num c_new(long double re, long double im)
{
    cmp_num c;
    c.re = re;
    c.im = im;
    return c;
}

/*
 * c_add
 * Returns sum of two complex numbers
 */
cmp_num c_add(cmp_num a, cmp_num b)
{
    cmp_num c;
    c.re = a.re + b.re;
    c.im = a.im + b.im;
    return c;
}

/*
 * c_sqr
 * Returns square of complex number
 */
cmp_num c_sqr(cmp_num z)
{
    cmp_num c;
    c.re = (z.re * z.re) - (z.im * z.im);
    c.im = 2 * z.re * z.im;
    return c;
}

/*
 * c_mult
 * Returns product of two complex numbers
 */
cmp_num c_mult(cmp_num a, cmp_num b)
{
    cmp_num c;
    c.re = (a.re * b.re) - (a.im * b.im);
    c.im = (a.re * b.im) + (a.im * b.re);
    return c;
}

/*
 * c_abs
 * Returns absolute value of a complex number
 */
long double c_abs(cmp_num z)
{
    long double a = sqrtl((z.re * z.re) + (z.im * z.im));
    return a;
}

/*
 * mandelbrot_algorithm
 * Returns colour for specified complex number
 * according to results of Mandelbrot iterator
 */
Uint32 mandelbrot_algorithm(cmp_num coord, int iterations, double long limit, SDL_PixelFormat *format)
{
    cmp_num z = c_new(0, 0);
    int i = 0, in_set = 1;
    Uint32 pixel = 0;
    
    for (i = 0; i < iterations; i++) {
        z = c_add(c_sqr(z), coord);
        if (c_abs(z) > limit) {
            in_set = 0;
            break;
        }
    }
    if (in_set) {
        pixel = SDL_MapRGB(format, 0, 0, 0);
    } else {
        pixel = SDL_MapRGB(format, 255, 255, 255);
    }

    return pixel;
}

/*
 * compute_set
 * Returns Mandelbrot image of specified size,
 * for specified coordinates
 */
SDL_Texture *compute_set(cmp_num top_left, cmp_num bottom_right, int img_w, int img_h, SDL_Renderer *renderer)
{
    /* Variables */
    SDL_Surface *surface = NULL;
    cmp_num coord = top_left;
    int i = 0, j = 0;
    long double reDivisions = 0.0, imDivisions = 0.0;
    Uint32 *pixel, colour = 0;
    int iterations = 1000;
    long double limit = 2.0;

    /* Create surface */
    surface = SDL_CreateRGBSurface(0, img_w, img_h, 32, 0, 0, 0, 0);
    if (surface == NULL) {
        printf("Surface creation failed\n");
        return NULL;
    }
    if (SDL_SetSurfaceRLE(surface, 1) != 0) {
        printf("Setting surface RLE failed\n");
        return NULL;
    }
    if (SDL_LockSurface(surface) != 0) {
        printf("Locking surface failed\n");
        return NULL;
    }

    /* Calculate 'gaps' between pixels */
    reDivisions = (bottom_right.re - top_left.re) / (long double)img_w;
    imDivisions = (top_left.im - bottom_right.im) / (long double)img_h;

    /* Run algorithm on coordinate assigned to each pixel */
    for (i = 0; i < img_h; i++) {
        coord.re = top_left.re;
        for (j = 0; j < surface->pitch; j += 4) {
            pixel = (Uint32 *)(surface->pixels + j + (i * surface->pitch));
            colour = mandelbrot_algorithm(coord, iterations, limit, surface->format);
            *pixel = colour;
            coord.re += reDivisions;
        }
        coord.im -= imDivisions;
    }

    SDL_UnlockSurface(surface);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        printf("Texture creation failed\n");
        return NULL;
    }
    
    return texture;
}
