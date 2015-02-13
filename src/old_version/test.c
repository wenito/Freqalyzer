#include <fmod/fmod.h>
#include <fmod/fmod_errors.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

int main(int argc, char *argv[])
{
	int att = 1;

	SDL_Event event;
	SDL_Surface *ecran = NULL, *fond = NULL, *spectrum = NULL;
	SDL_Rect pos_fond, pos_spect;

	printf("Démarrage de Freqalyzer\n");

	FMOD_SYSTEM *system;
	FMOD_SOUND *sound;
	FMOD_CHANNEL *channel=0;
	FMOD_RESULT result;
	FMOD_BOOL state;
	void *extradriverdata = 0;

	pos_fond.x = 0;
	pos_fond.y = 0;
	pos_spect.x = 100;
	pos_spect.y = 150;

	//Chargement en mémoire du système d'affichage SDL - Vidéo
	SDL_Init(SDL_INIT_VIDEO);

	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de la SDL");
		exit(EXIT_FAILURE);
	}

	//Paramétrage et ouverture de la fenêtre
	ecran =  SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

	spectrum = SDL_CreateRGBSurface(SDL_HWSURFACE, 600, 350, 32, 0, 0, 0, 0);
	SDL_FillRect(spectrum, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

	//Titrage de la fenêtre
	SDL_WM_SetCaption("Freqalyzer", NULL);

	//Chargement de l'image de fond
	fond = IMG_Load("../pictures/fond.jpg");

	//Allocation de mémoire à system
	result = FMOD_System_Create(&system);

	//Initialisation
	result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, extradriverdata);

	//Chargement du son
	result = FMOD_System_CreateSound(system, "../sounds/gardenseanhayes.mp3", FMOD_2D | FMOD_CREATESTREAM, 0, &sound);

	while(att)
	{
		SDL_WaitEvent(&event);

		switch(event.type)
		{
			case SDL_QUIT:
				att = 0;
				break;
			case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
 						att = 0;
						break;
					case SDLK_SPACE:
						//jouer le son et mettre en pause le programme le temps de sa lecture
						result=FMOD_System_PlaySound(system, sound, 0, 0, &channel);
						unsigned int length;
						FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_MS);
						break;
					case SDLK_p:
						FMOD_System_GetChannel(system, 512, &channel);
						FMOD_Channel_GetPaused(channel, &state);
						if(state)
							FMOD_Channel_SetPaused(channel, 0);
						else
							FMOD_Channel_SetPaused(channel, 1);
						break;
					default:
						break;
					}
			default:
				break;
		}
		//Affichage des surfaces
		SDL_BlitSurface(fond, NULL, ecran, &pos_fond);
		SDL_BlitSurface(spectrum, NULL, ecran, &pos_spect);

		//Rafraîchissement de la fenêtre
		SDL_Flip(ecran);

	}

	FMOD_System_Update(system);

	//Fermeture et libération de l'objet system en mémoire
	FMOD_Sound_Release(sound);
	FMOD_System_Close(system);
	FMOD_System_Release(system);

	SDL_FreeSurface(spectrum);
	SDL_FreeSurface(fond);
	SDL_Quit();

	return EXIT_SUCCESS;
}
