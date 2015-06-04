#ifndef MOUSEEVENTS_H_INCLUDED
#define MOUSEEVENTS_H_INCLUDED
#include "SDL/SDL.h"

enum MOUSESTATUS {MOUSEOUT, MOUSEOVER, MOUSEDOWN, MOUSECLICK};

class RECTBOX {
private:
    bool mouseDown_;
protected:
    SDL_Rect box_;
public:
    RECTBOX(int w = 0, int h = 0, int x = 0, int y = 0);
    void setPosition(int x, int y);
    void setSize(int w, int h);
    virtual MOUSESTATUS handleEvents(const SDL_Event *event);
};

#endif // MOUSEEVENTS_H_INCLUDED
