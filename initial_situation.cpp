// FEHLER BEHEBEN : T BESITT KEINEN WERT FÜRS STEHEN BLEIBEN; DA PERs
#ifndef initial_situation
#define initial_situation
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

using namespace std;

/*
Erklärung der Parameter:
grid_height: Gibt die Höhe des Feldes wieder, in dem die Personen simuliert werden
grid_width: Gibt die Breite des Feldes wieder, in dem die Personen simuliert werden
max_number_of_iterations: Gibt die maximale Anzahl der Simulationsschritte an, die durchlaufen werden
iteration_break_condition: Gibt an ob die Simulation auch schon vorher gestoppt werden soll, wenn alle Personen ihre Ziele erreicht haben
plant_layout: Gibt den Namen des Bildes an, welches den Anfangssituation der Hindernisse, Personen und Ziele darstellt. Dieses Bild wird im Laufe der Simulation geladen
movement_update: Gibt an ob sich die Personen parallel oder hintereinander Bewegen sollen. Verschiedene Effekte sind nur mit der parallelen Methode möglich (z.B. das Ausbrechen von Panik)
grafic_delay: Gibt an wie schnell die Iterationsschritte abgearbeitet werden [in ms]



Wie setze ich die Variablen richtig?
Schritt:
1 - plant_layout wählen; Ist die Datei nicht vorhanden, so wird eine Fehlermeldung beim Ausführen angezeigt.
2 - grid_heigth und grid_width an die Situation anpassen; Damit die Simulation richtig funtktioniert muss der gesammte Grundriss im von der Höhe und der Breite gesetzten Rechteck liegen

Die restlichen Optionen können nach Belieben eingestellt werden und werden zu keinem Fehler in der Simulation führen.
*/


const static int grid_height = 35;
const static int grid_width = 260;


static int max_number_of_iterations = 1000;
static bool iteration_break_condition = true; //kann das Program auch vorher schon abbrechen(wenn alle Personen im Ziel sind)?

static const char plant_layout[] = "Korridor_ohne_Hin.bmp";//Name des Gebäudeplans

static const char movement_update = 'p'; //'s' - sequential, 'p' - parallel
//BEIM PARALLELEN NOCHMAL NACHSCHAUEN: C[][] WIRD WIRKLICH RICHTIG GEWÄHLT ?? was hat es mit den einsen in der Matrix zu tun?

static int grafic_delay = 0;// Je höher, desto langsamer aktuallisiert sich die grafische Anzeige

static int decay_param = 35; //Zerfallsparameter fürs dynamische Feld [0,100]
static int diffusion_param = 35; //Verteilungsparameter fürs dynamische Feld [0,100] ERZEUGT FEHLER BEIM AUSFÜHREN!

/*
Veränderungen am Ablauf des Programms, wenn "reject_other_D_fields" aktiviert ist:
- die Personen erhalten nur für exakt einen Ausgang den Wissensstand 1 (Ueberschreibung der Wissensstände in Funktion set_model_parameter())
    -> jede Person kennt also nur ein Ziel. Die Nummer dieses Ziels wird in der Variable "numb_selected_dest" in der Personenklasse gespeichert
- Das D Feld von einer Person wird von anderen Personen nur erhöht, wenn sich diese in die Richtung bewegen, in die das statische Feld der Person zeigt
*/
static bool corridor_conditions = true; //Korridor muss waagerecht liegen; aktiviert automatisch unite_destinations_if_possible
static bool reject_other_D_fields = true; //(noch nicht eingebaut) Ist für die Simulation für den Korridor nötig, bei dem die Menschen mit unterschiedlichen Zielen das D Feld der Menschen mit einem anderen Ziel abstoßend finden
static bool unite_destinations_if_possible = false; //(nur möglich wenn reject_other_D_fields aktiv ist) Vereinigt Ziele die genau nebeneinanderliegen zu einem Ziel (w_S wird kopiert)
/*
Erklärung zur Benutzung des Analysedurchlaufs:
Wenn die Daten des Simulationslaufs gespeichert werden sollen, so muss "execute" aktiviert sein.

Wenn die Parameter der Simulation so verändert werden sollen, dass die unten aufgeführten Werte von jedem Objekt angenommen werden,
so muss "execute" aktiviert sein und der jeweilige Parameter muss positiv gewählt sein. Alle so gewählten Parameter werden immer in allen Objekten verändert.
Zuweisungen für einzelne Objekte sind also nicht möglich.

Wenn das Programm über die Shell aufgerufen werden soll, bzw. der Aufruf durch die vorher programmierte batch Datei(auf einem Windows Rechner)
stammt, so muss der Parameter foreign_call aktiviert sein. Die angegebenen Parameter werden dann ignoriert und die Eingabe der Parameter erfolgt über
die Batch Datei, bzw. den Aufruf.
Die Parameter bei einem solchen Aufruf sind der Reihe nach: k_S, k_D, w_S, friction.

Ist fälschlicherweise der Paramter "foreign_call" aktiviert wird das Programm abstürzen.



Erklärung der Effekte der einzelnen Paramter auf die Bewegung einer Person:
k_S: Einfluss des statischen Feldes auf die Bewegungen der Personenu
k_D: Einfluss des dynamischen Feldes auf die Bewegungen der Personen
w_S: Wissensstand der Personen über die einzelnen Ausgänge (wenn der Einfluss vom statischen Feld nicht beeinflusst werden soll, sollte w_S = 1 gewählt sein !!!
friction: Gibt die Wahrscheinlichkeit an, dass sich eine Person in einem Iterationsschritt nicht bewegt, obwohl sie es dürfte
alpha:
delta:
*/


 struct analysis_run{
    bool execute = true; //wenn true: Werte in dieser Strukturen werden dann an die Objekte übergeben und die Abfrage an den Benutzer entfallen
    bool foreign_call = false; //experimentell; Werte werden mit der Konsole hinzugefügt, dies kann für die Analyse benutzt werden
    // wird hier ein negativer eintrag gewählt, so wird dieser Parameter nicht gesetzt

    double k_S = 1; //Einfluss von s auf die Bewegung der Personen
    double k_D = 4; //Einfluss von D auf die Bewegung der Personen
    double w_S = -1; //Wissen der Personen über die Ausgänge
    double friction = -1;

};

#endif




