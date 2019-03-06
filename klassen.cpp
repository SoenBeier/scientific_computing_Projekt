#include "initial_situation.cpp"
#include <stddef.h>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <vector>

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
    destination(int a, int b,obstacle* obsarray){
        x = a;
        y = b;
        setrgb(0,200,0);
        set_static_field_k3(obsarray);
    };
    destination(int a, int b, int f1, int f2, int f3,obstacle* obsarray){
        x = a;
        y = b;
        setrgb(f1,f2,f3);
        set_static_field_k3(obsarray);
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
    int counter_call = 0;
    int counter_global = 0;

    void set_static_field_k1(obstacle obsarray[quantity_obstacles]){
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
        set_S_entry2(x,y,counter,obsarray,true,true,true,true);
        //set_S_entry(x,y,counter,obsarray);



    //time 2
    time1 += clock() - tstart;
    time1 = time1/CLOCKS_PER_SEC;
    cout << "  time = " << time1 << " sec." << endl;
    //time 2

    }
    //declare a vector, which contains all positions of entries, which has to be set with set_S_entry:
    vector<int> qns; //"queue next steps"
    void set_static_field_k2(obstacle obsarray[quantity_obstacles]){//Max_counter anpassen !!
    //time 1
    double time1=0.0, tstart;
    tstart = clock();
    //time 1

        int max_counter = 15;
        // set all entries of S_k = 0:
        for(int g = 0; g < grid_width; g++){
            for(int h = 0; h< grid_height; h++){
                S_k[g][h] = 0;
            }
        }
        // Use the Manhatten Metric to fill S_k with potentials dependent on the distance the person has to go to the individually cell of S_k:



        //set start condition:
        qns.push_back(x);
        qns.push_back(y);

        //

        int qns_n;

        for (int c = 0; c < max_counter; c++){

            qns_n = qns.size() / 2;

            if(qns_n == 0){return;}
            int i = 0;
            while(i < qns_n && i < 190000){

                S_k[qns[0]][qns[1]] = c;

                find_neigbours(qns[0],qns[1],c,obsarray,qns_n);

                qns.erase(qns.begin());
                qns.erase(qns.begin());

                i++;
        }}


    //time 2
    time1 += clock() - tstart;
    time1 = time1/CLOCKS_PER_SEC;
    cout << "  time = " << time1 << " sec." << endl;
    //time 2

    }
    void set_static_field_k3(obstacle obsarray[quantity_obstacles]){//ist am schnellsten
    //time 1
    double time1=0.0, tstart;
    tstart = clock();
    //time 1

        int max_counter = 300;
        // set all entries of S_k = 0:
        for(int g = 0; g < grid_width; g++){
            for(int h = 0; h< grid_height; h++){
                S_k[g][h] = 0;
            }
        }
        // Use the Manhatten Metric to fill S_k with potentials dependent on the distance the person has to go to the individually cell of S_k:

        //set start condition:
        qns.push_back(x);
        qns.push_back(y);
        //

        int qns_n;
        int i = 0;
        for (int c = 0; c < max_counter; c++){

            qns_n = qns.size() / 2;
            cout << c << ";" << qns_n*2 << endl;


            int q = 0;

            while(i < qns_n && i < (qns.max_size() -10)){

                S_k[qns[2*i]][qns[2*i + 1]] = c;

                find_neigbours(qns[2*i],qns[2*i + 1],c,obsarray, qns_n);

                i++;
        }}


    //time 2
    time1 += clock() - tstart;
    time1 = time1/CLOCKS_PER_SEC;
    cout << "  time = " << time1 << " sec." << endl;
    //time 2

    }
    void find_neigbours(int sx, int sy, int counter,  obstacle obsarray[quantity_obstacles], int qns_n){

        int coords_neigbours[8] = {// using Von Neumann neighbourhood
            sx, sy + 1,
            sx, sy - 1,
            sx + 1, sy,
            sx - 1, sy,
        };

        bool uebergeben = true;
        for(int j = 0; j < 4; j++){
            if (((S_k[coords_neigbours[2*j]][coords_neigbours[2*j+1]]) > counter || (S_k[coords_neigbours[2*j]][coords_neigbours[2*j+1]]) == 0 ) && could_a_person_go_to(coords_neigbours[2*j],coords_neigbours[2*j+1], obsarray) &&
                (coords_neigbours[2*j] != x || coords_neigbours[2*j+1] != y)){
                for(int i = 0 /*50000*grid_height*/; i < qns.size()/2 ; i++){// HIER HAUT ES EINEN FEHLER REIN
                    if((coords_neigbours[2*j] == qns[2*i]) && (coords_neigbours[2*j + 1] == qns[2*i + 1])){
                        uebergeben = false;
                    }
                }
                if(uebergeben == true){
                    qns.push_back(coords_neigbours[2*j]);
                    qns.push_back(coords_neigbours[2*j+1]);
                }
            }
        }

    }
    void set_S_entry(int sx, int sy, int counter, obstacle obsarray[quantity_obstacles]){

        //cout << "pyth :" << counter << endl;
                //cout << counter << ": grenze: "<< grid_height*grid_height << endl;
        if (counter > (20)){return;}

        int coords_neigbours[8] = {// using Von Neumann neighbourhood
            sx, sy + 1,
            sx, sy - 1,
            sx + 1, sy,
            sx - 1, sy,
        };

        S_k[sx][sy] = counter; // the entry of S_k at the position (sx,sy) becomes the new potential
        S_k[x][y] = 0; //probably the entry at the cell with the coordinates of the destination is overwritten; so we have to correct the entry

        for (int j = 0; j < 4; j++){
            if (((S_k[coords_neigbours[2*j]][coords_neigbours[2*j+1]]) > counter || (S_k[coords_neigbours[2*j]][coords_neigbours[2*j+1]]) == 0 ) && could_a_person_go_to(coords_neigbours[2*j],coords_neigbours[2*j+1], obsarray)){// if the entry of the neigbourcell is larger than the counter or 0 and could a person go to this coordinates, the program will continue with:
                //cout << "Ansage:" << could_I_go_to(-1,2,obsarray) << "x: " << obsarray[0].x << "y: " << obsarray[0].y << endl;
                counter_call++;
                set_S_entry(coords_neigbours[2*j],coords_neigbours[2*j+1],counter + 1, obsarray);

            }
        }
    }
    void set_S_entry2(int sx, int sy, int counter, obstacle obsarray[quantity_obstacles], bool up, bool down, bool right, bool left){// set one entry of S_k and call itself afterwards, with new coordinates sx,sy ; if it isn't worth to go to this side the parameters up,down,right,left will be false

        if (counter > (25)){
            return;
        }

        counter_call++;
        int coords_neigbours[8] = {// using Von Neumann neighbourhood
            sx, sy + 1,
            sx, sy - 1,
            sx + 1, sy,
            sx - 1, sy,
        };

        S_k[sx][sy] = counter; // the entry of S_k at the position (sx,sy) becomes the new potential
        //S_k[x][y] = 0; //probably the entry at the cell with the coordinates of the destination is overwritten; so we have to correct the entry




        for (int j = 0; j < 4; j++){

            // ###Zeitersparnis
            //wenn der Wert des Potentials in eine Richtung schon niedriger als der Counter selbst ist, so muss man nicht weiter in diese Richtung gehen und überprüfen ob es sinn macht in diese Richtung zu gehen
            if ((S_k[coords_neigbours[2*j]][coords_neigbours[2*j+1]] + 1 <= counter) && S_k[coords_neigbours[2*j]][coords_neigbours[2*j+1]] != 0){
                if(j == 0){up = false;}
                if(j == 1){down = false;}
                if(j == 2){right = false;}
                if(j == 3){left = false;}
            }
            //cout << up << ";" << down<<";"<< right << ";" << left << endl;
            //springt ans Ende der for Schleife wenn es keinen Sinn macht in die jeweilige Richtung zu gehen
            if ((up == false && j == 0 )||(down == false && j == 1)||(right == false && j == 2)||(left == false && j == 3) ){continue;}
            //###Zeitersparnis

            //cout << up << ";" << down << ";" << right << ";" << left << endl;
                if (((S_k[coords_neigbours[2*j]][coords_neigbours[2*j+1]]) > counter || (S_k[coords_neigbours[2*j]][coords_neigbours[2*j+1]]) == 0 ) && could_a_person_go_to(coords_neigbours[2*j],coords_neigbours[2*j+1], obsarray) && (coords_neigbours[2*j] != x || coords_neigbours[2*j+1] != y)){// if the entry of the neigbourcell is larger than the counter or 0 and could a person go to this coordinates and this coordinates arent the coordinates of the destination, the program will continue with:
                    //cout << "Ansage:" << could_I_go_to(-1,2,obsarray) << "x: " << obsarray[0].x << "y: " << obsarray[0].y << endl;


                    set_S_entry2(coords_neigbours[2*j],coords_neigbours[2*j+1],counter + 1, obsarray,up,down,right,left);
                }
        }
    }
    bool could_a_person_go_to (int qx, int qy, obstacle obsarray[quantity_obstacles]){//delivers true, if the person could stay at (x,y)/ could move to this cell
    //###cells out of borders arent available for a person:
        if ((qy >= grid_height) || (qx >= grid_width)){
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





private:

};



//person::person(){}
//hindernis::hindernis(){}
//ziel::ziel(){}
