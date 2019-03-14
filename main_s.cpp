#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "klassen.cpp"



using namespace std;


//################## initial situation
#include "initial_situation.cpp"
//################## initial situation


//################## object declaration 1
//declaration of used objects:
person persarray [quantity_persons]; //contains all objects of type person
destination destarray [quantity_destinations];
obstacle obsarray [quantity_obstacles];
//################## object declaration 1

//#### Grafik
void clear_drawing(SDL_Renderer *renderer){// clears the whole screen/pigment the whole screen white //NOCHMAL DURCH RICHTIGEN BEFEHL ERSETZEN
    SDL_SetRenderDrawColor(renderer,250,250,250,0);
    const SDL_Rect scrrect = {0,0,grid_width*10,grid_height*10}; //declarate rectangle, which contains the whole screen -> NOCHMAL NACHBESSERN : NUR BENÖTIGTER PLATZ
    SDL_RenderFillRect(renderer, &scrrect);
}
void draw_grid(person *pa, destination *da, obstacle *oa, SDL_Renderer *renderer){ //draw all objects on screen

    clear_drawing(renderer); // pigment the screen white
    for(int p = 0; p < quantity_persons; p++){
        SDL_SetRenderDrawColor(renderer, pa[p].r, pa[p].g, pa[p].b, 0);
        SDL_RenderDrawPoint(renderer, pa[p].x, pa[p].y);
    }
    for(int d = 0; d < quantity_destinations; d++){
        SDL_SetRenderDrawColor(renderer,da[d].r, da[d].g, da[d].b, 0);
        SDL_RenderDrawPoint(renderer, da[d].x, da[d].y);
    }
    for(int o = 0; o < quantity_obstacles; o++){
        SDL_SetRenderDrawColor(renderer,oa[o].r, oa[o].g, oa[o].b, 0);
        SDL_RenderDrawPoint(renderer, oa[o].x, oa[o].y) ;
    }
    SDL_RenderPresent(renderer);
}
void draw_grid(person *pa, destination *da, obstacle *oa, SDL_Renderer *renderer, int magnification_factor){//draw all objects on screen with a magnication ; möglicherweise eine größere Vergrößerung ermöglichen ?

    clear_drawing(renderer); // pigment the screen white
    if (magnification_factor == 1){
        draw_grid(pa,da,oa,renderer);
    }
    if (magnification_factor == 2){
        int shift_factor = magnification_factor; //shift of all points because of the magnification
        int quantity_drawing_points = 4; //quadrieren vom magnification factor

        for(int p = 0; p < quantity_persons; p++){
            SDL_SetRenderDrawColor(renderer, pa[p].r, pa[p].g, pa[p].b, 0);
            SDL_Point drawing_points[quantity_drawing_points] = {
                {pa[p].x + pa[p].x * shift_factor , pa[p].y + pa[p].y * shift_factor},
                {pa[p].x + pa[p].x * shift_factor + 1 , pa[p].y + pa[p].y * shift_factor},
                {pa[p].x + pa[p].x * shift_factor , pa[p].y + pa[p].y * shift_factor + 1},
                {pa[p].x + pa[p].x * shift_factor + 1 , pa[p].y + pa[p].y * shift_factor + 1},
            };
            SDL_RenderDrawPoints(renderer,drawing_points,quantity_drawing_points);
        }

        for(int d = 0; d < quantity_destinations; d++){
            SDL_SetRenderDrawColor(renderer, da[d].r, da[d].g, da[d].b, 0);
            SDL_Point drawing_points[quantity_drawing_points] = {
                {da[d].x + da[d].x * shift_factor , da[d].y + da[d].y * shift_factor},
                {da[d].x + da[d].x * shift_factor + 1 , da[d].y + da[d].y * shift_factor},
                {da[d].x + da[d].x * shift_factor , da[d].y + da[d].y * shift_factor + 1},
                {da[d].x + da[d].x * shift_factor + 1 , da[d].y + da[d].y * shift_factor + 1}
            };
            SDL_RenderDrawPoints(renderer,drawing_points,quantity_drawing_points);
        }

        for(int o = 0; o < quantity_obstacles; o++){
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
//#### Grafik

//#### Vorgehen während Iteration
void move_people_sequential(person *persarray, obstacle *obsarray, destination *destarray){
    //cout << "SIND GERADE HIER AM ARBEITEN" << endl;
    //Vector wird mit allen Nummern gefüllt; jede Nummer kann genau einer Person zugeordnet werden kann
    vector<int> serial_number_pers;
    for(int i = 0; i < quantity_persons; i++){
        serial_number_pers.push_back(i);
        //cout << i;
    }
    //cout << endl;
//Bewegung jeder Person:
    for(int j = 0; j < quantity_persons; j++){
    //Aussuchen: wer ist dran?
        int l =  rand() % (quantity_persons - j);
        //cout << "l: " << l << endl;
        int whose_turn = serial_number_pers[l];
        //cout << "whose turn: " << whose_turn << endl;
        serial_number_pers.erase(serial_number_pers.begin() + l);

    //Abrufen: Erstellen der transition Matrix:
    persarray[j].set_T(obsarray,persarray);

    //Aussuchen: Bewegungsrichtung:
    double r = ((rand() % 10000) / 10000.);
    //cout << "Zufallszahl r: " << r << endl;

    if(r < persarray[j].get_T(1,0)){// Bewegung nach oben?
        persarray[j].moveto(persarray[j].x, persarray[j].y - 1);
    }
    else if(r < (persarray[j].get_T(1,0) + persarray[j].get_T(2,1))){//nach rechts?
        persarray[j].moveto(persarray[j].x + 1, persarray[j].y);
    }
    else if(r < (persarray[j].get_T(1,0) + persarray[j].get_T(2,1) + persarray[j].get_T(1,2))){//nach unten?
        persarray[j].moveto(persarray[j].x, persarray[j].y + 1);
    }
    else if(r < (persarray[j].get_T(1,0) + persarray[j].get_T(2,1) + persarray[j].get_T(1,2) + persarray[j].get_T(0,1))){//nach links?
        persarray[j].moveto(persarray[j].x - 1, persarray[j].y);
    }
    else{//stehen bleiben
    }

    }

}
void move_people_parallel(person *persarray, obstacle *obsarray, destination *destarray){
    double r;
    vector<char> direction;
    // Für Jede Person:
    for(int i = 0; i < quantity_persons; i++){
        persarray[i].already_moved = false;
        //Erstellt die transmission Matrix:
        persarray[i].set_T(obsarray,persarray,'p');

        //Erstellt random Zahl r zwischen 0 und 1:
        r = (rand() % 10000) /10000.0;

        //Findet nun heraus, welche Koordinaten unter (desired x, desired y) gespeichert werden sollen:
        if(r < persarray[i].get_T(1,0)){ //nach oben?
            persarray[i].desired_x = persarray[i].x;
            persarray[i].desired_y = persarray[i].y - 1;
            direction.push_back('o');
        }
        else if(r < (persarray[i].get_T(1,0) + persarray[i].get_T(2,1))){//nach rechts?
            persarray[i].desired_x = persarray[i].x + 1;
            persarray[i].desired_y = persarray[i].y;
            direction.push_back('r');
        }
        else if(r < (persarray[i].get_T(1,0) + persarray[i].get_T(2,1) + persarray[i].get_T(1,2))){//nach unten?
            persarray[i].desired_x = persarray[i].x;
            persarray[i].desired_y = persarray[i].y + 1;
            direction.push_back('u');
        }
        else if(r < (persarray[i].get_T(1,0) + persarray[i].get_T(2,1) + persarray[i].get_T(1,2) + persarray[i].get_T(0,1))){//nach links?
            persarray[i].desired_x = persarray[i].x - 1;
            persarray[i].desired_y = persarray[i].y;
            direction.push_back('l');
        }
        else{// stehen bleiben?
            persarray[i].desired_x = persarray[i].x;
            persarray[i].desired_y = persarray[i].y;
            direction.push_back('m');
            }
    }

    //geht alle Personen durch:
    for(int i = 0; i < quantity_persons; i++){
        //Sind die gewünschten Koordinaten der iten Person (desired x, desired y) auch gewünschte Koordinaten einer anderen Person?:
        bool only_one_desired = true;
        persarray[i].conflict_partner.push_back(i);
        for(int j = 0; j < quantity_persons; j++){
            if(persarray[i].desired_x == persarray[j].desired_x && persarray[i].desired_y == persarray[j].desired_y && j != i){
                only_one_desired = false;
                persarray[i].conflict_partner.push_back(j);
            }
        }

        //Hat die ite Person als einzige einen Anspruch auf (desired_x,desired_y)?, dann:
        if(only_one_desired == true){
            if(persarray[i].already_moved == false){
                persarray[i].moveto(persarray[i].desired_x,persarray[i].desired_y);
                persarray[i].already_moved = true;
                persarray[i].conflict_partner.clear();
            }
        }
        else{//Es gibt Konflikte
            if(persarray[i].already_moved == false){
                int x = persarray[i].desired_x;
                int y = persarray[i].desired_y;
                cout << x << ";" << y << endl;
                //initialisiere Konfliktvektor C
                vector<double> C;
                // Fülle Konfliktvektor C; T-Matrix-Wert wird hierfür verwendet
                for(int t = 0; t < persarray[i].conflict_partner.size(); t++){
                    if(direction[persarray[i].conflict_partner[t]] == 'o'){
                        C.push_back(persarray[persarray[i].conflict_partner[t]].get_T(1,0));
                    }
                    if(direction[persarray[i].conflict_partner[t]] == 'r'){
                        C.push_back(persarray[persarray[i].conflict_partner[t]].get_T(2,1));
                    }
                    if(direction[persarray[i].conflict_partner[t]] == 'u'){
                        C.push_back(persarray[persarray[i].conflict_partner[t]].get_T(1,2));
                    }
                    if(direction[persarray[i].conflict_partner[t]] == 'l'){
                        C.push_back(persarray[persarray[i].conflict_partner[t]].get_T(0,1));
                    }
                    if(direction[persarray[i].conflict_partner[t]] == 'm'){
                        C.push_back(persarray[persarray[i].conflict_partner[t]].get_T(1,1));
                    }
                    persarray[persarray[i].conflict_partner[t]].already_moved = true;
                }

                C.push_back(0);
                C.push_back(0);
                C.push_back(0);
                C.push_back(0);
                C.push_back(0);

                int index_max_C = 0;
                int value_max_C = 0;
                for(int c = 0; c < C.size();c++){
                    if (C[c] > value_max_C){
                        value_max_C = C[c];
                        index_max_C = c;
                    }
                    if (C[c] > 0 && persarray[persarray[i].conflict_partner[c]].evacuated == false){
                        persarray[persarray[i].conflict_partner[c]].number_of_conflicts++;
                    }
                }

                persarray[persarray[i].conflict_partner[index_max_C]].moveto(x,y);
                persarray[i].conflict_partner.clear();
            }
            //Erstelle eine Konfliktmatrix:





        }
    }

}
bool has_pers_reached_destination(destination *destarray, person *persarray){//Überprüft ob die Person das Ziel erreicht hat
        bool return_value = false;

        for(int i = 0; i < quantity_persons; i++){
            for(int j = 0; j < quantity_destinations; j++){
                int nh[10] = {//neighbours of the selected destination
                    destarray[j].x, destarray[j].y + 1,
                    destarray[j].x + 1, destarray[j].y,
                    destarray[j].x, destarray[j].y - 1,
                    destarray[j].x - 1, destarray[j].y,
                    destarray[j].x, destarray[j].y
                    };
                for(int k = 0; k < 5; k++){
                    if(persarray[i].x == nh[2*k] && persarray[i].y == nh[2*k + 1] && persarray[i].evacuated == false){//Ist die Person ein Nachbar des Ziels?
                        persarray[i].x = destarray[j].x;//setzt Person in das Feld mit den Koordinaten des Ziels
                        persarray[i].y = destarray[j].y;
                        persarray[i].evacuated = true; //damit sich die Person nicht mehr aus dem Ziel hinausbewegt
                        persarray[i].end_time_measurement();// Stoppt Zeitmessung
                        return_value = true;
                    }
                }
            }
        }
        return return_value;
}
//#### Vorgehen während Iteration

//#### Analyse
void set_model_parameters(person *persarray){//setzt Parameter aller Personen; dies ist für die Analyse der Evakuierungszeit unabdingbar
    double k_S;
    double k_D;
    double w_S;

    cout << "Legen Sie den Einfluss des statischen Feldes k_S fest :" << endl;
    cin >> k_S;
    cout << "Legen Sie den Einfluss des dynamischen Feldes k_D fest :" << endl;
    cin >> k_D;
    cout << "Legen Sie den Wissensstand aller Personen zum Ausgang fest (w_S):" << endl;
    cin >> w_S;

    for(int i = 0; i < quantity_persons; i++){
        persarray[i].k_S = k_S;
        persarray[i].k_D = k_D;
        persarray[i].set_w_S(w_S);
    }
}
void evacuation_analysis(person *persarray){// Analysiert die Evakuierungszeit der Personen
/*
Density
panic
k_D
k_S
w_S - Wissen über das Ziel
omega
*/
}
//#### Analyse



int main(int argc, char* args[]){
    srand (time(NULL));

//################## object declaration 2
//construction of used objects
    for(int o = 0; o < quantity_obstacles; o++){
        obsarray[o] = obstacle(initcoord_obst_array[o][0],initcoord_obst_array[o][1]);
    }
    for(int d = 0; d < quantity_destinations; d++){
        destarray[d] = destination(initcoord_dest_array[d][0],initcoord_dest_array[d][1],obsarray);
    }
    for(int p = 0; p < quantity_persons; p++){
        persarray[p] = person(initcoord_pers_array[p][0],initcoord_pers_array[p][1],destarray);
    }
//################## object declaration 2


//################## visual output 1
    SDL_Event event;
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(grid_width*3, grid_height*3, 0, &window, &renderer);
//################## visual output 1


//test
//test


for(int i = 0; i < number_of_iterations; i++){
//################## iteration method
//cout << "ITERATION: " << i << endl;
    has_pers_reached_destination(destarray,persarray);
    move_people_sequential(persarray,obsarray,destarray);


    for(int j = 0; j < quantity_persons; j++){
        persarray[j].renew_w_S(destarray);
    }
/*//Erneuern des statischen Feldes um auf Umweltänderungen reagieren zu können
    persarray[j].renew_w_S(destarray);
    persarray[j].set_S(destarray);
*/


//################## iteration method



//################## visual output 2
        draw_grid(persarray,destarray,obsarray,renderer,2);
        SDL_Delay(20);
}

    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
//################## visual output 2

}
