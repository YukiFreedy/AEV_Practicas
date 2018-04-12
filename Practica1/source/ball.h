#ifndef BALL_H
#define BALL_H

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

void ballUpdate( ball* b );
void ballRender( ball* b, int camera_x, int camera_y );

#endif 
