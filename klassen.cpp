#include "initial_situation.cpp"
#include <stddef.h>
#include <iostream>
#include <time.h>

using namespace std;


class obstacle
{
public:
// constructors
    obstacle(){};
    obstacle(int a, int b){
        x = a;
        y = b;
        setrgb(200,200,200);
    };
    obstacle(int a, int b, int f1, int f2, int f3){
        x = a;
        y = b;
        setrgb(f1,f2,f3);
    };
// constructors

// methods
    void setrgb(int f1, int f2, int f3){
        r = f1;
        g = f2;
        b = f3;
    };
    void moveto(int xn, int yn){
        x = xn;
        y = yn;
    }
    bool is_it_here(int qx, int qy){// has the object the coordinates (qx,qy) ?
        if (x == qx && y == qy){
            return true;
        }
        else {
            return false;
        }
    }
// methods

//coordinates
    int x;
    int y;
//colour
    int r;
    int g;
    int b;

private:

};

class destination
{
public:
// constructors
    destination(){};
    destination(int a, int b){
        x = a;
        y = b;
        setrgb(0,200,0);
    };
    destination(int a, int b, int f1, int f2, int f3){
        x = a;
        y = b;
        setrgb(f1,f2,f3);
    };
// constructors

// methods
    void setrgb(int f1, int f2, int f3){
        r = f1;
        g = f2;
        b = f3;
    };
    void moveto(int xn, int yn){
        x = xn;
        y = yn;
    }
    bool is_it_here(int qx, int qy){// has the object the coordinates (qx,qy) ?
        if (x == qx && y == qy){
            return true;
        }
        else {
            return false;
        }
    }
// methods

//coordinates
    int x;
    int y;
//coordinates
//colour
    int r;
    int g;
    int b;
//colour
//Static field Sk, which is created by this destination:
    int S_k[grid_width][grid_height];


    void set_static_field(obstacle obsarray[quantity_obstacles]){
    //time 1
    double time1=0.0, tstart;
    tstart = clock();
    //time 1
        // set all entries of S_k = 0:
        for(int g = 0; g < grid_width; g++){
            for(int h = 0; h< grid_height; h++){
                S_k[g][h] = 0;
            }
        }
        // Use the Manhatten Metric to fill S_k with potentials dependent on the distance the person has to go to the individually cell of S_k:
        int counter = 0;
        set_S_entry(x,y,counter,obsarray);
    //time 2
    time1 += clock() - tstart;
    time1 = time1/CLOCKS_PER_SEC;
    cout << "  time = " << time1 << " sec." << endl;
    //time 2

    }
        void set_S_entry(int sx, int sy, int counter, obstacle obsarray[quantity_obstacles]){

        //cout << "pyth :" << counter << endl;
                //cout << counter << ": grenze: "<< grid_height*grid_height << endl;
        if (counter > (25)){return;}

        int coords_neigbours[8] = {// using Von Neumann neighbourhood
            sx, sy + 1,
            sx, sy - 1,
            sx + 1, sy,
            sx - 1, sy,
        };
        for (int j = 0; j < 4; j++){
            S_k[sx][sy] = counter;
            S_k[x][y] = 0;
            if (((S_k[coords_neigbours[2*j]][coords_neigbours[2*j+1]]) > counter | (S_k[coords_neigbours[2*j]][coords_neigbours[2*j+1]]) == 0 ) & could_a_person_go_to(coords_neigbours[2*j],coords_neigbours[2*j+1], obsarray)){
                //cout << "Ansage:" << could_I_go_to(-1,2,obsarray) << "x: " << obsarray[0].x << "y: " << obsarray[0].y << endl;
                set_S_entry(coords_neigbours[2*j],coords_neigbours[2*j+1],counter + 1, obsarray);
            }
        }
    }


    bool could_a_person_go_to (int qx, int qy, obstacle obsarray[quantity_obstacles]){//delivers true, if the person could stay at (x,y)/ could move to this cell
    //###cells out of borders arent available for a person:
        if ((qy > grid_height) || (qx > grid_width)){
            return false;
        }
        if ((qy < 0) || (qx < 0)){
            return false;
        }
    //###cells filled by an obstacle isnt available for a person:
        bool return_value = true;
        for(int i = 0; i < quantity_obstacles; i++){
            //cout << "x: " << obsarray[i].x << ", y: " << obsarray[i].y << endl;
            if((obsarray[i].x == qx) && (obsarray[i].y == qy)){
                return_value = false;
            }
        }

    // in all other cases:
        return return_value;
    }






private:

};

