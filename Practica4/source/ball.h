#ifndef BALL_H
#define BALL_H

#include <maxmod9.h>

typedef struct t_ball
{
    int x; 
    int y;
    int xvel;
    int yvel;
    u8 sprite_index;
    u8 sprite_affine_index;
    int height;
} ball;

void ballUpdate( ball* b, mm_sound_effect* ambulance );
void ballRender( ball* b, int camera_x, int camera_y );

#endif 
