#include "mouseEvents.h"
RECTBOX::RECTBOX(int w, int h, int x, int y) {
    box_.w = w;
    box_.h = h;
    box_.x = x;
    box_.y = y;
    mouseDown_ = false;
}

void RECTBOX::setPosition(int x, int y) {
    box_.x = x;
    box_.y = y;
}

void RECTBOX::setSize(int w, int h) {
    box_.w = w;
    box_.h = h;
}

MOUSESTATUS RECTBOX::handleEvents(const SDL_Event *event) {
    const int x = event -> motion.x, y = event -> motion.y;

    if((x > box_.x) && (x < box_.x + box_.w) && (y > box_.y) && (y < box_.y + box_.h)) {
        switch (event -> type) {
        case SDL_MOUSEBUTTONDOWN: {
            mouseDown_ = true;
            return MOUSEDOWN;
        }
        break;
        case SDL_MOUSEBUTTONUP: {
            if(mouseDown_) {
                mouseDown_ = false;
                return MOUSECLICK;
            }
        }
        break;
        }
        return MOUSEOVER;
    }
    mouseDown_ = false;
    return MOUSEOUT;
}
