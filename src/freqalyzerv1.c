#include <fmod/fmod.h>
#include <fmod/fmod_errors.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define CONTROLLER_SIZE		30

enum {PAUSE, PLAY, STOP};

int W_LoadSurface(SDL_Surface *fond, SDL_Surface *play, SDL_Surface *pause, SDL_Surface *stop, SDL_Surface *volup, SDL_Surface *voldo, SDL_Surface *iplay, SDL_Surface *ipause, SDL_Surface *istop, SDL_Surface *ivolup, SDL_Surface *ivoldo);
void W_event(SDL_Surface *ecran, SDL_Surface *iplay, SDL_Surface *ipause, SDL_Surface *istop, SDL_Surface *ivolup, SDL_Surface *ivoldo);

int main(int argc, char *argv[])
{
	int att = 1, motion_state = 0, status = STOP;
	float volume;

	SDL_Event event;
	SDL_Surface *ecran = NULL, *fond = NULL, *play = NULL, *stop = NULL, *pause = NULL, *volumeup = NULL, *volumedown = NULL, *spectrum = NULL;
	SDL_Surface *iplay = NULL, *ipause = NULL, *istop = NULL, *ivolup = NULL, *ivoldo = NULL;

	SDL_Rect pos_fond, pos_spect, pos_play, pos_pause, pos_stop, pos_volumeup, pos_volumedown;

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
	pos_play.x = 100;
	pos_play.y = 520;
	pos_pause.x = 100 + CONTROLLER_SIZE;
	pos_pause.y = 520;
	pos_stop.x = 100 + (CONTROLLER_SIZE * 2);
	pos_stop.y = 520;
	pos_volumeup.x = 100 + (CONTROLLER_SIZE * 3);
	pos_volumeup.y = 520;
	pos_volumedown.x = 100 + (CONTROLLER_SIZE * 4);
	pos_volumedown.y = 520;

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
	if (fond == NULL)
		exit(EXIT_FAILURE);
	play = IMG_Load("../pictures/play.jpg");
	if (play == NULL)
		exit(EXIT_FAILURE);
	pause = IMG_Load("../pictures/pause.jpg");
	if (pause == NULL)
		exit(EXIT_FAILURE);
	stop = IMG_Load("../pictures/stop.jpg");
	if (stop == NULL)
		exit(EXIT_FAILURE);
	volumeup = IMG_Load("../pictures/volume_up.jpg");
	if (volumeup == NULL)
		exit(EXIT_FAILURE);
	volumedown = IMG_Load("../pictures/volume_down.jpg");
	if (volumedown == NULL)
		exit(EXIT_FAILURE);
	iplay = IMG_Load("../pictures/iplay.jpg");
	if (iplay == NULL)
		exit(EXIT_FAILURE);
	ipause = IMG_Load("../pictures/ipause.jpg");
	if (ipause == NULL)
		exit(EXIT_FAILURE);
	istop = IMG_Load("../pictures/istop.jpg");
	if (istop == NULL)
		exit(EXIT_FAILURE);
	ivolup = IMG_Load("../pictures/ivolume_up.jpg");
	if (ivolup == NULL)
		exit(EXIT_FAILURE);
	ivoldo = IMG_Load("../pictures/ivolume_down.jpg");
	if (ivoldo == NULL)
		exit(EXIT_FAILURE);

	//Allocation de mémoire à system
	result = FMOD_System_Create(&system);

	//Initialisation
	result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, extradriverdata);

	result = FMOD_System_CreateSound(system, "../sounds/pacman.wav", FMOD_2D | FMOD_CREATESTREAM, 0, &sound);

