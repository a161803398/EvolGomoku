#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"


SDL_Surface *loadImg(const char filename[]);
void applyImg(SDL_Rect *offset, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);
void applyImg(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip = NULL);
//

#endif // DISPLAY_H_INCLUDED
