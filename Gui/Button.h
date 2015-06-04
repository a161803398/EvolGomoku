#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED
#include "mouseEvents.h"
#include "display.h"

const int BUTTON_WIDTH = 160;
const int BUTTON_HEIGHT = 48;


class Button : public RECTBOX {
private:
    SDL_Surface *tarImg_;
    bool clickOnce_;
    bool enable_;

    static SDL_Surface *destination_;
    static SDL_Event *srcEvent_;

public:

    static void setDestination(SDL_Surface *destination) {
        destination_ = destination;
    }

    static void setEvent(SDL_Event *event){
        srcEvent_ = event;
    }


    Button(SDL_Surface *tarImg, bool clickOnce = false, int x = 0, int y = 0) : RECTBOX(BUTTON_WIDTH, BUTTON_HEIGHT, x, y) {
        tarImg_ = tarImg;
        clickOnce_ = clickOnce;
        enable_ = true;
    }

    void setEnable(bool enable = true){
        enable_ = enable;
    }

    bool isClick();
};

#endif // BUTTON_H_INCLUDED
