#include <nds.h>
#include "ball.h"

#include <maxmod9.h>

#include "soundbank.h"
#include "soundbank_bin.h"

#define c_radius (8<<8)

#define c_gravity           40
#define c_air_friction      1
#define c_ground_friction   30
#define c_platform_level    ((192-48) << 8)
#define c_bounce_damper     20
#define c_radius            (8<<8)
#define c_diam              16
#define min_height          (1200)
#define min_yvel            (1200)
#define max_xvel            (1000<<4)

static inline int clampint( int value, int low, int high )
{
    if( value < low ) value = low;
    if( value > high) value = high;
    return value;
}

void ballUpdate( ball* b, mm_sound_effect* ambulance )
{
    b->x += (b->xvel>>4);
    b->xvel = (b->xvel * (256-c_air_friction)) >> 8;
    b->xvel = clampint( b->xvel, -max_xvel, max_xvel );
    
    b->yvel += c_gravity;
    
    b->y += (b->yvel);
    
    if( b->y + c_radius >= c_platform_level ) {
        b->xvel = (b->xvel * (256-c_ground_friction)) >> 8;
        if( b->y > c_platform_level - min_height )
        {
            b->y = c_platform_level - c_radius;
            b->yvel = -(b->yvel * (256-c_bounce_damper)) >> 8;
            
            if( b-> yvel > -min_yvel )
                b->yvel = -min_yvel;
        }
        b->height = (c_platform_level - b->y) * 2;
    }
    else
    {
        mmEffectEx(ambulance);
        b->height = c_diam << 8;
    }
}

void ballRender( ball* b, int camera_x, int camera_y)
{
    u16* sprite = OAM + b->sprite_index * 4;
    int x, y;
    x = ((b->x - c_radius * 2) >> 8) - camera_x;
    y = ((b->y - c_radius * 2) >> 8) - camera_y;
    
    sprite[0] = (y & 255) | ATTR0_ROTSCALE_DOUBLE;
    sprite[1] = (x & 511) | ATTR1_SIZE_16 | ATTR1_ROTDATA( b->sprite_affine_index );
    
    u16* affine;
    affine = OAM + b->sprite_affine_index * 16 + 3;
    affine[4] = 0;
    affine[8] = 0;
    int pa = (b->height * (65536/c_diam)) >> 16;
    int pd = 65536 / pa;
    affine[0] = pa;
    affine[12] = pd;
}
