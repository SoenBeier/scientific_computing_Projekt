//#################SDL######################
#include <SDL.h>//SDL
#include <SDL_image.h>
#include <stdio.h>//alternative iostream

//Bildschirm-Dimensionen
const int BILDSCHIRM_BREITE = 600;
const int BILDSCHIRM_HOEHE = 600;
//###########################################

//#include "klassen.h"
#include "klassen.cpp"
#include <iostream>
#include <stdlib.h> //für random-Zahlen


//Erstellt Informationen zum Spielfeld; wird zur Ausgabe des Feldes benutzt
const int stretching_points = 9;
const int sqrt_s_p = 3;

const int feldbreite = BILDSCHIRM_BREITE/sqrt_s_p;
const int feldhoehe = BILDSCHIRM_HOEHE/sqrt_s_p;
int feld[feldbreite][feldhoehe];



//Erstellt Array mit allen Personen, Hindernissen und Zielen
const int anzahl_personen = 10;
const int anzahl_hindernisse = 10;
const int anzahl_ziele = 1;

person persarray [anzahl_personen];
hindernis hinarray [anzahl_hindernisse];
ziel zarray [anzahl_ziele];








using namespace std;



void feldeingabe(int arr[feldbreite][feldhoehe])
{

    /*
    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }

    gTexture = loadTexture( "Haus.png" ); //Name BILD
	if( gTexture == NULL ){printf( "Fehler beim Laden des Bildes" );}

    SDL_Texture* loadTexture( std::string path );
    SDL_Texture* gTexture = NULL; */

}

void eintragzeichnen(SDL_Renderer *renderer,int r, int g, int b, int alpha, int x, int y, bool aktuallisieren)//Zeichnet einen Arrayeintrag
{

    SDL_Point points[stretching_points] =
    {
        {x + sqrt_s_p * x , y + sqrt_s_p * y},
        {x + sqrt_s_p * x , y + sqrt_s_p * y + 1},
        {x + sqrt_s_p * x , y + sqrt_s_p * y + 2},
        {x + sqrt_s_p * x + 1 ,y + sqrt_s_p * y},
        {x + sqrt_s_p * x + 1 , y + sqrt_s_p * y + 1},
        {x + sqrt_s_p * x + 1 , y + sqrt_s_p * y + 2},
        {x + sqrt_s_p * x + 2 , y + sqrt_s_p * y},
        {x + sqrt_s_p * x + 2 ,y + sqrt_s_p * y + 1},
        {x + sqrt_s_p * x + 2 ,y + sqrt_s_p * y + 2},
    }; // bei einer Änderung des stretching-Verfahrens muss die feldbreite/feldhoehe oben angepasst werden / entfällt
    SDL_SetRenderDrawColor(renderer,r,g,b,alpha);
    //SDL_RenderDrawPoint(renderer,x,y);
    SDL_RenderDrawPoints(renderer,points,stretching_points);
    if(aktuallisieren == true){SDL_RenderPresent(renderer);}


}


void random_intarray(int arr[feldbreite][feldhoehe], int max_h, int max_b){//nimmt das initiallisierte Array und fügt dem Bereich [0,0] bis [max_h,max_b] zufällige int Werte hinzu
    for(int h=0; h<max_h; h++){
        for(int b=0; b<max_b; b++){
            arr[b][h] = rand() % 3;
        }
    }

}

void feldzeichnen(int arr[feldbreite][feldhoehe], int b, int h, SDL_Renderer *renderer)
{
    for(int i = 0; i < b; i++){
        for(int j = 0; j < h; j++){

            if (i == b - 1 and j == h - 1) { eintragzeichnen(renderer,arr[i][j]*75,arr[i][j]*75,arr[i][j]*75,arr[i][j],i,j,true); }
            else {eintragzeichnen(renderer,arr[i][j]*75,arr[i][j]*80,arr[i][j]*50,arr[i][j],i,j,false);}
        }
    }
}


int main(int argc, char* args[])
{
    SDL_Event event;
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(BILDSCHIRM_BREITE, BILDSCHIRM_HOEHE, 0, &window, &renderer);

    if( window == NULL )
    {
        printf( "SDL Fenster konnte nicht geöffnet werden; SDL_Error: %s\n", SDL_GetError() ); //SDL_GETERROR() gibt die die Art des Errors zurück
    }
    if(renderer == NULL)
    {
        printf( "SDL renderer wurde nicht richtig erstellt; SDL_Error : %s\n", SDL_GetError() );
    }

    //erstellt die im Feld befindlichen Personen, hindernisse, ziele
    for(int o = 0; o < anzahl_personen; o++){
        persarray[o] = person(o+20,o*2+20) ;
        cout << persarray[o].x << ";" << persarray[o].y << endl; // gibt Variablen der Klassen aus
    }
    for(int p = 0; p < anzahl_hindernisse; p++){
        hinarray[p] = hindernis(6,3) ;
    }
    for(int q = 0; q < anzahl_ziele; q++){
            zarray[q] = ziel(7,2) ;
    }


    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);//r,g,b,alpha
    //SDL_RenderClear(renderer);

    //eintragzeichnen(renderer,50,100,150,100,100,100,true);

    for (int k = 0; k < 50; k++){
        random_intarray(feld,feldhoehe,feldbreite);
        feldzeichnen(feld,feldbreite,feldhoehe,renderer);
        SDL_Delay(1);
    }

    //SDL_SetRenderDrawColor(renderer, 199, 21, 133, 255);//r,g,b,alpha
    //SDL_RenderDrawPoint(renderer, 100, 100);
    //SDL_RenderPresent(renderer);
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
    return 0;
}



