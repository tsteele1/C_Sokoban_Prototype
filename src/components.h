#ifndef COMPONENTS_H
#define COMPONENTS_H

#define MAX_ANIMS 4
#define MAX_ANIM_FRAMES 4

#define WALK_TIME 0.125
#define BUMP_TIME 0.250

#define BASE_HOLE_X 11
#define BASE_HOLE_Y 0

struct Frame {
    float atlasX;
    float atlasY;
};


enum AnimType {
    DOWN,
    UP,
    LEFT,
    RIGHT
};

struct Animator {
    Frame animations[MAX_ANIMS][MAX_ANIM_FRAMES];
    int framesPerAnim[MAX_ANIMS];
    int fpsForAnim[MAX_ANIMS];
    int currentNumAnims;
    int animIdx;
    int frameIdx;
    float animTime;
};

enum MoveAnimType {
    WALK,
    BUMP
};

struct Renderer {
    float spriteWidth;
    float spriteHeight;
    float atlasX;
    float atlasY;
    float spriteOffsetX;
    float spriteOffsetY;
    float savedOffsetX;
    float savedOffsetY;
    float moveAnimTime;
    float maxMoveAnimTime;
    MoveAnimType moveAnimType;
};

struct Pushable {
    bool isMoving;
};

enum BoxType {
    WOODEN,
    RED_METAL,
    BLUE_METAL,
    GREEN_METAL,
    GREY_METAL,
    NO_BOX
};

struct Goal {
    BoxType requiredBox;    
};

struct Box {
    BoxType type;
};

struct Hole {
    BoxType storedType;
    int boxDirX;
    int boxDirY;
    int boxId;
    float baseAtlasX;
    float baseAtlasY;
};
#endif
