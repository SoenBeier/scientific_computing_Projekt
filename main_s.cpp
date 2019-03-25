#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include "klassen.cpp"
#include <fstream>
#include <time.h>


using namespace std;


//################## initial situation
#include "initial_situation.cpp"
//################## initial situation





//#### Grundriss einlesen
Uint32 getpixel_function(SDL_Surface *surface, int x, int y){ //Quelle: http://sdl.beuc.net/sdl.wiki/Pixel_Access; unter getpixel in SDL Paket enthalten, Liest Farbe eines Pixels aus


        int bpp = surface->format->BytesPerPixel;
        /* Here p is the address to the pixel we want to retrieve */
        Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

        switch(bpp) {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
    }
}
void set_init_vectors(SDL_Surface * surface,vector <vector <int>> &initcoord_pers_vec,vector <vector <int>> &initcoord_dest_vec,vector <vector <int>> &initcoord_obst_vec, int p_x, int p_y, int d_x, int d_y, int o_x, int o_y){//Liest den Grundriss ein

    vector <int > ith_coord;
    for (int y=0; y< grid_height; y++)
    {
        for (int x=3; x< grid_width; x++)
        {
            if (getpixel_function(surface, x, y)== getpixel_function(surface, p_x, p_y))
            {
                ith_coord.clear();
                ith_coord.push_back(x);
                ith_coord.push_back(y);

                initcoord_pers_vec.push_back(ith_coord);
            }
            else if (getpixel_function(surface, x, y)== getpixel_function(surface, o_x, o_y))
            {
                ith_coord.clear();
                ith_coord.push_back(x);
                ith_coord.push_back(y);

                initcoord_obst_vec.push_back(ith_coord);
            }
            else if (getpixel_function(surface, x, y)== getpixel_function(surface, d_x, d_y))
            {
                ith_coord.clear();
                ith_coord.push_back(x);
                ith_coord.push_back(y);

                initcoord_dest_vec.push_back(ith_coord);
            }
        }
    }

}
void print_init_vector(vector <vector<int>> &initcoord_vec){
    cout << "Init Koordinaten sind:" << endl;
    for(int i = 0; i < initcoord_vec.size(); i++){
        cout << "(" << initcoord_vec[i][0] << ";" << initcoord_vec[i][1] << ")" << " ";
    }
    cout << endl;
}
//#### Grundriss einlesen



