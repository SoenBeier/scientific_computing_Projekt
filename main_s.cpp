#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
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


void clear_drawing(SDL_Renderer *renderer){// clears the whole screen/pigment the whole screen white //NOCHMAL DURCH RICHTIGEN BEFEHL ERSETZEN
    SDL_SetRenderDrawColor(renderer,250,250,250,0);
    const SDL_Rect scrrect = {0,0,grid_width*10,grid_height*10}; //declarate rectangle, which contains the whole screen -> NOCHMAL NACHBESSERN : NUR BENÖTIGTER PLATZ
    SDL_RenderFillRect(renderer, &scrrect);
}
void draw_grid(person pa[quantity_persons], destination da[quantity_destinations], obstacle oa[quantity_obstacles], SDL_Renderer *renderer){ //draw all objects on screen

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
void draw_grid(person pa[quantity_persons], destination da[quantity_destinations], obstacle oa[quantity_obstacles], SDL_Renderer *renderer, int magnification_factor){//draw all objects on screen with a magnication ; möglicherweise eine größere Vergrößerung ermöglichen ?

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



int main(int argc, char* args[]){


//################## object declaration 2
//construction of used objects:
//HIER EINFÜGEN WIE NIMMT DAS GANZE DING DIE LISTEN AUF


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
destarray[0].print_S_k();
destarray[1].print_S_k();
persarray[0].print_S();
//test



for(int i = 0; i < number_of_iterations; i++){
//################## iteration method
for(int h = 0; h < quantity_persons; h++){
    int new_x = rand() % grid_width;
    int new_y = rand() % grid_height;
    persarray[h].moveto(new_x , new_y);
    cout << new_x << " ; " << new_y << endl;
}
cout << "-------------------------------------" << endl;
//################## iteration method



//################## visual output 2
        draw_grid(persarray,destarray,obsarray,renderer,2);
        SDL_Delay(100);

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
