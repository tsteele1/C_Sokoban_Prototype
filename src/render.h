#ifndef RENDER_H
#define RENDER_H
#include "ecs.h"
#include "raylib.h"

// Depending on implementation, you may want to set this up so that
// instead of providing a texture, it provides a list of textures wrapped
// in a struct or something, and then give each render component a way
// to know which texture it wants to pull from that struct.
//
// For this sokoban game however, all our textures are on one atlas, so
// we have the freedom to only pass in the one atlas and call it a day.
void renderDrawSprites(Texture *atlas, ECS *ecs);
void renderUpdateOffsets(ECS *ecs);
void renderUpdateAnimations(ECS *ecs);

#endif
