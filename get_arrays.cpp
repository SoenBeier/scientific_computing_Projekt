#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <vector>

using namespace std;

const int grid_width = 80;
const int grid_height = 70;


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



Uint32 getpixel(SDL_Surface *surface, int x, int y)
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















main (int argc, char* args[])
{

    gWindow = SDL_CreateWindow( "Grundriss", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, grid_width, grid_height, SDL_WINDOW_SHOWN );
    gScreenSurface = SDL_GetWindowSurface( gWindow );
    gHelloWorld = SDL_LoadBMP( "Haus.bmp" );


    SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
    SDL_UpdateWindowSurface( gWindow );
    SDL_Delay( 2000 );




    get_arrays(gHelloWorld, 0, 0, 2, 0, 1, 0);




    cout << "Destination Array:" << endl;
    cout << " " << endl;
    for (int i=0; i<initcoord_dest_arr.size(); i++)
    {
        for (int j=0; j<ith_coord.size(); j++)
        {
            cout << initcoord_dest_arr[i][j]<< ", ";
        }
        cout << endl;
    }
    cout << "Dest-Array Groesse: " << initcoord_dest_arr.size() << endl;
    cout << " " << endl;
    cout << " " << endl;


    cout << "Obstacle Array:" << endl;
    cout << " " << endl;
    for (int i=0; i<initcoord_obst_arr.size(); i++)
    {
        for (int j=0; j<ith_coord.size(); j++)
        {
            cout << initcoord_obst_arr[i][j]<< ", ";
        }
        cout << endl;
    }
    cout << "Obst-Array Groesse: " << initcoord_obst_arr.size() << endl;
    cout << " " << endl;
    cout << " " << endl;


    cout << "Person Array:" << endl;
    cout << " " << endl;
    for (int i=0; i<initcoord_pers_arr.size(); i++)
    {
        for (int j=0; j<ith_coord.size(); j++)
        {
            cout << initcoord_pers_arr[i][j]<< ", ";
        }
        cout << endl;
    }
    cout << "Pers-Array Groesse: " << initcoord_pers_arr.size() << endl;

    close();


    return 0;

}