//#### Grafikausgabe
void clear_drawing(SDL_Renderer *renderer){// clears the whole screen/pigment the whole screen white //NOCHMAL DURCH RICHTIGEN BEFEHL ERSETZEN
    SDL_SetRenderDrawColor(renderer,250,250,250,0);
    const SDL_Rect scrrect = {0,0,grid_width*10,grid_height*10}; //declarate rectangle, which contains the whole screen -> NOCHMAL NACHBESSERN : NUR BENÃTIGTER PLATZ
    SDL_RenderFillRect(renderer, &scrrect);
}
void draw_grid(vector <person> &pa, vector <destination> &da, vector<obstacle> &oa, SDL_Renderer *renderer){ //draw all objects on screen

    clear_drawing(renderer); // pigment the screen white
    for(int p = 0; p < pa.size(); p++){
        SDL_SetRenderDrawColor(renderer, pa[p].r, pa[p].g, pa[p].b, 0);
        SDL_RenderDrawPoint(renderer, pa[p].x, pa[p].y);
    }
    for(int d = 0; d < da.size(); d++){
        SDL_SetRenderDrawColor(renderer,da[d].r, da[d].g, da[d].b, 0);
        SDL_RenderDrawPoint(renderer, da[d].x, da[d].y);
    }
    for(int o = 0; o < oa.size(); o++){
        SDL_SetRenderDrawColor(renderer,oa[o].r, oa[o].g, oa[o].b, 0);
        SDL_RenderDrawPoint(renderer, oa[o].x, oa[o].y) ;
    }
    SDL_RenderPresent(renderer);
}
void draw_grid(vector <person> &pa, vector <destination> &da, vector<obstacle> &oa, SDL_Renderer *renderer, int magnification_factor){//draw all objects on screen with a magnication ; mÃ¶glicherweise eine grÃ¶Ãere VergrÃ¶Ãerung ermÃ¶glichen ?

    clear_drawing(renderer); // pigment the screen white
    if (magnification_factor == 1){
        draw_grid(pa,da,oa,renderer);
    }
    if (magnification_factor == 2){
        int shift_factor = magnification_factor; //shift of all points because of the magnification
        int quantity_drawing_points = 4; //quadrieren vom magnification factor

        for(int p = 0; p < pa.size(); p++){
            SDL_SetRenderDrawColor(renderer, pa[p].r, pa[p].g, pa[p].b, 0);
            SDL_Point drawing_points[quantity_drawing_points] = {
                {pa[p].x + pa[p].x * shift_factor , pa[p].y + pa[p].y * shift_factor},
                {pa[p].x + pa[p].x * shift_factor + 1 , pa[p].y + pa[p].y * shift_factor},
                {pa[p].x + pa[p].x * shift_factor , pa[p].y + pa[p].y * shift_factor + 1},
                {pa[p].x + pa[p].x * shift_factor + 1 , pa[p].y + pa[p].y * shift_factor + 1},
            };
            SDL_RenderDrawPoints(renderer,drawing_points,quantity_drawing_points);
        }

        for(int d = 0; d < da.size(); d++){
            SDL_SetRenderDrawColor(renderer, da[d].r, da[d].g, da[d].b, 0);
            SDL_Point drawing_points[quantity_drawing_points] = {
                {da[d].x + da[d].x * shift_factor , da[d].y + da[d].y * shift_factor},
                {da[d].x + da[d].x * shift_factor + 1 , da[d].y + da[d].y * shift_factor},
                {da[d].x + da[d].x * shift_factor , da[d].y + da[d].y * shift_factor + 1},
                {da[d].x + da[d].x * shift_factor + 1 , da[d].y + da[d].y * shift_factor + 1}
            };
            SDL_RenderDrawPoints(renderer,drawing_points,quantity_drawing_points);
        }

        for(int o = 0; o < oa.size(); o++){
            SDL_SetRenderDrawColor(renderer,oa[o].r, oa[o].g, oa[o].b, 0);
            SDL_Point drawing_points[quantity_drawing_points] = {
                {oa[o].x + oa[o].x * shift_factor , oa[o].y + oa[o].y * shift_factor},
                {oa[o].x + oa[o].x * shift_factor + 1 , oa[o].y + oa[o].y * shift_factor},
                {oa[o].x + oa[o].x * shift_factor , oa[o].y + oa[o].y * shift_factor + 1},
                {oa[o].x + oa[o].x * shift_factor + 1 , oa[o].y + oa[o].y * shift_factor + 1}
            };
            SDL_RenderDrawPoints(renderer,drawing_points,quantity_drawing_points);
        }

        }
    SDL_RenderPresent(renderer);
}
//#### Grafikausgabe


