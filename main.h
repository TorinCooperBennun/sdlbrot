#ifndef MAIN_H
#define MAIN_H

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
 
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
typedef struct {
    long double re;
    long double im;
} cmp_num;

cmp_num c_new(long double re, long double im);
cmp_num c_add(cmp_num a, cmp_num b);
cmp_num c_sqr(cmp_num z);
cmp_num c_mult(cmp_num a, cmp_num b);
long double c_abs(cmp_num z);
Uint32 mandelbrot_algorithm(cmp_num coord, int iterations, double long limit, SDL_PixelFormat *format);
SDL_Texture *compute_set(cmp_num top_left, cmp_num bottom_right, int img_w, int img_h, SDL_Renderer *renderer);

#endif