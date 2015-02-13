#include <fmod/fmod.h>
#include <fmod/fmod_errors.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

int main(int argc, char **argv)
{
	printf("It works fine :D !\n");

	FMOD_SYSTEM *system;
	FMOD_SOUND *sound;
	FMOD_CHANNEL *channel=0;
	FMOD_RESULT result;
	void *extradriverdata = 0;

	result=FMOD_System_Create(&system);

	result=FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, extradriverdata);

	result=FMOD_System_CreateSound(system, "../sounds/pacman.wav", FMOD_DEFAULT, 0, &sound);

	//jouer le son et mettre en pause le programme le temps de sa lecture
	result=FMOD_System_PlaySound(system, sound, 0, 0, &channel);
	unsigned int length;
	FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_MS);
	usleep(length*1000);

	FMOD_System_Update(system);
	FMOD_Sound_Release(sound);
	FMOD_System_Close(system);
	FMOD_System_Release(system);

	return EXIT_SUCCESS;
}
