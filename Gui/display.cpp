#include "display.h"

SDL_Surface *loadImg(const char filename[]) {
    SDL_Surface *loadedImage = NULL, *optimizedImage = NULL;

    if((loadedImage = IMG_Load(filename)) != NULL) {
        optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
        SDL_FreeSurface(loadedImage);
    }

    return optimizedImage;
}


void applyImg(SDL_Rect *offset, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
    SDL_BlitSurface(source, clip, destination, offset);
}

void applyImg(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, clip, destination, &offset);
}