//#### Vorgehen wÃ¤hrend Iteration
void move_people_sequential(vector<person> &persvec, vector<obstacle> &obstvec, vector<destination> &destvec, vector <int > &propability_arr_diff, vector<int> &propability_arr_dec){
    //cout << "SIND GERADE HIER AM ARBEITEN" << endl;
    //Vector wird mit allen Nummern gefÃ¼llt; jede Nummer kann genau einer Person zugeordnet werden kann
    vector<int> serial_number_pers;
    for(int i = 0; i < persvec.size(); i++){
        serial_number_pers.push_back(i);
        //cout << i;
    }
    //cout << endl;
//Bewegung jeder Person:
    for(int j = 0; j < persvec.size(); j++){
    //Aussuchen: wer ist dran?
        int l =  rand() % (persvec.size() - j);
        //cout << "l: " << l << endl;
        int whose_turn = serial_number_pers[l];
        //cout << "whose turn: " << whose_turn << endl;
        serial_number_pers.erase(serial_number_pers.begin() + l);

    //Abrufen: Erstellen der transition Matrix:
    persvec[j].set_T(obstvec,persvec);

    //Aussuchen: Bewegungsrichtung:
    double r = ((rand() % 100) / 100.);
    //cout << "Zufallszahl r: " << r << endl;

    if(r < persvec[j].get_T(1,0)){// Bewegung nach oben?
        persvec[j].moveto(persvec[j].x, persvec[j].y - 1, persvec, propability_arr_diff, propability_arr_dec, obstvec);
    }
    else if(r < (persvec[j].get_T(1,0) + persvec[j].get_T(2,1))){//nach rechts?
        persvec[j].moveto(persvec[j].x + 1, persvec[j].y, persvec, propability_arr_diff, propability_arr_dec, obstvec);
    }
    else if(r < (persvec[j].get_T(1,0) + persvec[j].get_T(2,1) + persvec[j].get_T(1,2))){//nach unten?
        persvec[j].moveto(persvec[j].x, persvec[j].y + 1, persvec, propability_arr_diff, propability_arr_dec, obstvec);
    }
    else if(r < (persvec[j].get_T(1,0) + persvec[j].get_T(2,1) + persvec[j].get_T(1,2) + persvec[j].get_T(0,1))){//nach links?
        persvec[j].moveto(persvec[j].x - 1, persvec[j].y, persvec, propability_arr_diff, propability_arr_dec, obstvec);
    }
    else{//stehen bleiben
    }

    }

}
void move_people_parallel(vector<person> &persvec, vector<obstacle> &obstvec, vector<destination> &destvec, vector <int > &propability_arr_diff, vector<int> &propability_arr_dec){
//Jede Person entescheidet nun auf welche Koordinaten (desired_x,desired_y) sie gehen möchte
    for(int i = 0; i < persvec.size(); i++){
        //Voreinstellung der variable "wins_conflict", die für die nächste Schleife benötigt wird:
        persvec[i].wins_conflict = false;
        persvec[i].had_a_conflict = false;

        //jede Person setzt ihre Transition Matrix:
        persvec[i].set_T(obstvec,persvec);

        //für jede Person wird eine random Zahl zwischen 0 und 1 erstellt:
        double r = (rand() % 100) / 100. ;

        //Ermittelt nun stochastisch in welche Richtung sich die Person bewegen möchte:
        if(r < persvec[i].get_T(1,0)){ //nach oben?
            persvec[i].desired_x = persvec[i].x;
            persvec[i].desired_y = persvec[i].y - 1;
            persvec[i].desired_direction = 'o';
        }
        else if(r < (persvec[i].get_T(1,0) + persvec[i].get_T(2,1))){//nach rechts?
            persvec[i].desired_x = persvec[i].x + 1;
            persvec[i].desired_y = persvec[i].y;
            direction.push_back('r');
        }
        else if(r < (persvec[i].get_T(1,0) + persvec[i].get_T(2,1) + persvec[i].get_T(1,2))){//nach unten?
            persvec[i].desired_x = persvec[i].x;
            persvec[i].desired_y = persvec[i].y + 1;
            direction.push_back('u');
        }
        else if(r < (persvec[i].get_T(1,0) + persvec[i].get_T(2,1) + persvec[i].get_T(1,2) + persvec[i].get_T(0,1))){//nach links?
            persvec[i].desired_x = persvec[i].x - 1;
            persvec[i].desired_y = persvec[i].y;
            direction.push_back('l');
        }
        else{// stehen bleiben?
            persvec[i].desired_x = persvec[i].x;
            persvec[i].desired_y = persvec[i].y;
            persvec[i].wins_conflict = true; //Möchte die Person stehen bleiben, so gewinnt diese Person immer den Konflikt
            persvec[i].desired_direction = 's';
        }
    }

//Entscheidung welche Person sich bewegen darf und welche beispielsweise bei einem Konflikt stehen bleiben muss:
    for(int i = 0; i <persvec.size(); i++){
        vector<int> conflict_partner;//Enthält Nummer der Konfliktpartner

        //Fügt sich selbst zu den Konfliktpartnern hinzu:
        conflict_partner.push_back(i);

        //Fügt die anderen Konfliktpartner hinzu:
        for(int j = 0; j < persvec.size(); j++){
            //die Personen, die das selbe Ziel haben, werden zum Vektor conflict_partner hinzugefügt:
            if(persvec[j].desired_x == persvec[i].desired_x && persvec[j].desired_y == persvec[i].desired_y && i != j){
                conflict_partner.push_back(j);
            }
        }
        //Überprüft ob der Konflikt ausgetragen werden muss:
        bool conflict_done = false;
            //Überprüft ob nur eine Person im vector ist
        if(conflict_partner.size() == 1){
            persvec[conflict_partner[0]].wins_conflict = true;
            conflict_done = true;
        }
        else{//Überprüft, ob dieser Konflikt schon ausgetragen wurde, also ob eine Person, des conflict partner vectors, schon unter wins_conflict ein "true" zu stehen hat:
            for(int j = 0; j < conflict_partner.size(); j++){
                if(persvec[conflict_partner[j]].wins_conflict == true){
                    conflict_done = true;
                }

            }
        }

        //Wenn der Konflikt noch nicht ausgetragen wurde, also conflict_done == false ist, so wird dieser ausgeführt:
        if(conflict_done == false){
            conflict con = conflict(persvec[conflict_partner[0]].desired_x,persvec[conflict_partner[0]].desired_y, conflict_partner, persvec);
            persvec[con.number_of_winner].wins_conflict = true;
        }
        //Reset des Vektors:
        conflict_partner.clear();
    }


//Bewegt die Personen, die die Konflikte gewonnen haben
    for(int i = 0; i < persvec.size(); i++){
        if(persvec[i].wins_conflict == true){
            persvec[i].moveto(persvec[i].desired_x, persvec[i].desired_y, persvec, propability_arr_diff, propability_arr_dec, obstvec,persvec[i].had_a_conflict);
            //Nach konflikt muss hier noch true übergeben werden
        }
    }

}


}
bool has_pers_reached_destination(vector<destination> &destvec, vector<person> &persvec){//ÃberprÃ¼ft ob die Person das Ziel erreicht hat
        bool return_value = false;

        for(int i = 0; i < persvec.size(); i++){
            for(int j = 0; j < destvec.size(); j++){
                int nh[10] = {//neighbours of the selected destination
                    destvec[j].x, destvec[j].y + 1,
                    destvec[j].x + 1, destvec[j].y,
                    destvec[j].x, destvec[j].y - 1,
                    destvec[j].x - 1, destvec[j].y,
                    destvec[j].x, destvec[j].y
                    };
                for(int k = 0; k < 5; k++){
                    if(persvec[i].x == nh[2*k] && persvec[i].y == nh[2*k + 1] && persvec[i].evacuated == false){//Ist die Person ein Nachbar des Ziels?
                        persvec[i].x = destvec[j].x;//setzt Person in das Feld mit den Koordinaten des Ziels
                        persvec[i].y = destvec[j].y;
                        persvec[i].evacuated = true; //damit sich die Person nicht mehr aus dem Ziel hinausbewegt
                        persvec[i].iteration_when_evacuated = persvec[i].iteration; // Stoppt "Iterationsmessung"
                        persvec[i].end_time_measurement();// Stoppt Zeitmessung
                        return_value = true;
                    }
                }
            }
        }
        return return_value;
}
void update_object_parameters(int iteration, vector<person> &persvec, vector<destination> &destvec, vector<int> &propability_arr_diff, vector<int> &propability_arr_dec, vector <obstacle> &obstvec){

    for(int j = 0; j < persvec.size(); j++)
    {
        persvec[j].iteration = iteration;
        persvec[j].renew_w_S_and_S(destvec);

        persvec[j].diffusion_dyn_f(propability_arr_diff, persvec, persvec[j].x, persvec[j].y,j, obstvec, propability_arr_dec);
        persvec[j].decay_dyn_f(propability_arr_dec, persvec, j, obstvec);
    }
}
//#### Vorgehen wÃ¤hrend Iteration


