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


//#### Vorgehen waehrend Iteration
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
        persvec[j].moveto(persvec[j].x, persvec[j].y - 1);
    }
    else if(r < (persvec[j].get_T(1,0) + persvec[j].get_T(2,1))){//nach rechts?
        persvec[j].moveto(persvec[j].x + 1, persvec[j].y);
    }
    else if(r < (persvec[j].get_T(1,0) + persvec[j].get_T(2,1) + persvec[j].get_T(1,2))){//nach unten?
        persvec[j].moveto(persvec[j].x, persvec[j].y + 1);
    }
    else if(r < (persvec[j].get_T(1,0) + persvec[j].get_T(2,1) + persvec[j].get_T(1,2) + persvec[j].get_T(0,1))){//nach links?
        persvec[j].moveto(persvec[j].x - 1, persvec[j].y);
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
            persvec[i].desired_direction = 'r';
        }
        else if(r < (persvec[i].get_T(1,0) + persvec[i].get_T(2,1) + persvec[i].get_T(1,2))){//nach unten?
            persvec[i].desired_x = persvec[i].x;
            persvec[i].desired_y = persvec[i].y + 1;
            persvec[i].desired_direction = 'u';
        }
        else if(r < (persvec[i].get_T(1,0) + persvec[i].get_T(2,1) + persvec[i].get_T(1,2) + persvec[i].get_T(0,1))){//nach links?
            persvec[i].desired_x = persvec[i].x - 1;
            persvec[i].desired_y = persvec[i].y;
            persvec[i].desired_direction = 'l';
        }
        else{// stehen bleiben?
            persvec[i].desired_x = persvec[i].x;
            persvec[i].desired_y = persvec[i].y;
            persvec[i].wins_conflict = true; //Möchte die Person stehen bleiben, so gewinnt diese Person immer den Konflikt
            persvec[i].desired_direction = 's';
        }
    }

///Entscheidung welche Person sich bewegen darf und welche beispielsweise bei einem Konflikt stehen bleiben muss:
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
                persvec[conflict_partner[j]].had_a_conflict=true;
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
            persvec[i].moveto(persvec[i].desired_x, persvec[i].desired_y, persvec[i].had_a_conflict);
            //Nach konflikt muss hier noch true übergeben werden
        }
        //Die Personen die den Konflikt verloren haben erhalten ein D Feld steigerung auf der von ihnen aus rechts gesehenem Feld -> Dies soll zu einer besseren Konfliktbeweltigung führen:
        else if(corridor_conditions == true){
            int radius = 5; //Radius um die gefragt Person, die einen Konflikt hat, welcher angibt von welchen Nachbarpersonen das D Feld verändert wird
            if(persvec[i].desired_direction == 'o'){
                //Veränderung des D Feldes der Nachbarpersonen
                for(int j = 0; j < persvec.size(); j++){
                    if(persvec[j].desired_direction == 'o' && i!=j && persvec[j].x < persvec[i].x + radius && persvec[j].x > persvec[i].x - radius && persvec[j].y < persvec[i].y + radius && persvec[j].y > persvec[i].y - radius){
                        if(persvec[j].could_I_go_to(persvec[j].x + 1,persvec[j].y,obstvec,persvec)){
                            persvec[j].D[persvec[j].x + 1][persvec[j].y]++;
                        }
                    }
                }
                //Veränderung des eigenen D Feldes
                if(persvec[i].could_I_go_to(persvec[i].x + 1,persvec[i].y,obstvec,persvec)){
                    persvec[i].D[persvec[i].x + 1][persvec[i].y]++;
                    persvec[i].D[persvec[i].x + 1][persvec[i].y]++;
                }

            }
            else if(persvec[i].desired_direction == 'r'){
                for(int j = 0; j < persvec.size(); j++){
                    if(persvec[j].desired_direction == 'r' && i!=j &&persvec[j].x < persvec[i].x + radius && persvec[j].x > persvec[i].x - radius && persvec[j].y < persvec[i].y + radius && persvec[j].y > persvec[i].y - radius){
                        if(persvec[j].could_I_go_to(persvec[j].x,persvec[j].y + 1,obstvec,persvec)){
                            persvec[j].D[persvec[j].x][persvec[j].y + 1]++;
                        }
                    }
                }

                if(persvec[i].could_I_go_to(persvec[i].x,persvec[i].y + 1,obstvec,persvec)){
                    persvec[i].D[persvec[i].x][persvec[i].y + 1]++;
                    persvec[i].D[persvec[i].x][persvec[i].y + 1]++;
                }
            }
            else if(persvec[i].desired_direction == 'u'){
                for(int j = 0; j < persvec.size(); j++){
                    if(persvec[j].desired_direction == 'u' && i!=j &&persvec[j].x < persvec[i].x + radius && persvec[j].x > persvec[i].x - radius && persvec[j].y < persvec[i].y + radius && persvec[j].y > persvec[i].y - radius){
                        if(persvec[j].could_I_go_to(persvec[j].x - 1,persvec[j].y,obstvec,persvec)){
                            persvec[j].D[persvec[j].x - 1][persvec[j].y]++;
                        }
                    }
                }
                if(persvec[i].could_I_go_to(persvec[i].x - 1,persvec[i].y,obstvec,persvec)){
                    persvec[i].D[persvec[i].x - 1][persvec[i].y]++;
                    persvec[i].D[persvec[i].x - 1][persvec[i].y]++;
                }
            }
            else if(persvec[i].desired_direction == 'l'){
                for(int j = 0; j < persvec.size(); j++){
                    if(persvec[j].desired_direction == 'l' && i!=j &&persvec[j].x < persvec[i].x + radius && persvec[j].x > persvec[i].x - radius && persvec[j].y < persvec[i].y + radius && persvec[j].y > persvec[i].y - radius){
                        if(persvec[j].could_I_go_to(persvec[j].x,persvec[j].y - 1,obstvec,persvec)){
                            persvec[j].D[persvec[j].x][persvec[j].y - 1]++;
                        }
                    }
                }
                if(persvec[i].could_I_go_to(persvec[i].x,persvec[i].y - 1,obstvec,persvec)){
                    persvec[i].D[persvec[i].x][persvec[i].y - 1]++;
                    persvec[i].D[persvec[i].x][persvec[i].y - 1]++;
                }
            }
        }
    }
}

