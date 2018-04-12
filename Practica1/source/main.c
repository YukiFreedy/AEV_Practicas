#include <nds.h>

#include "gfx_ball.h"
#include "gfx_brick.h"
#include "gfx_gradient.h"

#define tile_empty      0
#define tile_brick      1
#define tile_gradient   2

#define pal_bricks      0
#define pal_gradient    1

#define backdrop_colour RGB8( 190, 255, 255 )
#define pal2bgram(p)    (BG_PALETTE + (p) * 16)

#define tile2bgram(t) (BG_GFX + (t) * 16)

#define bg0map        ((u16*) BG_MAP_RAM(1))
#define bg1map        ((u16*) BG_MAP_RAM(2))

#define tiles_ball      0
#define tile2objram(t)  (SPRITE_GFX + (t) * 16)

#define pal2objram(p)   (SPRITE_PALETTE + (p) * 16)
#define pal_ball        0

#include "ball.h"

ball g_ball;

typedef struct t_spriteEntry
{
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 affine_data;
} spriteEntry;

#define sprites ((spriteEntry*)OAM)

#define x_tweak         (2<<8)
#define y_tweak         25

int g_camera_x;
int g_camera_y;

void setupGraphics( void )
{
    vramSetBankE( VRAM_E_MAIN_BG );
    vramSetBankF( VRAM_F_MAIN_SPRITE );
    
    int n;
    for( n = 0; n < 16; n++)
        BG_GFX[n] = 0;
    
    dmaCopyHalfWords( 3, gfx_brickTiles, tile2bgram( tile_brick ), gfx_brickTilesLen );
    dmaCopyHalfWords( 3, gfx_gradientTiles, tile2bgram( tile_gradient ), gfx_gradientTilesLen );
    
    dmaCopyHalfWords( 3, gfx_brickPal, pal2bgram(pal_bricks), gfx_brickPalLen );
    dmaCopyHalfWords( 3, gfx_gradientPal, pal2bgram(pal_gradient), gfx_gradientPalLen );
    
    BG_PALETTE[0] = backdrop_colour;
    
    REG_BG0CNT = BG_MAP_BASE(1);
    REG_BG1CNT = BG_MAP_BASE(2);
    
    for( n = 0; n < 1024; n++ )
        bg0map[n] = 0;
    
    int x, y;
    for( x = 0; x < 32; x++ )
    {
        for ( y = 0; y < 6; y++ )
        {
            int hflip = (x & 1) ^ (y & 1);
            
            bg0map[x + y * 32] = tile_brick | (hflip << 10) | (pal_bricks << 12);
        }
    }
    
    for( x = 0; x < 32; x++ )
    {
        for ( y = 0; y < 8; y++ )
        {
            int tile = tile_gradient + y;
            bg1map[ x + y * 32] = tile | (pal_gradient << 12);
        }
    }
     
    REG_BG0VOFS = 112;
    
    for( n = 0; n < 1024; n++ )
        bg1map[n] = 0;
    
    REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG1 | BLEND_DST_BACKDROP;
    REG_BLDALPHA = (4) + (16<<8);
    
    dmaCopyHalfWords( 3, gfx_ballTiles, tile2objram(tiles_ball), gfx_ballTilesLen );
    dmaCopyHalfWords( 3, gfx_ballPal, pal2objram(pal_ball), gfx_ballPalLen );
    
        
    for( n = 0; n < 128; n++ )
        sprites[n].attr0 = ATTR0_DISABLED;
    /*
    for( n = 0; n < 50; n++)
    {
        sprites[n].attr0 = rand() % (192 - 16);
        sprites[n].attr1 = (rand() % (256 - 16)) + ATTR1_SIZE_16;
        sprites[n].attr2 = tiles_ball + (pal_ball << 12);
    }
    */
    
    videoSetMode( MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D_LAYOUT);
    
}

void setupInterrupts( void )
{
    irqInit();
    irqEnable( IRQ_VBLANK );
}

void resetBall( void )
{
    g_ball.sprite_index = 0;
    g_ball.sprite_affine_index = 0;
    g_ball.x = 128 << 8;
    g_ball.y = 64 << 8;
    g_ball.xvel = 100 << 4;
    g_ball.yvel = 0;
}

void updateGraphics( void )
{
    ballRender( &g_ball, 0, 0 );
    
    REG_BG0HOFS = g_camera_x >> 8;
}


void processInput( void )
{
    scanKeys();
    int keysh = keysHeld();
    if( keysh & KEY_UP )
    {
        g_ball.yvel -= y_tweak;
    }
    if( keysh & KEY_DOWN )
    {
        g_ball.yvel += y_tweak;
    }
    if( keysh & KEY_LEFT )
    {
        g_ball.xvel -= x_tweak;
    }
    if( keysh & KEY_RIGHT )
    {
        g_ball.xvel += x_tweak;
    }
}

void updateCamera( void )
{
    int cx = ((g_ball.x)) - (128 << 8);
    int dx;
    dx = cx - g_camera_x;
    if( dx > 10 || dx < -10 )
        dx = (dx * 50) >> 10;
    
    g_camera_x += dx;
    g_camera_y = 0;
}

void processLogic( void )
{
    processInput();
    ballUpdate( &g_ball );
    updateCamera();
}

int main( void )
{
    setupInterrupts();
    setupGraphics();
    resetBall();
    
    while(1){
        
        processLogic();
        
        swiWaitForVBlank();     
        
        updateGraphics();
        
    
        }
    return 0;
}



