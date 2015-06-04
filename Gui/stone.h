#ifndef STONE_H_INCLUDED
#define STONE_H_INCLUDED
#include "mouseEvents.h"
#include "display.h"

const int STONE_WIDTH = 37;
const int STONE_HEIGHT = 37;

enum STONE_TYPE {BLACK, WHITE, NONE, WALL};

class Stone : public RECTBOX {
private:
    STONE_TYPE type_;
    bool isMarked_;

    static SDL_Surface *destination_;
    static SDL_Surface *stoneImg_;
    static SDL_Event *srcEvent_;
    static bool pause_;

    void draw(STONE_TYPE type);
public:
    static void setPause(bool pause) {
        pause_ = pause;
    }
    static bool getPause() {
        return pause_;
    }
    static void setImage(SDL_Surface *stoneImg, SDL_Surface *destination) {
        stoneImg_ = stoneImg;
        destination_ = destination;
    }

    static void setEvent(SDL_Event *event) {
        srcEvent_ = event;
    }

    Stone(int x = 0, int y = 0) : RECTBOX(STONE_WIDTH, STONE_HEIGHT, x, y) {
        type_ = NONE;
        isMarked_ = false;
        //type_ = BLACK;
    }

    void setMarked(bool isMarked) {
        isMarked_ = isMarked;
    }
    void setType(STONE_TYPE type) {
        type_ = type;
        //draw(type);
    }
    void display() {
        draw(type_);
    }
    STONE_TYPE getType() {
        return type_;
    }

    bool isClick(STONE_TYPE overType);
};

STONE_TYPE reverseStone(STONE_TYPE stone);
#endif // STONE_H_INCLUDED
