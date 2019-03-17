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





//#### Grundriss einlesen


//#### Grundriss einlesen


//#### Grafikausgabe
void clear_drawing(SDL_Renderer *renderer){// clears the whole screen/pigment the whole screen white //NOCHMAL DURCH RICHTIGEN BEFEHL ERSETZEN
    SDL_SetRenderDrawColor(renderer,250,250,250,0);
    const SDL_Rect scrrect = {0,0,grid_width*10,grid_height*10}; //declarate rectangle, which contains the whole screen -> NOCHMAL NACHBESSERN : NUR BENÖTIGTER PLATZ
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
void draw_grid(vector <person> &pa, vector <destination> &da, vector<obstacle> &oa, SDL_Renderer *renderer, int magnification_factor){//draw all objects on screen with a magnication ; möglicherweise eine größere Vergrößerung ermöglichen ?

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

//#### Vorgehen während Iteration
void move_people_sequential(vector<person> &persvec, vector<obstacle> &obstvec, vector<destination> &destvec){
    //cout << "SIND GERADE HIER AM ARBEITEN" << endl;
    //Vector wird mit allen Nummern gefüllt; jede Nummer kann genau einer Person zugeordnet werden kann
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
    double r = ((rand() % 10000) / 10000.);
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
void move_people_parallel(vector<person> &persvec, vector<obstacle> &obstvec, vector<destination> &destvec){
    double r;
    vector<char> direction;
    // Für Jede Person:
    for(int i = 0; i < persvec.size(); i++){
        persvec[i].already_moved = false;
        //Erstellt die transmission Matrix:
        persvec[i].set_T(obstvec,persvec,'p');

        //Erstellt random Zahl r zwischen 0 und 1:
        r = (rand() % 10000) /10000.0;

        //Findet nun heraus, welche Koordinaten unter (desired x, desired y) gespeichert werden sollen:
        if(r < persvec[i].get_T(1,0)){ //nach oben?
            persvec[i].desired_x = persvec[i].x;
            persvec[i].desired_y = persvec[i].y - 1;
            direction.push_back('o');
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
            direction.push_back('m');
            }
    }

    //geht alle Personen durch:
    for(int i = 0; i < persvec.size(); i++){
        //Sind die gewünschten Koordinaten der iten Person (desired x, desired y) auch gewünschte Koordinaten einer anderen Person?:
        bool only_one_desired = true;
        persvec[i].conflict_partner.push_back(i);
        for(int j = 0; j < persvec.size(); j++){
            if(persvec[i].desired_x == persvec[j].desired_x && persvec[i].desired_y == persvec[j].desired_y && j != i){
                only_one_desired = false;
                persvec[i].conflict_partner.push_back(j);
            }
        }

        //Hat die ite Person als einzige einen Anspruch auf (desired_x,desired_y)?, dann:
        if(only_one_desired == true){
            if(persvec[i].already_moved == false){
                persvec[i].moveto(persvec[i].desired_x,persvec[i].desired_y);
                persvec[i].already_moved = true;
                persvec[i].conflict_partner.clear();
            }
        }
        else{//Es gibt Konflikte
            if(persvec[i].already_moved == false){
                int x = persvec[i].desired_x;
                int y = persvec[i].desired_y;
                cout << x << ";" << y << endl;
                //initialisiere Konfliktvektor C
                vector<double> C;
                // Fülle Konfliktvektor C; T-Matrix-Wert wird hierfür verwendet
                for(int t = 0; t < persvec[i].conflict_partner.size(); t++){
                    if(direction[persvec[i].conflict_partner[t]] == 'o'){
                        C.push_back(persvec[persvec[i].conflict_partner[t]].get_T(1,0));
                    }
                    if(direction[persvec[i].conflict_partner[t]] == 'r'){
                        C.push_back(persvec[persvec[i].conflict_partner[t]].get_T(2,1));
                    }
                    if(direction[persvec[i].conflict_partner[t]] == 'u'){
                        C.push_back(persvec[persvec[i].conflict_partner[t]].get_T(1,2));
                    }
                    if(direction[persvec[i].conflict_partner[t]] == 'l'){
                        C.push_back(persvec[persvec[i].conflict_partner[t]].get_T(0,1));
                    }
                    if(direction[persvec[i].conflict_partner[t]] == 'm'){
                        C.push_back(persvec[persvec[i].conflict_partner[t]].get_T(1,1));
                    }
                    persvec[persvec[i].conflict_partner[t]].already_moved = true;
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
                    if (C[c] > 0 && persvec[persvec[i].conflict_partner[c]].evacuated == false){
                        persvec[persvec[i].conflict_partner[c]].number_of_conflicts++;
                    }
                }

                persvec[persvec[i].conflict_partner[index_max_C]].moveto(x,y);
                persvec[i].conflict_partner.clear();
            }
            //Erstelle eine Konfliktmatrix:





        }
    }

}
bool has_pers_reached_destination(vector<destination> &destvec, vector<person> &persvec){//Überprüft ob die Person das Ziel erreicht hat
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
                        persvec[i].end_time_measurement();// Stoppt Zeitmessung
                        return_value = true;
                    }
                }
            }
        }
        return return_value;
}
//#### Vorgehen während Iteration