//#### Analyse
void set_analyse_parameters(analysis_run &ana_run, char *k_S, char *k_D, char *w_S, char *friction, char *w_decay, char *w_diffusion){//setzt Parameter aus einem Ausruf aus der Shell
    ana_run.k_S = atoi(k_S) / 1000.;
    ana_run.k_D = atoi(k_D) / 1000.;
    ana_run.w_S = atoi(w_S) / 1000.;
    ana_run.friction = atoi(friction) / 1000.;

    decay_param = atoi(w_decay);
    diffusion_param = atoi(w_diffusion);

    cout << ana_run.k_S << ";" << ana_run.k_D << ";" << ana_run.w_S<< ";" << ana_run.friction << ";" << decay_param << ";" << diffusion_param << endl;
}
void set_model_parameters(vector<person> &persvec, double k_S, double k_D, double w_S, double friction){//setzt Parameter aller Personen; dies ist fÃ¼r die Analyse der Evakuierungszeit unabdingbar
    cout << "BIS HIER! " << k_D << endl;

    for(int i = 0; i < persvec.size(); i++){
        if(k_S >= 0){
            persvec[i].k_S = k_S;
        }
        if(k_D >= 0){
            persvec[i].k_D = k_D;
            cout << "HIER !!!:" << k_D << endl;
        }
        if(w_S >= 0){
            persvec[i].set_w_S(w_S);//Setzt w_S aller Ziela auf 1
        }
        if(friction >= 0){
            if(friction <= 1){
                persvec[i].friction = friction;
            }
            else{
                cout << "Fehler - der Friction Parameter kann nicht grÃ¶Ãer als 1 sein!" << endl;
                break;
            }
        }
        if(reject_other_D_fields == true){
            //Setzt alle w_S Parameter einer Person auf 0 außer die von einem einzigen zufällig ausgewähltem Ziel:
            persvec[i].set_w_S(0.0);
            persvec[i].set_w_S(1,false);
        }
    }
}
void evacuation_analysis(vector<person> &persvec){// Analysiert die Evakuierungszeit der Personen, sollte nur ausgefÃ¼rt werden, wenn vorher "set_model_parameters" angewendet wurde, also analysis_run.execute aktiviert ist
    //Ãffnet ein Dokument, in dem alle Daten gespeichert werden:
    fstream f;
    f.open("daten.dat", ios::app);

    // Berechnet die durchschnittliche Evakuierungszeit der Personen, die ihre Ziele erreichen
    double average_evac_time = 0;
    int  number_evac_pers = 0;
    for(int i = 0; i < persvec.size(); i++){
        if(persvec[i].evacuated == true){
            //cout << i << " endtime: " << persvec[i].time_end << endl;
            average_evac_time = average_evac_time + persvec[i].evacuation_time;
            number_evac_pers++;
        }
    }
    average_evac_time = average_evac_time / number_evac_pers;
    // Berechnet die durchschnittlich benÃ¶tigte Iterationsanzahl, damit die Personen ans Ziel kommen:
    double average_evac_iteration = 0;
    for(int i = 0; i < persvec.size(); i++){
        if(persvec[i].evacuated == true){
            //cout << i << "enditeration: " << persvec[i].iteration_when_evacuated << endl;
            average_evac_iteration = average_evac_iteration + persvec[i].iteration_when_evacuated;
        }
    }
    average_evac_iteration = average_evac_iteration / number_evac_pers;

    //Berechnet die durschnittliche Anzahl an Kollisionen der einzelnen Personen:
    double average_number_conflicts = 0;
    for(int i = 0; i < persvec.size(); i++){
        //cout << i << "Anzahl Konflikte: " << persvec[i].number_of_conflicts << endl;
        average_number_conflicts = average_number_conflicts + persvec[i].number_of_conflicts;
    }
    average_number_conflicts = average_number_conflicts / number_evac_pers;

    //Schreibt berechnete Daten in das geöffnete Dokument
    //Reihenfolge der Daten ist: Name Grundris, Durchschnittliche Evakuierungszeit, Durchschnittliche Iteration bei Evakuierung, Anzahl der Personen, die das Ziel nicht erreichen, Anzahl der Personen, k_S, k_D, w_S, friction, decay, diffusion Update Regel, Grafik_Delay, Durschnittliche Anzahl der Kollisionen
    f << (string) plant_layout << " " << average_evac_time << " " << average_evac_iteration << " " << persvec.size() - number_evac_pers << " " << persvec.size() << " " << persvec[0].k_S << " " << persvec[0].k_D << " " << persvec[0].w_S[0] << " " << persvec[0].friction <<" " <<decay_param << " " <<diffusion_param << " " << movement_update << " " << grafic_delay << " " << average_number_conflicts << endl;
    f.close();
/*
Density
panic
k_D
k_S
w_S - Wissen Ã¼ber das Ziel
omega - steht fÃ¼r w_S
*/
}
//#### Analyse



