#ifndef COMPONENTS_H
#define COMPONENTS_H
#define MAX_ANIMS 4
#define MAX_ANIM_FRAMES 4

struct Frame {
    float atlasX;
    float atlasY;
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

struct Renderer {
    float spriteWidth;
    float spriteHeight;
    float atlasX;
    float atlasY;
    float spriteOffsetX;
    float spriteOffsetY;
    float moveAnimTime;
};

struct Pushable {
    bool isMoving;
};

enum BoxType {
    WOODEN,
    RED_METAL,
    BLUE_METAL,
    GREEN_METAL,
    GREY_METAL
};

struct Goal {
    BoxType requiredBox;    
};

struct Box {
    BoxType type;
};

#endif
