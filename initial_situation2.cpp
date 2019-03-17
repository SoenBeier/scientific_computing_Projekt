#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <vector>

using namespace std;
const static int grid_width = 25;
const static int grid_height = 25;

    vector <vector <int >> initcoord_dest_arr;
    vector <vector <int >> initcoord_pers_arr;
    vector <vector <int >> initcoord_obst_arr;

    vector <int > ith_coord;

Uint32 getpixel(SDL_Surface *surface, int x, int y) //Quelle: http://sdl.beuc.net/sdl.wiki/Pixel_Access
{
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


void get_arrays(SDL_Surface * surface, int p_x, int p_y, int d_x, int d_y, int o_x, int o_y)
{
    for (int y=0; y< grid_height; y++)
    {
        for (int x=3; x< grid_width; x++)
        {
            if (getpixel(surface, x, y)==getpixel(surface, p_x, p_y))
            {
                ith_coord.clear();
                ith_coord.push_back(x);
                ith_coord.push_back(y);

                initcoord_pers_arr.push_back(ith_coord);
            }
            else if (getpixel(surface, x, y)==getpixel(surface, o_x, o_y))
            {
                ith_coord.clear();
                ith_coord.push_back(x);
                ith_coord.push_back(y);

                initcoord_obst_arr.push_back(ith_coord);
            }
            else if (getpixel(surface, x, y)==getpixel(surface, d_x, d_y))
            {
                ith_coord.clear();
                ith_coord.push_back(x);
                ith_coord.push_back(y);

                initcoord_dest_arr.push_back(ith_coord);
            }
        }
    }
}

int mains(int argc, char* args[]){
//Initialisation
    SDL_Window* Window;
    SDL_Surface* screen_surface;
    SDL_Surface* bmp_surf;
    SDL_PixelFormat *fmt;
    SDL_Init( SDL_INIT_VIDEO );
    Window = SDL_CreateWindow( "", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, grid_width, grid_height, SDL_WINDOW_SHOWN );
    screen_surface = SDL_GetWindowSurface( Window );
    bmp_surf = SDL_LoadBMP( "Haus.bmp" );
//Show picture
    SDL_BlitSurface( bmp_surf, NULL, screen_surface, NULL );
    SDL_UpdateWindowSurface( Window );
    SDL_Delay( 1000 );
//get start conditions
    get_arrays(bmp_surf,2,0,1,0,0,0);
//Close
    SDL_FreeSurface( bmp_surf );
	bmp_surf = NULL;
	SDL_DestroyWindow( Window );
	Window = NULL;
	SDL_Quit();

    for(int i = 0; i < initcoord_dest_arr.size(); i++){
        for(int j = 0; j < initcoord_dest_arr[i].size(); j++ ){
            cout << initcoord_dest_arr[i][j];
        }
    }


    return EXIT_SUCCESS;
}
