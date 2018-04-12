#include <nds.h>
#include <fat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


touchPosition touch;
int color;
int r, g, b;

int paintedR[256][192];
int paintedG[256][192];
int paintedB[256][192];

void ClearScreen(void)
{
	int i;
	for(i = 0; i < 256 * 192; i++){
		VRAM_A[i] = RGB15(255,255,255);
	}int j;
	for(i = 0; i < 256; i++){
		for(j = 0; j < 192; j++){
			paintedR[i][j] = 255;
			paintedG[i][j] = 255;
			paintedB[i][j] = 255;
		}
	}
}

void assignRandomColor(void){
	r = rand() % 255;
	g = rand() % 255;
	b = rand() % 255;
	color = RGB15(r, g, b);
}

int main(void)
{
	videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);
	lcdMainOnBottom();
	ClearScreen();
	assignRandomColor();
	while(1)
	{
		swiWaitForVBlank();
		scanKeys();
		if(keysHeld() & KEY_TOUCH)
		{	
			touchRead(&touch);
			VRAM_A[touch.px + touch.py * SCREEN_WIDTH] = color;
			paintedR[touch.px][touch.py] = r;
			paintedG[touch.px][touch.py] = g;
			paintedB[touch.px][touch.py] = b;
		}
		if(keysHeld() & KEY_B)
		{
			ClearScreen();
		}
		if(keysHeld() & KEY_A)
		{
			assignRandomColor();
		}
		if(keysHeld() & KEY_START)
		{
			FILE *fp;
			u8 c;
		
			lcdMainOnTop();
			consoleDemoInit();

			if (!fatInitDefault()) 
				printf("Fail");
			else{
				fp = fopen("fat:/test.txt", "wb");
				if(!fp)
					printf("Escribint fat:/test.txt: Error!\n");
				else
				{
					fprintf(fp, "%s\n", "P3" );
					fprintf(fp, "%s\n", "256 192");
					fprintf(fp, "%s\n", "255");
					int i; int j;
					for(i = 0; i < 256; i++){
						for(j = 0; j < 192; j++){
							fprintf(fp, "%d ", paintedR[i][j]);
							fprintf(fp, "%d ", paintedG[i][j]);
							fprintf(fp, "%d ", paintedB[i][j]);
						}
						fprintf(fp, "\n");
					}
					printf("\n");
					fclose(fp);
					fp = fopen("fat:/test.txt", "r");
					
				}
			}
		}
	}
	return 0;
}