/*	//Chargement du son
	result = FMOD_System_CreateSound(system, argv[1], FMOD_2D | FMOD_CREATESTREAM, 0, &sound);
	if (result != 0)
	{
		printf("\nErreur de saisie, veuillez taper un chemin correct \n(ex : ./Freqalyser ../sounds/gameofthrones.mp3)\n");
		exit(EXIT_FAILURE);
	}*/

	while(att)
	{
		SDL_WaitEvent(&event);

		if (event.motion.x > 100 && event.motion.x < (100 + CONTROLLER_SIZE * 5) && event.motion.y > 520 && event.motion.y < (520 + CONTROLLER_SIZE))
		{
			motion_state = 1;
			W_event(ecran, iplay, ipause, istop, ivolup, ivoldo);
		}else
			motion_state = 0;

		switch(event.type)
		{
			case SDL_QUIT:
				att = 0;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.x > 100 && event.button.x < (100 + CONTROLLER_SIZE) && event.button.y > 520 && event.button.y < (520 + CONTROLLER_SIZE))
				{
					//jouer le son et mettre en pause le programme le temps de sa lecture
					result=FMOD_System_PlaySound(system, sound, 0, 0, &channel);
					unsigned int length;
					FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_MS);
					FMOD_Channel_GetVolume(channel, &volume);
					FMOD_Channel_GetVolume(channel, &volume);
					status = PLAY;
				}
				if (event.button.x > (100 + CONTROLLER_SIZE) && event.button.x < (100 + (CONTROLLER_SIZE * 2)) && event.button.y > 520 && event.button.y < (520 + CONTROLLER_SIZE))
				{
					FMOD_System_GetChannel(system, 512, &channel);
					FMOD_Channel_GetPaused(channel, &state);
					if(state)
					{
						FMOD_Channel_SetPaused(channel, 0);
						status = PLAY;
					}
					else
					{
						FMOD_Channel_SetPaused(channel, 1);
						status = PAUSE;
					}
				}
				if (event.button.x > (100 + CONTROLLER_SIZE * 2) && event.button.x < (100 + CONTROLLER_SIZE * 3) && event.button.y > 520 && event.button.y < (520 + CONTROLLER_SIZE))
				{
					FMOD_Channel_Stop(channel);
					status = STOP;
				}
				if (event.button.x > (100 + CONTROLLER_SIZE * 3) && event.button.x < (100 + CONTROLLER_SIZE * 4) && event.button.y > 520 && event.button.y < (520 + CONTROLLER_SIZE))
				{
					printf("\nLa valeur du volume est : %f", volume);
					if(volume < 1)
						volume += 0.1;
					else if(volume > 1)
						volume = 1;
					FMOD_Channel_SetVolume(channel, volume);
				}
				if (event.button.x > (100 + CONTROLLER_SIZE * 4) && event.button.x < (100 + CONTROLLER_SIZE * 5) && event.button.y > 520 && event.button.y < (520 + CONTROLLER_SIZE))
				{
					printf("\nLa valeur du volume est : %f", volume);
					if(volume > 0)
						volume -= 0.1;
					else if(volume < 0)
						volume = 0;
					FMOD_Channel_SetVolume(channel, volume);
				}
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
						FMOD_Channel_GetVolume(channel, &volume);
						FMOD_Channel_GetVolume(channel, &volume);
						status = PLAY;
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
					case SDLK_p:
						FMOD_System_GetChannel(system, 512, &channel);
						FMOD_Channel_GetPaused(channel, &state);
						if(state)
						{
							FMOD_Channel_SetPaused(channel, 0);
							status = PLAY;
						}
						else
						{
							FMOD_Channel_SetPaused(channel, 1);
							status = PAUSE;
						}
						break;
					case SDLK_s:
						FMOD_Channel_Stop(channel);
						status = STOP;
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
		switch (status)
		{
			case PLAY:
				SDL_BlitSurface(iplay, NULL, ecran, &pos_play);
				SDL_BlitSurface(pause, NULL, ecran, &pos_pause);
				SDL_BlitSurface(stop, NULL, ecran, &pos_stop);
				break;
			case PAUSE:
				SDL_BlitSurface(play, NULL, ecran, &pos_play);
				SDL_BlitSurface(ipause, NULL, ecran, &pos_pause);
				SDL_BlitSurface(stop, NULL, ecran, &pos_stop);
				break;
			case STOP:
				SDL_BlitSurface(play, NULL, ecran, &pos_play);
				SDL_BlitSurface(pause, NULL, ecran, &pos_pause);
				SDL_BlitSurface(istop, NULL, ecran, &pos_stop);
				break;
			default:
				break;
		}
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

	SDL_FreeSurface(spectrum);
	SDL_FreeSurface(fond);
	SDL_FreeSurface(play);
	SDL_FreeSurface(pause);
	SDL_FreeSurface(stop);
	SDL_FreeSurface(volumeup);
	SDL_FreeSurface(volumedown);
	SDL_FreeSurface(iplay);
	SDL_FreeSurface(ipause);
	SDL_FreeSurface(istop);
	SDL_FreeSurface(ivolup);
	SDL_FreeSurface(ivoldo);

	SDL_Quit();

	return EXIT_SUCCESS;
}

