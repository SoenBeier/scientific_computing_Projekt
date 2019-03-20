// FEHLER BEHEBEN : T BESITT KEINEN WERT F�RS STEHEN BLEIBEN; DA PERs
#ifndef initial_situation
#define initial_situation
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

using namespace std;

/*
Erkl�rung der Parameter:
grid_height: Gibt die H�he des Feldes wieder, in dem die Personen simuliert werden
grid_width: Gibt die Breite des Feldes wieder, in dem die Personen simuliert werden
max_number_of_iterations: Gibt die maximale Anzahl der Simulationsschritte an, die durchlaufen werden
iteration_break_condition: Gibt an ob die Simulation auch schon vorher gestoppt werden soll, wenn alle Personen ihre Ziele erreicht haben
plant_layout: Gibt den Namen des Bildes an, welches den Anfangssituation der Hindernisse, Personen und Ziele darstellt. Dieses Bild wird im Laufe der Simulation geladen
movement_update: Gibt an ob sich die Personen parallel oder hintereinander Bewegen sollen. Verschiedene Effekte sind nur mit der parallelen Methode m�glich (z.B. das Ausbrechen von Panik)
grafic_delay: Gibt an wie schnell die Iterationsschritte abgearbeitet werden [in ms]

Wie setze ich die Variablen richtig?
Schritt:
1 - plant_layout w�hlen; Ist die Datei nicht vorhanden, so wird eine Fehlermeldung beim Ausf�hren angezeigt.
2 - grid_heigth und grid_width an die Situation anpassen; Damit die Simulation richtig funtktioniert muss der gesammte Grundriss im von der H�he und der Breite gesetzten Rechteck liegen

Die restlichen Optionen k�nnen nach Belieben eingestellt werden und werden zu keinem Fehler in der Simulation f�hren.
*/


const static int grid_height = 33;
const static int grid_width = 33;

static int max_number_of_iterations = 6000;
static bool iteration_break_condition = true; //kann das Program auch vorher schon abbrechen(wenn alle Personen im Ziel sind)?

static const char plant_layout[] = "25x25_Haus.bmp";//Name des Geb�udeplans
static const char movement_update = 's'; //'s' - sequential, 'p' - parallel

static int grafic_delay = 10;// Je h�her, desto langsamer aktuallisiert sich die grafische Anzeige




/*
Erkl�rung zur Benutzung des Analysedurchlaufs:
Wenn die Daten des Simulationslaufs gespeichert werden sollen, so muss "execute" aktiviert sein.

Wenn die Parameter der Simulation so ver�ndert werden sollen, dass die unten aufgef�hrten Werte von jedem Objekt angenommen werden,
so muss "execute" aktiviert sein und der jeweilige Parameter muss positiv sein. Alle so gew�hlten Parameter werden immer in allen Objekten ver�ndert.
Einzelne Zuweisungen sind also nicht m�glich.

Wenn das Programm �ber die Shell aufgerufen werden soll, bzw. der Aufruf durch die vorher programmierte batch Datei(auf einem Windows Rechner)
stammt, so muss der Parameter foreign_call aktiviert sein. Die angegebenen Parameter werden dann ignoriert und die Eingabe der Parameter erfolgt �ber
die Batch Datei, bzw. den Aufruf.
Die Parameter bei einem solchen Aufruf sind der Reihe nach: k_S, k_D, w_S, friction.

Ist f�lschlicherweise der Paramter "foreign_call" aktiviert wird das Programm abst�rzen.



Erkl�rung der Effekte der einzelnen Paramter auf die Bewegung einer Person:
k_S: Einfluss des statischen Feldes auf die Bewegungen der Personen
k_D: Einfluss des dynamischen Feldes auf die Bewegungen der Personen
w_S: Wissensstand der Personen �ber die einzelnen Ausg�nge (wenn der Einfluss vom statischen Feld nicht beeinflusst werden soll, sollte w_S = 1 gew�hlt sein !!!
friction: Gibt die Wahrscheinlichkeit an, dass sich eine Person in einem Iterationsschritt nicht bewegt, obwohl sie es d�rfte
alpha:
delta:
*/


 struct analysis_run{
    bool execute = true; //wenn true: Werte in dieser Strukturen werden dann an die Objekte �bergeben und die Abfrage an den Benutzer entfallen
    bool foreign_call = true; //experimentell; Werte werden mit der Konsole hinzugef�gt, dies kann f�r die Analyse benutzt werden
    // wird hier ein negativer eintrag gew�hlt, so wird dieser Parameter nicht gesetzt
    double k_S = 1; //Einfluss von s auf die Bewegung der Personen
    double k_D = -1; //Einfluss von D auf die Bewegung der Personen
    double w_S = 1; //Wissen der Personen �ber die Ausg�nge
    double friction = 0;
    double alpha = -1; // Ausbreiten von D
    double delta = -1; // Verschwinden von D
};

#endif




