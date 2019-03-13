// FEHLER BEHEBEN : T BESITT KEINEN WERT FÜRS STEHEN BLEIBEN; DA PERs
#ifndef initial_situation
#define initial_situation




const static int grid_height = 100;
const static int grid_width = 100;



static const int quantity_persons = 26;
static const int quantity_destinations = 2;
static const int quantity_obstacles = 14;
static int number_of_iterations = 1000;



static int initcoord_pers_array[quantity_persons][2] = {{5,6},{1,1},{5,18},{5,7},{12,12},{15,15},{18,18},{18,2},{0,0},{70,54},{70,55},{70,56},{70,57},{70,58},{70,59},{70,60},{70,61},{70,62},{70,63},{71,52},{71,53},{71,63},{71,64},{71,65},{71,66},{72,50}};
static int initcoord_dest_array[quantity_destinations][2] = {{13,14},{2,3}};
static int initcoord_obst_array[quantity_obstacles][2] = {{3,4},{4,4},{5,4},{6,4},{7,4},{7,5},{7,6},{7,9},{7,10},{6,10},{5,10},{4,10},{3,10},{3,9}};



#endif




