#include "Button.h"

SDL_Surface *Button::destination_;
SDL_Event *Button::srcEvent_;


bool Button::isClick() {
    SDL_Rect normal = {0, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect over = {BUTTON_WIDTH, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect down = {BUTTON_WIDTH * 2, 0, BUTTON_WIDTH, BUTTON_HEIGHT};

    if(enable_) {
        switch(RECTBOX::handleEvents(srcEvent_)) {
        case MOUSECLICK:
            if(clickOnce_) enable_ = false;
            applyImg(&box_, tarImg_, destination_, &down);
            return true;
        case MOUSEOVER:
            applyImg(&box_, tarImg_, destination_, &over);
            return false;
        case MOUSEDOWN:
            applyImg(&box_, tarImg_, destination_, &down);
            return false;
        default: {
            applyImg(&box_, tarImg_, destination_, &normal);
            return false;
        }
        }
    }else{
        applyImg(&box_, tarImg_, destination_, &down);
        return false;
    }
}

