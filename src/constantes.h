#define CONTROLLER_SIZE         30

enum {PAUSE, PLAY, STOP};
enum {UP, DOWN};
enum {P_IMAGE, P_CHARGSON, P_LECTURE, P_PAUSE, P_CREASON};

int W_LoadSurface(SDL_Surface *fond, SDL_Surface *play, SDL_Surface *pause, SDL_Surface *stop, SDL_Surface *volup, SDL_Surface *voldo, SDL_Surface *iplay, SDL_Surface *ipause, SDL_Surface *istop, SDL_Surface *ivolup, SDL_Surface *ivoldo); 
void W_event(SDL_Surface *ecran, SDL_Surface *iplay, SDL_Surface *ipause, SDL_Surface *istop, SDL_Surface *ivolup, SDL_Surface *ivoldo);
int W_GestionVolume(float *volume, FMOD_CHANNEL *channel, int vol_action);
int W_GestionErreur(int sujet,int valeur, SDL_Surface *surface, const char *titre_im);
