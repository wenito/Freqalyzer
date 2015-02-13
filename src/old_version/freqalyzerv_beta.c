#include <fmod/fmod.h>
#include <fmod/fmod_errors.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define CONTROLLER_SIZE		30

//Fonction principale
int main(int argc, char *argv[])
{
	int att = 1;
	int size;
	float volume;

	SDL_Event event;
	SDL_Surface *ecran = NULL, *fond = NULL, *play = NULL, *stop = NULL, *pause = NULL, *volumeup = NULL, *volumedown = NULL, *spectrum = NULL;

	SDL_Rect pos_fond, pos_spect, pos_play, pos_pause, pos_stop, pos_volumeup, pos_volumedown;

	printf("Démarrage de Freqalyzer\n");

	//Déclaratin des variables FMOD

	FMOD_SYSTEM *system;
	FMOD_SOUND *sound;
	FMOD_CHANNEL *channel = 0;
	FMOD_RESULT result;
	FMOD_BOOL state;
	void *extradriverdata = 0;

	pos_fond.x = 0;
	pos_fond.y = 0;
	pos_spect.x = 100;
	pos_spect.y = 150;
	pos_play.x = 100;
	pos_play.y = 130;
	pos_pause.x = 100;
	pos_pause.y = 130 + CONTROLLER_SIZE;
	pos_stop.x = 100;
	pos_stop.y = 130 + (CONTROLLER_SIZE * 2);
	pos_volumeup.x = 100;
	pos_volumeup.y = 130 + (CONTROLLER_SIZE * 3);
	pos_volumedown.x = 100;
	pos_volumedown.y = 130 + (CONTROLLER_SIZE * 4);

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

	//Titrage de la fenêtre
	SDL_WM_SetCaption("Freqalyzer", NULL);

	//Chargement de l'image de fond
	fond = IMG_Load("../pictures/fond.jpg");
	if (fond == NULL)
		exit(EXIT_FAILURE);
	play = IMG_Load("../pictures/play.png");
	if (fond == stop)
		exit(EXIT_FAILURE);
//	pause = IMG_Load("../pictures/pause.png");
//	stop = IMG_Load("../pictures/stop.png");
//	volumeup = IMG_Load("../pictures/volumeup.png");
//	volumedown = IMG_Load("../pictures/volumedown.png");

	//Allocation de mémoire à system
	result = FMOD_System_Create(&system);

	//Initialisation
	result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, extradriverdata);

	//Chargement du son
	result = FMOD_System_CreateSound(system, "../sounds/gameofthrones.mp3", FMOD_2D | FMOD_CREATESTREAM, 0, &sound);

	while(att)
	{
		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

		SDL_PollEvent(&event);
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
 						FMOD_Channel_GetVolume(channel, &volume);
						FMOD_Channel_SetVolume(channel, volume);
						unsigned int length;
						FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_MS);
						//Récupération du pointeur sur le canal
						//FMOD_System_GetChannel(system, 0, &channel);
						break;
					case SDLK_UP:
						printf("\nLa valeur du volume est : %f", volume);
						if(volume < 1)
							volume += 0.1;
						else if(volume > 1)
							volume = 1;
						FMOD_Channel_SetVolume(channel, volume);
						break;
					case SDLK_DOWN:
						printf("\nLa valeur du volume est : %f", volume);
						if(volume > 0)
							volume -= 0.1;
						else if(volume < 0)
							volume = 0;
						FMOD_Channel_SetVolume(channel, volume);
						break;
					case SDLK_s:
 						FMOD_Channel_Stop(channel);
						break;
					case SDLK_p:
						FMOD_Channel_GetPaused(channel, &state);
						printf("La pause est : %d", state);
						if(state)
						{
							SDL_Delay(250);
							FMOD_Channel_SetPaused(channel, 0);
						}
						else
						{
							SDL_Delay(250);
							FMOD_Channel_SetPaused(channel, 1);
						}
						break;
					default:
						break;
					}
			default:
				break;
		}
		//Effacement de la surface du spectre avant affichage
		SDL_FillRect(spectrum, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

		//Affichage des surfaces
		SDL_BlitSurface(fond, NULL, ecran, &pos_fond);
		SDL_BlitSurface(spectrum, NULL, ecran, &pos_spect);
		SDL_BlitSurface(play, NULL, ecran, &pos_play);
		SDL_BlitSurface(pause, NULL, ecran, &pos_pause);
		SDL_BlitSurface(stop, NULL, ecran, &pos_stop);
		SDL_BlitSurface(volumeup, NULL, ecran, &pos_volumeup);
		SDL_BlitSurface(volumedown, NULL, ecran, &pos_volumedown);

		//Rafraîchissement de la fenêtre
		SDL_Flip(ecran);
		FMOD_System_Update(system);

	}

	//Fermeture et libération de l'objet system en mémoire
	FMOD_Sound_Release(sound);
	FMOD_System_Close(system);
	FMOD_System_Release(system);

	SDL_FreeSurface(play);
	SDL_FreeSurface(pause);
	SDL_FreeSurface(stop);
	SDL_FreeSurface(volumeup);
	SDL_FreeSurface(volumedown);
	SDL_FreeSurface(spectrum);
	SDL_FreeSurface(fond);
	SDL_Quit();

	return EXIT_SUCCESS;
}