//#### Analyse
void set_model_parameters(vector<person> &persvec){//setzt Parameter aller Personen; dies ist für die Analyse der Evakuierungszeit unabdingbar
    double k_S;
    double k_D;
    double w_S;

    cout << "Legen Sie den Einfluss des statischen Feldes k_S fest :" << endl;
    cin >> k_S;
    cout << "Legen Sie den Einfluss des dynamischen Feldes k_D fest :" << endl;
    cin >> k_D;
    cout << "Legen Sie den Wissensstand aller Personen zum Ausgang fest (w_S):" << endl;
    cin >> w_S;

    for(int i = 0; i < persvec.size(); i++){
        persvec[i].k_S = k_S;
        persvec[i].k_D = k_D;
        persvec[i].set_w_S(w_S);
    }
}
void evacuation_analysis(vector<person> &persvec){// Analysiert die Evakuierungszeit der Personen
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

static const int quantity_persons = 1;
static const int quantity_destinations = 2;
static const int quantity_obstacles = 14;

static int initcoord_pers_array[quantity_persons][2] = {{15,2}};
static int initcoord_dest_array[quantity_destinations][2] = {{13,14},{2,3}};
static int initcoord_obst_array[quantity_obstacles][2] = {{3,4},{4,4},{5,4},{6,4},{7,4},{7,5},{7,6},{7,9},{7,10},{6,10},{5,10},{4,10},{3,10},{3,9}};

//################## object declaration 1
//declaration of used objects:
vector<person> persvec;
persvec.resize(quantity_persons);

vector<destination> destvec;
destvec.resize(quantity_destinations);

vector<obstacle> obstvec;
obstvec.resize(quantity_obstacles);
//################## object declaration 1


//################## object declaration 2
//construction of used objects
    for(int o = 0; o < quantity_obstacles; o++){
        obstvec[o] = obstacle(initcoord_obst_array[o][0],initcoord_obst_array[o][1],quantity_obstacles,quantity_destinations,quantity_persons);
    }
    for(int d = 0; d < quantity_destinations; d++){
        destvec[d] = destination(initcoord_dest_array[d][0],initcoord_dest_array[d][1],obstvec,quantity_obstacles,quantity_destinations,quantity_persons);
    }
    for(int p = 0; p < quantity_persons; p++){
        persvec[p] = person(initcoord_pers_array[p][0],initcoord_pers_array[p][1],destvec,quantity_obstacles,quantity_destinations,quantity_persons);
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
    cout << i << endl;
//################## iteration method
//cout << "ITERATION: " << i << endl;
    has_pers_reached_destination(destvec,persvec);
    move_people_sequential(persvec,obstvec,destvec);


    for(int j = 0; j < quantity_persons; j++){
        persvec[j].renew_w_S(destvec);
    }

/*//Erneuern des statischen Feldes um auf Umweltänderungen reagieren zu können
    persarray[j].renew_w_S(destarray);
    persarray[j].set_S(destarray);
*/


//################## iteration method



//################## visual output 2
        draw_grid(persvec,destvec,obstvec,renderer,2);
        SDL_Delay(200);
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