class person
{
public:
// constructors
    person(){};
    person(int a, int b){
        x = a;
        y = b;
        setrgb(0,0,200);
    };
    person(int a, int b, int f1, int f2, int f3){
        x = a;
        y = b;
        setrgb(f1,f2,f3);
    };
// constructors

// methods
    void setrgb(int f1, int f2, int f3){
        r = f1;
        g = f2;
        b = f3;
    };
    void moveto(int xn, int yn){
        x = xn;
        y = yn;
    }
    bool is_it_here(int qx, int qy){
        if (x == qx && y == qy){
            return true;
        }
        else {
            return false;
        }
    }
    bool could_I_go_to (int qx, int qy, obstacle obsarray[quantity_obstacles]){//delivers true, if the person could stay at (x,y)/ could move to this cell
    //###cells out of borders arent available for a person:
        if ((qy > grid_height) || (qx > grid_width)){
            return false;
        }
        if ((qy < 0) || (qx < 0)){
            return false;
        }
    //###cells filled by an obstacle isnt available for a person:
        bool return_value = true;
        for(int i = 0; i < quantity_obstacles; i++){
            //cout << "x: " << obsarray[i].x << ", y: " << obsarray[i].y << endl;
            if((obsarray[i].x == qx) && (obsarray[i].y == qy)){
                return_value = false;
            }
        }

    // in all other cases:
        return return_value;
    }

// methods

//coordinates
    int x;
    int y;
//colour
    int r;
    int g;
    int b;

// Dynamic floor field D
    int D[grid_width][grid_height];
// Static field S
    double S[grid_width][grid_height];
/*
    void set_static_field(destination destarray[quantity_destinations], obstacle obsarray[quantity_obstacles]){

        int k = 0;
        double S_k[grid_width][grid_height];
        int xd = destarray[k].x;
        int yd = destarray[k].y;

        // set all entries of S_k = 0:
        for(int g = 0; g < grid_width; g++){
            for(int h = 0; h< grid_height; h++){
                S_k[g][h] = 0;
            }
        }
    // Use the Manhatten Metric to fill S_k with potentials dependent on the distance the person has to go to the individually cell of S_k:
        int counter = 0;
        set_S_entry(xd,yd,counter,destarray,S_k,k,obsarray);


        for(int g = 0; g < grid_width; g++){
            for (int h = 0; h < grid_height; h++){
                S[g][h] = S_k[g][h];
                cout << S[g][h] << ";";
            }
            cout << endl;
        }
    }

    void set_S_entry(int x, int y, int counter, destination destarray[quantity_destinations], double S_k[grid_height][grid_height], int k, obstacle obsarray[quantity_obstacles]){
        int xd = destarray[k].x;
        int yd = destarray[k].y;
        //cout << "pyth :" << counter << endl;
                //cout << counter << ": grenze: "<< grid_height*grid_height << endl;
        if (counter > (15)){return;}


        int coords_neigbours[8] = {// using Von Neumann neighbourhood
            x, y + 1,
            x, y - 1,
            x + 1, y,
            x - 1, y,
        };
        for (int j = 0; j < 4; j++){
            S_k[x][y] = counter;
            S_k[xd][yd] = 0;
            if ((S_k[coords_neigbours[2*j]][coords_neigbours[2*j+1]]) > counter || (S_k[coords_neigbours[2*j]][coords_neigbours[2*j+1]] == 0 )){
                //cout << "Ansage:" << could_I_go_to(-1,2,obsarray) << "x: " << obsarray[0].x << "y: " << obsarray[0].y << endl;
                if (could_I_go_to(coords_neigbours[2*j],coords_neigbours[2*j+1], obsarray)){
                    set_S_entry(coords_neigbours[2*j],coords_neigbours[2*j+1],counter + 1, destarray, S_k, k,obsarray);
            }}
        }
    }


*/




private:

};



//person::person(){}
//hindernis::hindernis(){}
//ziel::ziel(){}
