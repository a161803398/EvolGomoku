#include "Stone.h"

SDL_Surface *Stone::destination_;
SDL_Surface *Stone::stoneImg_;
SDL_Event *Stone::srcEvent_;
bool Stone::pause_ = false;

void Stone::draw(STONE_TYPE type) {
    SDL_Rect BLACK_CHIP = {0, 0, STONE_WIDTH, STONE_HEIGHT};
    SDL_Rect WHITE_CHIP = {STONE_WIDTH, 0, STONE_WIDTH, STONE_HEIGHT};
    SDL_Rect MARK_CHIP = {STONE_WIDTH * 2, 0, STONE_WIDTH, STONE_HEIGHT};


    switch(type) {
    case BLACK: {
        applyImg(&box_, stoneImg_, destination_, &BLACK_CHIP);
    }
    break;

    case WHITE: {
        applyImg(&box_, stoneImg_, destination_, &WHITE_CHIP);
    }
    break;
    default: {
    }
    }
    if(isMarked_)applyImg(&box_, stoneImg_, destination_, &MARK_CHIP);


}

bool Stone::isClick(STONE_TYPE overType) {
    if(type_ == NONE && !pause_) { //if it is none then can click
        switch(RECTBOX::handleEvents(srcEvent_)) {
        case MOUSECLICK:
            //setType(overType);
            return true;
        case MOUSEOVER:
        case MOUSEDOWN:
            //draw(overType); //show stone when mouse over
            return false;
        default: {
        }
        }

    }
    //draw(type_);
    return false;
}

STONE_TYPE reverseStone(STONE_TYPE stone) {
    return (stone == BLACK) ? WHITE : BLACK;
}