bool has_pers_reached_destination(vector<destination> &destvec, vector<person> &persvec){//Ueberprueft ob die Person das Ziel erreicht hat
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
                        if (corridor_conditions==true)
                        {
                            int max_x=0;
                            int min_x=grid_width;
                            for (int i=0; i<destvec.size(); i++)
                            {
                                if (destvec[i].x > max_x)
                                {
                                    max_x=destvec[i].x;
                                }
                                if (destvec[i].x < min_x)
                                {
                                    min_x=destvec[i].x;
                                }
                            }
                            if (destvec[j].x==max_x)
                            {
                                persvec[i].moveto(min_x+2, destvec[j].y);
                            }
                            if (destvec[j].x==min_x)
                            {
                                /*cout << "max_x= " << max_x << endl;
                                cout << "persvec[i].x= " << persvec[i].x << endl;*/
                                persvec[i].x=max_x-2;
                                //cout << "persvec[i].x= " << persvec[i].x << endl;
                            }
                            persvec[i].evacuated = false;
                            return_value = false;
                        }
                        else
                        {
                            persvec[i].moveto(destvec[j].x,destvec[j].y);
                            persvec[i].evacuated = true; //damit sich die Person nicht mehr aus dem Ziel hinausbewegt
                            persvec[i].iteration_when_evacuated = persvec[i].iteration; // Stoppt "Iterationsmessung"
                            persvec[i].end_time_measurement();// Stoppt Zeitmessung
                            return_value = true;
                        }
                    }
                }
                if(persvec[i].x == nh[2*4] && persvec[i].y == nh[2*4 + 1] && persvec[i].evacuated == true && return_value == false && corridor_conditions==false){
                    persvec[i].aax = persvec[i].ax;
                    persvec[i].aay = persvec[i].ay;
                    persvec[i].ax = persvec[i].x;
                    persvec[i].ay = persvec[i].y;
                }
            }
        }
        return return_value;
}
void update_object_parameters(int iteration, vector<person> &persvec, vector<destination> &destvec, vector<int> &propability_arr_diff, vector<int> &propability_arr_dec, vector <obstacle> &obstvec, bool foreign_call){//Erneuert Parameter, wird nach jedem Iterationsschritt aufgerufen

    for(int j = 0; j < persvec.size(); j++)
    {
        //cout << "persvec[j].x= " << persvec[j].x << endl;
        persvec[j].iteration = iteration;
        if(corridor_conditions == false){ persvec[j].renew_w_S_and_S(destvec,foreign_call); }
        persvec[j].last_movement_direction = persvec[j].set_last_movement_direction(persvec[j].ax, persvec[j].ay, persvec[j].x, persvec[j].y);
        persvec[j].a_last_movement_direction = persvec[j].set_last_movement_direction(persvec[j].aax, persvec[j].aay, persvec[j].ax, persvec[j].ay);
        persvec[j].set_D_3(persvec, j);
        persvec[j].set_panic_par(persvec, j, iteration);
        persvec[j].diffusion_dyn_f(propability_arr_diff, persvec, persvec[j].x, persvec[j].y,j, obstvec, propability_arr_dec);
        persvec[j].decay_dyn_f(propability_arr_dec, persvec, j);
    }
}
//Wenn Ziele nebeneinanderliegen, kann folgende Funktion ausgeführt werden, damit die Personen beide Ziele "als ein Ziel" sehen (w_S wird bei beiden gleich gesetzt)
void unite_destinations(vector <person> &persvec, vector <destination> &destvec){
    for(int l = 0; l < destvec.size(); l++){//Damit auch Nachbarn von Nachbarn angepasst werden (destvec.size() ist die maximal nötige Anzahl an Iterationen)
        for(int i = 0; i < destvec.size(); i++){
            for(int j = 0; j < destvec.size(); j++){
                if((destvec[i].x == destvec[j].x + 1 && destvec[i].y == destvec[j].y) || (destvec[i].x == destvec[j].x - 1 && destvec[i].y == destvec[j].y) || (destvec[i].x == destvec[j].x && destvec[i].y == destvec[j].y + 1)  || (destvec[i].x == destvec[j].x && destvec[i].y == destvec[j].y - 1)){//wenn ja dann sind beides Nachbarn
                    if(l == destvec.size()){
                        destvec[i].dest_neighbours.push_back(j);
                    }
                    for(int k = 0; k < persvec.size(); k++){
                        if(persvec[k].w_S[i] > persvec[k].w_S[j]){
                            persvec[k].w_S[j] = persvec[k].w_S[i];
                        }
                        else{
                            persvec[k].w_S[i] = persvec[k].w_S[j];
                        }
                    }
                }
                //AUSSNAHME SICH SELBST
            }
        }
    }
}
//#### Vorgehen wÃ¤hrend Iteration


