#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>

#include "klassen.cpp"

using namespace std;


//################## initial situation
const int quantity_persons = 3;
const int quantity_destinations = 2;
const int quantity_obstacles = 10;
int number_of_iterations = 10;
const int grid_height = 100;
const int grid_wigth = 150;
int initcoord_pers_array[quantity_persons*2] = {2,2,5,5,5,6};
int initcoord_dest_array[quantity_destinations*2] = {8,8,8,9};
int initcoord_obst_array[quantity_obstacles*2] = {1,1,1,2,1,3,2,3,3,3,3,1,3,2,2,1,2,2,10,10};
//################## initial situation


//################## object declaration 1
//declaration of used objects:
person persarray [quantity_persons]; //contains all objects of type person
destination destarray [quantity_destinations];
obstacle obsarray [quantity_obstacles];
//################## object declaration 1

void clear_drawing(SDL_Renderer *renderer){// clears the whole screen/pigment the whole screen white
    SDL_SetRenderDrawColor(renderer,250,250,250,0);
    const SDL_Rect scrrect = {0,0,grid_wigth,grid_height}; //declarate rectangle, which contains the whole screen
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

int main(int argc, char* args[]){


//################## object declaration 2
//construction of used objects:
//HIER EINFÜGEN WIE NIMMT DAS GANZE DING DIE LISTEN AUF

for(int p = 0; p < quantity_persons; p++){
    persarray[p] = person(initcoord_pers_array[p*2],initcoord_pers_array[p*2+1]);
}
for(int d = 0; d < quantity_destinations; d++){
    destarray[d] = destination(initcoord_dest_array[d*2],initcoord_dest_array[d*2+1]);
}
for(int o = 0; o < quantity_obstacles; o++){
    obsarray[o] = obstacle(initcoord_obst_array[o*2],initcoord_obst_array[o*2+1]);
}
//################## object declaration 2


//################## visual output 1
    SDL_Event event;
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(grid_wigth, grid_height, 0, &window, &renderer);

    // initialisation of visual components correct?:
    if( window == NULL )
    {
        printf( "SDL Fenster konnte nicht geöffnet werden; SDL_Error: %s\n", SDL_GetError() ); //SDL_GETERROR() gibt die die Art des Errors zurück
    }
    if(renderer == NULL)
    {
        printf( "SDL renderer wurde nicht richtig erstellt; SDL_Error : %s\n", SDL_GetError() );
    }

//################## visual output 1

for(int i = 0; i < number_of_iterations; i++){
//################## iteration method
//################## iteration method



//################## visual output 2
        draw_grid(persarray,destarray,obsarray,renderer);
        SDL_Delay(1);

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
