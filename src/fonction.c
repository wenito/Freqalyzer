/**
* \file fonction.c
* \brief Définition des fonction de freqalyzerv1.c
* \author Wenceslas MARIE SAINTE
* \version bêta
* \date 02 février 2015
*
*/

#include <fmod/fmod.h>
#include <fmod/fmod_errors.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "constantes.h"

/**
* \fn W_GestionVolume(float *volume, FMOD_CHANNEL *channel, int vol_action)
* \brief Fonction de gestion du volume sonore
*
* \param[in, out] volume Pointeur sur une variable permettant de gérer le volume sonore
* \param[in, out] channel Pointeur sur un le canal du fichier son
* \param[in] vol_action variable permettant de gérer l'action sur le volume sonore
* \return Etat d'execution du programme
*/

int W_GestionVolume(float *volume, FMOD_CHANNEL *channel, int vol_action)
{
	switch(vol_action)
	{
		case UP :
			printf("\nLa valeur du volume est : %f", *volume);
			if(*volume < 1)
				*volume += 0.1;
			else if(*volume > 1)
				*volume = 1;
			FMOD_Channel_SetVolume(channel, *volume);
			break;
		case DOWN :
			printf("\nLa valeur du volume est : %f", *volume);
			if(*volume > 0)
				*volume -= 0.1;
			else if(*volume < 0)
				*volume = 0;
			FMOD_Channel_SetVolume(channel, *volume);
			break;
		default:
			printf("\nProblème de gestion du volume");
			exit(EXIT_FAILURE);
			break;
	}
	return EXIT_SUCCESS;
}

/**
* \fn int W_GestionErreur(int sujet,int valeur, SDL_Surface *surface, const char *titre_im)
* \brief Gestion des erreurs
*
* \param[in] sujet variable permettant de définir l'erreur à vérifier
* \param[in] valeur variable retour de fonctions diverses
* \param[in] Pointeur sur structure de type SDL_Surface
* \param[in] Chaîne de caractère
* \return Etat d'execution du programme
*/

int W_GestionErreur(int sujet,int valeur, SDL_Surface *surface, const char *titre_im)
{
	switch(sujet)
	{
		case P_IMAGE:
			if (surface == NULL)
			{
				printf("\nErreur lors du chargement de l'image %s\n", titre_im);
				exit(EXIT_FAILURE);
			}
			else
				return EXIT_SUCCESS;
			break;
		case P_LECTURE:
			if(valeur != 0)
			{
				printf("\nErreur lors de la lecture\n");
				exit(EXIT_FAILURE);
			}else
				return EXIT_SUCCESS;
			break;
		case P_CREASON:
			if(valeur != FMOD_OK)
			{
				printf("\nErreur lors de l'allocation mémoire du fichier audio\n");
				exit(EXIT_FAILURE);
			}else
				return EXIT_SUCCESS;
			break;
		case P_CHARGSON:
			if(valeur != FMOD_OK)
			{
				printf("\nErreur lors du chargement du son\n");
				exit(EXIT_FAILURE);
			}else
				return EXIT_SUCCESS;
			break;
		default:
			break;
	}
}

//Chargement de l'image de fond
int W_LoadSurface(SDL_Surface *fond, SDL_Surface *play, SDL_Surface *pause, SDL_Surface *stop, SDL_Surface *volup, SDL_Surface *voldo, SDL_Surface *iplay, SDL_Surface *ipause, SDL_Surface *istop, SDL_Surface *ivolup, SDL_Surface *ivoldo)
{
	fond = IMG_Load("../pictures/fond.jpg");
	play = IMG_Load("../pictures/play.jpg");
	pause = IMG_Load("../pictures/pause.jpg");
	stop = IMG_Load("../pictures/stop.jpg");
	volup = IMG_Load("../pictures/volume_up.jpg");
	voldo = IMG_Load("../pictures/volume_down.jpg");
	iplay = IMG_Load("../pictures/iplay.jpg");
	ipause = IMG_Load("../pictures/ipause.jpg");
	istop = IMG_Load("../pictures/istop.jpg");
	ivolup = IMG_Load("../pictures/ivolume_up.jpg");
	ivoldo = IMG_Load("../pictures/ivolume_down.jpg");

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