//#### Analyse

void adapt_w_S_has_only_one_destination(vector<person> &persvec, vector<destination> &destvec){/// Sorgt dafür, dass alle Personen nur ein Ziel kennen, dies wird für die Simulation des Korridors benötigt
    //cout << "HIER !" << endl;
    for(int i = 0; i < persvec.size(); i++){
    //Setzt alle w_S Parameter einer Person auf 0 außer die von einem einzigen zufällig ausgewähltem Ziel:
        bool w_S_modified = false; //zur Ueberpruefung der korrekten Ausführung der Änderung von w_S

        //Ueberschreibung des vectors w_S, damit nur noch ein Eintrag von w_S die Zahl 1 enthält (alle anderen werden mit 0 gefüllt)
        persvec[i].set_w_S(0.0);
        persvec[i].set_w_S(1,false);
        for(int j = 0; j < destvec.size(); j++){
            if(persvec[i].w_S[j] == 1){
                persvec[i].numb_selected_dest = j;
                //Änderung der Farbe der Personen
                persvec[i].g = (int)(j * 250 / destvec.size());

                w_S_modified = true;
            }
        }
            //Fehlerueberpuefung:
        if(w_S_modified == false){
            cout << "Fehler: w_S wurde nicht dem Betriebsmodus angepasst" << endl;
        }

    }
}
void set_analyse_parameters(analysis_run &ana_run, char *k_S, char *k_D, char *w_S, char *friction, char *w_decay, char *w_diffusion){//setzt Parameter aus einem Ausruf aus der Shell
    ana_run.k_S = atoi(k_S) / 1000.;
    ana_run.k_D = atoi(k_D) / 1000.;
    ana_run.w_S = atoi(w_S) / 1000.;
    ana_run.friction = atoi(friction) / 1000.;

    decay_param = atoi(w_decay);
    diffusion_param = atoi(w_diffusion);

    cout << ana_run.k_S << ";" << ana_run.k_D << ";" << ana_run.w_S<< ";" << ana_run.friction << ";" << decay_param << ";" << diffusion_param << endl;
}
void set_model_parameters(analysis_run ana_run, vector<person> &persvec, vector<destination> &destvec, vector<obstacle> &obstvec){//setzt Parameter aller Personen; dies ist fÃ¼r die Analyse der Evakuierungszeit unabdingbar
    if(ana_run.execute == true){
        for(int i = 0; i < persvec.size(); i++){
            if(ana_run.k_S >= 0){
                persvec[i].k_S = ana_run.k_S;
            }
            if(ana_run.k_D >= 0){
                persvec[i].k_D = ana_run.k_D;
            }
            if(ana_run.w_S >= 0){
                persvec[i].set_w_S(ana_run.w_S);//Setzt w_S aller Ziela auf 1
            }
            if(ana_run.friction >= 0){
                if(ana_run.friction <= 1){
                    persvec[i].friction = ana_run.friction;
                }
                else{
                    cout << "Fehler - der Friction Parameter kann nicht groesser als 1 sein!" << endl;
                    break;
                }
            }
        }
    }
    if(corridor_conditions == true){//Handelt es sich bei der geforderten Situation um einen Korridor, so ist reject_other_D_fields automatisch aktiviert
        adapt_w_S_has_only_one_destination(persvec,destvec);
        unite_destinations_if_possible = true;
        for(int i = 0; i < persvec.size(); i++){
            persvec[i].set_S_corridor(persvec,destvec,obstvec);
        }
    }
    if(reject_other_D_fields == true && corridor_conditions == false){
        adapt_w_S_has_only_one_destination(persvec,destvec);
    }
    if(unite_destinations_if_possible == true){
        unite_destinations(persvec,destvec);
    }
}

