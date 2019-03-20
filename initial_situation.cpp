// FEHLER BEHEBEN : T BESITT KEINEN WERT F�RS STEHEN BLEIBEN; DA PERs
#ifndef initial_situation
#define initial_situation
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

using namespace std;

const static int grid_height = 100;
const static int grid_width = 100;

static int number_of_iterations = 50;

static const char plant_layout[] = "einzel_Haus.bmp";//Name des Geb�udeplans
static const char movement_update = 's'; //'s' - sequential, 'p' - parallel

static int decay_param=0; //Zerfallsparameter f�rs dynamische Feld [0,100]
static int diffusion_param=80; //Verteilungsparameter f�rs dynamische Feld [0,100]

static int grafic_delay = 20;// Je h�her, desto langsamer aktuallisiert sich die grafische Anzeige

static const bool analysis_run = true;

#endif




