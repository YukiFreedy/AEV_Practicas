#include <nds.h>
#include <stdlib.h>

touchPosition touch;
int color;

void ClearScreen(void)
{
	int i;
	for(i = 0; i < 256 * 192; i++)
		VRAM_A[i] = RGB15(255,255,255);
}

int main(void)
{
	videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);
	lcdMainOnBottom();
	ClearScreen();
	color = rand();
	while(1)
	{
		swiWaitForVBlank();
		scanKeys();
		if(keysHeld() & KEY_TOUCH)
		{	
			touchRead(&touch);
			VRAM_A[touch.px + touch.py * SCREEN_WIDTH] = color;
		}
		if(keysHeld() & KEY_B)
		{
			ClearScreen();
		}
		if(keysHeld() & KEY_A)
		{
			color = rand();
		}
	}
	return 0;
}