//Chargement de l'image de fond
int W_LoadSurface(SDL_Surface *fond, SDL_Surface *play, SDL_Surface *pause, SDL_Surface *stop, SDL_Surface *volup, SDL_Surface *voldo, SDL_Surface *iplay, SDL_Surface *ipause, SDL_Surface *istop, SDL_Surface *ivolup, SDL_Surface *ivoldo)
{
	fond = IMG_Load("../pictures/fond.jpg");
	if (fond == NULL)
		exit(EXIT_FAILURE);
	play = IMG_Load("../pictures/play.jpg");
	if (play == NULL)
		exit(EXIT_FAILURE);
	pause = IMG_Load("../pictures/pause.jpg");
	if (pause == NULL)
		exit(EXIT_FAILURE);
	stop = IMG_Load("../pictures/stop.jpg");
	if (stop == NULL)
		exit(EXIT_FAILURE);
	volup = IMG_Load("../pictures/volume_up.jpg");
	if (volup == NULL)
		exit(EXIT_FAILURE);
	voldo = IMG_Load("../pictures/volume_down.jpg");
	if (voldo == NULL)
		exit(EXIT_FAILURE);
	iplay = IMG_Load("../pictures/iplay.jpg");
	if (iplay == NULL)
		exit(EXIT_FAILURE);
	ipause = IMG_Load("../pictures/ipause.jpg");
	if (ipause == NULL)
		exit(EXIT_FAILURE);
	istop = IMG_Load("../pictures/istop.jpg");
	if (istop == NULL)
		exit(EXIT_FAILURE);
	ivolup = IMG_Load("../pictures/ivolume_up.jpg");
	if (ivolup == NULL)
		exit(EXIT_FAILURE);
	ivoldo = IMG_Load("../pictures/ivolume_down.jpg");
	if (ivoldo == NULL)
		exit(EXIT_FAILURE);

	return EXIT_SUCCESS;
}


void W_event(SDL_Surface *ecran, SDL_Surface *iplay, SDL_Surface *ipause, SDL_Surface *istop, SDL_Surface *ivolup, SDL_Surface *ivoldo)
{
	SDL_Rect pos_set;
	SDL_Event event;

	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if (event.motion.x > 100 && event.motion.x < (100 + CONTROLLER_SIZE) && event.motion.y > 520 && event.motion.y < (520 + CONTROLLER_SIZE))
			{
				pos_set.x = 100;
				pos_set.y = 520;
				SDL_BlitSurface(iplay, NULL, ecran, &pos_set);
			}
			if (event.motion.x > (100 + CONTROLLER_SIZE) && event.motion.x < (100 + (CONTROLLER_SIZE * 2)) && event.motion.y > 520 && event.motion.y < (520 + CONTROLLER_SIZE))
			{
				pos_set.x = 100 + CONTROLLER_SIZE;
				pos_set.y = 520;
				SDL_BlitSurface(ipause, NULL, ecran, &pos_set);
			}
			if (event.motion.x > (100 + CONTROLLER_SIZE * 2) && event.motion.x < (100 + CONTROLLER_SIZE * 3) && event.motion.y > 520 && event.motion.y < (520 + CONTROLLER_SIZE))
			{
				pos_set.x = 100 + (CONTROLLER_SIZE * 2);
				pos_set.y = 520;
				SDL_BlitSurface(istop, NULL, ecran, &pos_set);
			}
			if (event.motion.x > (100 + CONTROLLER_SIZE * 3) && event.motion.x < (100 + CONTROLLER_SIZE * 4) && event.motion.y > 520 && event.motion.y < (520 + CONTROLLER_SIZE))
			{
				pos_set.x = 100 + (CONTROLLER_SIZE * 3);
				pos_set.y = 520;
				SDL_BlitSurface(ivolup, NULL, ecran, &pos_set);
			}
			if (event.motion.x > (100 + CONTROLLER_SIZE * 4) && event.motion.x < (100 + CONTROLLER_SIZE * 5) && event.motion.y > 520 && event.motion.y < (520 + CONTROLLER_SIZE))
			{
				pos_set.x = 100 + (CONTROLLER_SIZE * 4);
				pos_set.y = 520;
				SDL_BlitSurface(ivoldo, NULL, ecran, &pos_set);
			}
			break;
		default:
			break;
	}

	SDL_Flip(ecran);
}

