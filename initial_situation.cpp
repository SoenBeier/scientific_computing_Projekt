// FEHLER BEHEBEN : T BESITT KEINEN WERT FÜRS STEHEN BLEIBEN; DA PERs
#ifndef initial_situation
#define initial_situation
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

using namespace std;

const static int grid_height = 33;
const static int grid_width = 33;

static int number_of_iterations = 6000;
static bool iteration_break_condition = true; //kann das Program auch vorher schon abbrechen(wenn alle Personen im Ziel sind)?

static const char plant_layout[] = "25x25_Haus.bmp";//Name des Gebäudeplans
static const char movement_update = 's'; //'s' - sequential, 'p' - parallel

static int grafic_delay = 10;// Je höher, desto langsamer aktuallisiert sich die grafische Anzeige


 struct analysis_run{
    bool execute = true; //wenn true: Werte in dieser Strukturen werden dann an die Objekte übergeben und die Abfrage an den Benutzer entfallen
    bool foreign_call = true; //experimentell; Werte werden mit der Konsole hinzugefügt, dies kann für die Analyse benutzt werden
    // wird hier ein negativer eintrag gewählt, so wird dieser Parameter nicht gesetzt
    double k_S = 1; //Einfluss von s auf die Bewegung der Personen
    double k_D = -1; //Einfluss von D auf die Bewegung der Personen
    double w_S = 1; //Wissen der Personen über die Ausgänge
    double friction = 0;
    double alpha = -1; // Ausbreiten von D
    double delta = -1; // Verschwinden von D
};

#endif