void evacuation_analysis(vector<person> &persvec){// Analysiert die Evakuierungszeit der Personen, sollte nur ausgefÃ¼rt werden, wenn vorher "set_model_parameters" angewendet wurde, also analysis_run.execute aktiviert ist
    //oeffnet ein Dokument, in dem alle Daten gespeichert werden:
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
    for (int y=0; y<grid_height; y++)
    {
        for (int x=0; x<grid_width; x++)
        {
            Grundriss_D_Feld[x][y]=persvec[ith_person].D[x][y]+Grundriss[x][y];
        }
    }
    //print
    for (int y=-1; y<grid_width; y++)
    {
        if (y==-1)
        {
            cout << "  ";
        }
        if (0<=y && y <=9)
        {
            cout << " " << y ;
        }
        if (10<=y && y <=99)
        {
            cout << y ;
        }
        for (int x=0; x<grid_height; x++)
        {

            if (y==-1)
            {
                if (0<=x && x <=9)
                {
                    cout << "  " << x << ";";
                }
                if (10<=x && x <=99)
                {
                    cout << " " << x << ";";
                }
            }
            //cout << Grundriss_D_Feld[x][y] << ";" ;
            if (y>=0)
            {
                if (0<= Grundriss_D_Feld[x][y] && Grundriss_D_Feld[x][y]<=9)
                {
                    cout << "  " << Grundriss_D_Feld[x][y] << ";" ;
                }
                else if (10<=Grundriss_D_Feld[x][y] && Grundriss_D_Feld[x][y] <= 99)
                {
                    cout << " " << Grundriss_D_Feld[x][y] << ";";
                }
                else
                {
                    cout << Grundriss_D_Feld[x][y] << ";";
                }
            }
        }
        cout << endl;
    }
}



//test