//test

void lege_und_printe_grunriss_auf_dfeld (vector <person> &persvec, vector <obstacle > &obstvec, vector <destination > &destvec, int ith_person, vector <vector<int>> &initcoord_pers_vec)
{
    //erzeugt grundriss
    int static Grundriss [grid_width][grid_height];
    for (int x=0; x<grid_width; x++)
    {
        for (int y=0; y<grid_height; y++)
        {
            Grundriss[x][y]=0;
        }
    }


    for (int i=0; i< obstvec.size(); i++)
    {
        Grundriss [obstvec[i].x][obstvec[i].y]=-10;
    }

    for (int j=0; j< destvec.size(); j++)
    {
        Grundriss [destvec[j].x][destvec[j].y]=-20;
    }

    for (int k=0; k < persvec.size(); k++)
    {
        Grundriss [initcoord_pers_vec[k][0]][initcoord_pers_vec[k][1]]=-30;
    }

    // addiert grundriss mit d feld
    int static Grundriss_D_Feld[grid_width][grid_height];
    for (int x=0; x<grid_width; x++)
    {
        for (int y=0; y<grid_height; y++)
        {
            Grundriss_D_Feld[x][y]=persvec[ith_person].D[x][y]+Grundriss[x][y];
        }
    }
    //print
    for (int x=0; x<grid_width; x++)
    {
        for (int y=0; y<grid_height; y++)
        {
            cout << Grundriss_D_Feld[x][y] << ";" ;
        }
        cout << endl;
    }
}



