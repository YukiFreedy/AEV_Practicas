#include <nds.h>

#include <stdio.h>
#include <stdlib.h>

void getUsername(){
    consoleClear();
    char name[11] = {0};
    for(int i = 0; i<PersonalData->nameLen;i++)
        name[i] = (char)(PersonalData->name[i] & 255);
    iprintf(" Username is: %s\n Press \"B\" to return", name);
    scanKeys();
    while(!(keysHeld() & KEY_B))
	{
		scanKeys();
		swiWaitForVBlank();
	}
	consoleClear();
}

void ledBlinking(){
    ledBlink(100);
}