int main(int argc, char* args[]){
    //Ueberpruefung der angegebenen Parameter:
    ifstream file_test(plant_layout);
    if(!file_test){
        std::cout << "Der Name des Gebaeudeplans wurde falsch eingegeben oder diese Datei existiert nicht. Korrigieren Sie die Eingabe der Variable plant_layout! " << endl;
        return EXIT_SUCCESS;
    }

    cout << "Programm wurde gestartet;" << endl;

    srand (time(NULL));


    analysis_run ana_run;
    //Fuer den Aufruf ueber die Shell, bzw fuer den Aufruf ueber die Batch Datei:
    if(ana_run.foreign_call == true){
        set_analyse_parameters(ana_run, args[1], args[2], args[3], args[4], args[5], args[6]);
    }

    cout << "Grundriss wird geladen..." << endl;

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

    cout << "Grundriss geladen;" << endl;

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
    cout << "------------------------------" << endl;
    cout << "dest " ;
    print_init_vector(initcoord_dest_vec);
    cout << "Anzahl Ziele: " << initcoord_dest_vec.size()<< endl;
    cout << "------------------------------" << endl;

    cout << "obst " ;
    print_init_vector(initcoord_obst_vec);
    cout << "Anzahl Gegenstaende: " << initcoord_obst_vec.size()<< endl;
    cout << "------------------------------" << endl;

    cout << "pers " ;
    print_init_vector(initcoord_pers_vec);
    cout << "Anzahl Personen: " << initcoord_pers_vec.size()<< endl;
    cout << "------------------------------" << endl;

    //Schliessen des SDL_Fensters
    while (ana_run.foreign_call == false) {if (SDL_PollEvent(&Event) && Event.type == SDL_QUIT){break;}} //HÃ¤lt Fenster so lange offen bis es per Hand geschlossen wird
    SDL_FreeSurface( bmp_surf );
	bmp_surf = NULL;
	SDL_DestroyWindow( Window );
	Window = NULL;
	SDL_Quit();




//test

vector <int > propability_arr_diff(100);
vector <int> propability_arr_dec(100);

cout << "Wahrscheinlichkeitsarrays wurden erstellt;" << endl;

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


//Bei einem Durchlauf des Programms, bei dem Daten entnommen und Analysiert werden muessen, muessen gleichwertige Bedingungen hergestellt werden
//Deshalb werden dabei einige Parameter nochmals umgeaendert:
    set_model_parameters(ana_run, persvec, destvec, obstvec);

//################## object declaration

    cout << "Objekte wurden erstellt;" << endl;
    cout << " " << endl;

	cout << "k_D: " << persvec[0].k_D << endl;
	cout << "k_S: " << persvec[0].k_S << endl;

    cout << " " <<endl;
    cout << "Diff. Parameter: " << diffusion_param << endl;
    cout << " " <<endl;
    cout << "Dec. Parameter: " << decay_param << endl;

    cout << " " <<endl;
    cout << "corridorconditions: " << corridor_conditions << endl;

    cout << " " <<endl;
    cout << "reject_other_d_fields: " << reject_other_D_fields << endl;

    cout << " " <<endl;
    cout << "grafic delay: " << grafic_delay << endl;

    cout << " " <<endl;
    cout << "Bewegungsupdate: " << movement_update << endl;



//################## visual output 1
    SDL_Event event;
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(grid_width*3, grid_height*3, 0, &window, &renderer);
//################## visual output 1



//test

//persvec[0].print_S();
//persvec[1].print_S();
/*
for(int i = 0; i < 4;i++){
    cout << "Hier w_S:" << endl;
    for(int j = 0; j < persvec[i].w_S.size(); j++){
        cout << persvec[i].w_S[j] << ";";
    }
    cout << endl << "ausgewaehltes dest: " << persvec[i].numb_selected_dest << endl;
}
*/
//test


for(int i = 0; i < max_number_of_iterations; i++){
//################## iteration method
    //persvec[20].print_coords();
    //persvec[20].print_T();
    //persvec[20].print_S();
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

    update_object_parameters(i,persvec,destvec,propability_arr_diff,propability_arr_dec, obstvec, ana_run.foreign_call);

    ///test

    /*if (i%25==0)
    {
    cout << "                                                                " << endl;
    cout << "================================================================" << endl;
    cout << "Personen haben sich zum " << i<< "ten mal bewegt!" << endl;
    cout << "================================================================" << endl;

    cout << " " << endl;
    cout << "D feld von person: 1" << endl;
    persvec[1].print_D();

    cout << "                                                                " << endl;
    cout << "Grundriss + dfeld von Person:" << "1" << endl;
    lege_und_printe_grunriss_auf_dfeld(persvec, obstvec, destvec, 1, initcoord_pers_vec);
    }*/

    ///test

//################## iteration method



//################## visual output 2
        draw_grid(persvec,destvec,obstvec,renderer,2);
        SDL_Delay(grafic_delay);
}

    SDL_Delay(500);

    while (ana_run.foreign_call == false) {if (SDL_PollEvent(&event) && event.type == SDL_QUIT){break;}} //HÃ¤lt Fenster so lange offen bis es per Hand geschlossen wird
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


//test
/*
for (int i = 0; i < persvec.size(); i++){
    cout << "Anzahl Konflikte von: " << i << " ist: " << persvec[i].number_of_conflicts << endl;
}
cout << "anzahl personen:" << quantity_persons<< endl;
int i;
    for (i=1; i< 2  ; i++)
    {
        cout << i<<". " << "Person D Feld:"  << endl;
        persvec[i].print_D();

        /*cout << i<<". " << "Person S Feld:"  << endl;
        persvec[i].print_S();
    }

    cout << " " << endl;
    cout << "Grundriss + D-Feld von Person:" << "1" << endl;
    lege_und_printe_grunriss_auf_dfeld(persvec, obstvec, destvec, 1, initcoord_pers_vec);

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

*/
//test


//################## visual output 2

//################## Analyse
    if(ana_run.execute == true){
        evacuation_analysis(persvec);
    }
//################## Analyse



    return EXIT_SUCCESS;
}