//test



int main(int argc, char* args[]){
    //ÃberprÃ¼fung der angegebenen Parameter:
    ifstream file_test(plant_layout);
    if(!file_test){
        std::cout << "Der Name des Gebaeudeplans wurde falsch eingegeben oder diese Datei existiert nicht. Korrigieren Sie die Eingabe der Variable plant_layout! " << endl;
        return EXIT_SUCCESS;
    }


    srand (time(NULL));


    analysis_run ana_run;
    //FÃ¼r den Aufruf Ã¼ber die Shell, bzw fÃ¼r den Aufruf Ã¼ber die Batch Datei:
    if(ana_run.foreign_call == true){
        set_analyse_parameters(ana_run, args[1], args[2], args[3], args[4], args[5], args[6]);
    }


    //Initialisation SDL um den gespeicherten Grundriss zu laden
    SDL_Event Event;
    SDL_Window* Window;
    SDL_Surface* screen_surface;
    SDL_Surface* bmp_surf;
    SDL_PixelFormat *fmt;
    SDL_Init( SDL_INIT_VIDEO );
    Window = SDL_CreateWindow( "Grundriss", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, grid_width, grid_height, SDL_WINDOW_SHOWN );
    screen_surface = SDL_GetWindowSurface( Window );
    bmp_surf = SDL_LoadBMP(plant_layout);

    //Zeigt Grundriss, mit dem das Programm arbeiten wird
    SDL_BlitSurface( bmp_surf, NULL, screen_surface, NULL );
    SDL_UpdateWindowSurface( Window );

    //get start conditions
    vector <vector <int >> initcoord_dest_vec;
    vector <vector <int >> initcoord_pers_vec;
    vector <vector <int >> initcoord_obst_vec;
    set_init_vectors(bmp_surf,initcoord_pers_vec,initcoord_dest_vec,initcoord_obst_vec,2,0,1,0,0,0); //befÃ¼llt die initcoord Vektoren
    int quantity_persons = initcoord_pers_vec.size();
    int quantity_destinations = initcoord_dest_vec.size();
    int quantity_obstacles = initcoord_obst_vec.size();

    //Ausgabe der Koordinaten der noch zu erstellenden Personen, Hindernissen, Zielen
    cout << "dest " ;
    print_init_vector(initcoord_dest_vec);
    cout << "obst " ;
    print_init_vector(initcoord_obst_vec);
    cout << "pers " ;
    print_init_vector(initcoord_pers_vec);

    //SchlieÃen des SDL_Fensters
    while (ana_run.execute == false) {if (SDL_PollEvent(&Event) && Event.type == SDL_QUIT){break;}} //HÃ¤lt Fenster so lange offen bis es per Hand geschlossen wird
    SDL_FreeSurface( bmp_surf );
	bmp_surf = NULL;
	SDL_DestroyWindow( Window );
	Window = NULL;
	SDL_Quit();





//test

vector <int > propability_arr_diff(100);
vector <int> propability_arr_dec(100);

//test

//################## object declaration
//declaration of used objects:
    vector<person> persvec;
    persvec.resize(quantity_persons);

    vector<destination> destvec;
    destvec.resize(quantity_destinations);

    vector<obstacle> obstvec;
    obstvec.resize(quantity_obstacles);

//construction of used objects
    for(int o = 0; o < quantity_obstacles; o++){
        obstvec[o] = obstacle(initcoord_obst_vec[o][0],initcoord_obst_vec[o][1],quantity_obstacles,quantity_destinations,quantity_persons);
    }
    for(int d = 0; d < quantity_destinations; d++){
        destvec[d] = destination(initcoord_dest_vec[d][0],initcoord_dest_vec[d][1],obstvec,quantity_obstacles,quantity_destinations,quantity_persons);
    }
    for(int p = 0; p < quantity_persons; p++){
        persvec[p] = person(initcoord_pers_vec[p][0],initcoord_pers_vec[p][1],destvec,quantity_obstacles,quantity_destinations,quantity_persons);
    }

//Bei einem Durchlauf des Programms, bei dem Daten entnommen und Analysiert werden mÃ¼ssen, mÃ¼ssen gleichwertige Bedingungen hergestellt werden
//Deshalb werden dabei einige Parameter nochmals umgeÃ¤ndert:
    if(ana_run.execute == true){
        set_model_parameters(persvec,ana_run.k_S,ana_run.k_D,ana_run.w_S,ana_run.friction);
    }

//################## object declaration


//################## visual output 1
    SDL_Event event;
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(grid_width*3, grid_height*3, 0, &window, &renderer);
//################## visual output 1



//test
//test


for(int i = 0; i < max_number_of_iterations; i++){
//################## iteration method
    has_pers_reached_destination(destvec,persvec);

    if(movement_update == 's'){
        move_people_sequential(persvec,obstvec,destvec, propability_arr_diff, propability_arr_dec);
    }
    else if(movement_update == 'p'){
        move_people_parallel(persvec,obstvec,destvec, propability_arr_diff, propability_arr_dec);
    }
    else {
        cout << "Fehler in der Eingabe: movement_update kann nur 'p' oder 's' sein"  << endl;
    }

    //Abbruchbedingung, wenn die max_number_of_iterations zu hoch gewÃ¤hlt wurde
    bool b_c = true;
    for (int j = 0; j < persvec.size(); j++){
        if(persvec[j].evacuated == false){
            b_c = false;
        }
    }
    if (b_c == true){
        break;
    }


    update_object_parameters(i,persvec,destvec,propability_arr_diff,propability_arr_dec, obstvec);
//################## iteration method



//################## visual output 2
        draw_grid(persvec,destvec,obstvec,renderer,2);
        SDL_Delay(grafic_delay);
}
    cout << "k_D: " << persvec[0].k_D;
    cout << "Durchlauf abgeschlossen" << endl;
    SDL_Delay(500);

    while (ana_run.execute == false) {if (SDL_PollEvent(&event) && event.type == SDL_QUIT){break;}} //HÃ¤lt Fenster so lange offen bis es per Hand geschlossen wird
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


/*
//test
for (int i = 0; i < persvec.size(); i++){
    cout << "Anzahl Konflikte von: " << i << " ist:" << persvec[i].number_of_conflicts << endl;
}
cout << "anzahl personen:" << quantity_persons<< endl;
int i;
    for (i=0; i< 1  ; i++)
    {
        cout << i<<". " << "Person D Feld:"  << endl;
        persvec[i].print_D();

        cout << i<<". " << "Person S Feld:"  << endl;
        persvec[i].print_S();
    }

    cout << " " << endl;
    cout << "Grundriss + dfeld von Person:" << i << endl;
    lege_und_printe_grunriss_auf_dfeld(persvec, obstvec, destvec, 0, initcoord_pers_vec);

    cout << " " <<endl;
    cout << "diff parameter: " << diffusion_param << endl;
    cout << "propabillity arr diff" << endl;
    for (int i=0; i<propability_arr_diff.size(); i++)
    {
        cout << propability_arr_diff[i] << "; ";
    }

    cout << " " <<endl;
    cout << "dec parameter: " << decay_param << endl;
    cout << "propabillity arr dec" << endl;
    for (int i=0; i<propability_arr_dec.size(); i++)
    {
        cout << propability_arr_dec[i] << "; ";
    }



//test
*/

//################## visual output 2

//################## Analyse
    if(ana_run.execute == true){
        evacuation_analysis(persvec);
    }
//################## Analyse



    return EXIT_SUCCESS;
}
