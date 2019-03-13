#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <vector>

using namespace std;

const int grid_width = 640;
const int grid_height = 480;


SDL_PixelFormat *fmt;
SDL_Window* gWindow;
SDL_Surface* gScreenSurface;
SDL_Surface* gHelloWorld;



vector <vector <int >> initcoord_dest_arr;
vector <vector <int >> initcoord_pers_arr;
vector <vector <int >> initcoord_obst_arr;

vector <int > ith_coord;





void close()
{
	//Deallocate surface
	SDL_FreeSurface( gHelloWorld );
	gHelloWorld = NULL;

	//initcoord_dest_vectorroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}






void get_arrays()
{
    fmt = gScreenSurface->format;

    Uint8 r=0;
    Uint8 g=0;
    Uint8 b=0;

    //rgb der ersonen
    Uint8 r_p=0;
    Uint8 g_p=0;
    Uint8 b_p=0;

    //rgb der obst
    Uint8 r_o=0;
    Uint8 g_o=0;
    Uint8 b_o=0;

    //rgb der dest
    Uint8 r_d=0;
    Uint8 g_d=0;
    Uint8 b_d=0;

    //void SDL_GetRGB(Uint32 pixel, SDL_PixelFormat *fmt, Uint8 *r, Uint8 *g, Uint8 *b);

    Uint32 pixel = 0;

    for (int y=0; y< grid_height; y++)
    {
        for (int x=0; x< grid_width; x++)
        {
            int bpp = gScreenSurface->format->BytesPerPixel;
            /* Here p is the address to the pixel we want to retrieve */
            Uint8* p = (Uint8 *)gScreenSurface->pixels + y * gScreenSurface->pitch + x * bpp;

            switch(bpp)
            {
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
            pixel=p;
            SDL_GetRGB(pixel, fmt, &r, &g, &b);

            if (r==r_p && g==g_p && b==b_p)
            {
                ith_coord.push_back(x);
                ith_coord.push_back(y);

                initcoord_pers_arr.push_back(ith_coord);
            }
            if (r==r_o && g==g_o && b==b_o)
            {
                ith_coord.push_back(x);
                ith_coord.push_back(y);

                initcoord_obst_arr.push_back(ith_coord);
            }
            if (r==r_d && g==g_d && b==b_d)
            {
                ith_coord.push_back(x);
                ith_coord.push_back(y);

                initcoord_dest_arr.push_back(ith_coord);
            }
        }
    }
}













main (int argc, char* args[])
{

    gWindow = SDL_CreateWindow( "Geladenes Bild", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, grid_width, grid_height, SDL_WINDOW_SHOWN );
    gScreenSurface = SDL_GetWindowSurface( gWindow );
    gHelloWorld = SDL_LoadBMP( "Haus.bmp" );



    get_arrays();


    SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
    SDL_UpdateWindowSurface( gWindow );
    SDL_Delay( 2000 );

    close();
}
