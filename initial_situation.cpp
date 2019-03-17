// FEHLER BEHEBEN : T BESITT KEINEN WERT FÜRS STEHEN BLEIBEN; DA PERs
#ifndef initial_situation
#define initial_situation
#include <SDL.h>
#include <SDL_image.h>
#include <vector>



using namespace std;

const static int grid_height = 75;
const static int grid_width = 75;


class initialisation_data {

public:
initialisation_data(){};

vector <vector <int >> initcoord_dest_vec;
vector <vector <int >> initcoord_pers_vec;
vector <vector <int >> initcoord_obst_vec;


void set_vectors(SDL_Surface * surface, int p_x, int p_y, int d_x, int d_y, int o_x, int o_y){//Liest den Grundriss ein
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
int set_quantity_persons(){
    return initcoord_pers_vec.size();
}
int set_quantity_destinations(){
    return initcoord_dest_vec.size();
}
int set_quantity_obstacles(){
    return initcoord_obst_vec.size();
}




private:

};



static int number_of_iterations = 10;










#endif




