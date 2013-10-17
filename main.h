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
#include <string.h>
 
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define RMASK 0xff000000
#define GMASK 0x00ff0000
#define BMASK 0x0000ff00
#define AMASK 0x000000ff
#else
#define RMASK 0x000000ff
#define GMASK 0x0000ff00
#define BMASK 0x00ff0000
#define AMASK 0xff000000
#endif
 
typedef struct {
    long double re;
    long double im;
} cmp_num;

cmp_num c_new(long double re, long double im);
cmp_num c_add(cmp_num a, cmp_num b);
cmp_num c_sqr(cmp_num z);
cmp_num c_mult(cmp_num a, cmp_num b);
SDL_Texture *compute_set(cmp_num top_left, cmp_num bottom_right, int img_w, int img_h, SDL_Renderer *renderer);
