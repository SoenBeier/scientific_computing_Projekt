// FEHLER BEHEBEN : T BESITT KEINEN WERT FÜRS STEHEN BLEIBEN; DA PERs
#ifndef initial_situation
#define initial_situation
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

using namespace std;

const static int grid_height = 30;
const static int grid_width = 30;

static int number_of_iterations = 200;

static const char plant_layout[] = "10x10_Haus+Hin.bmp";//Name des Gebäudeplans
static const char movement_update = 's'; //'s' - sequential, 'p' - parallel

static int grafic_delay = 20;// Je höher, desto langsamer aktuallisiert sich die grafische Anzeige

static const bool analysis_run = true;

#endif




