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
        set_static_field_k(obsarray);
    };
    destination(int a, int b, int f1, int f2, int f3,obstacle* obsarray){
        x = a;
        y = b;
        setrgb(f1,f2,f3);
        set_static_field_k(obsarray);
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
    vector<int> to_do;
    vector<int> processed;
    void set_static_field_k(obstacle obsarray[quantity_obstacles]){
        //Setzt alle Einträge von S_k auf 0
        for(int g = 0; g < grid_width; g++){
            for(int h = 0; h< grid_height; h++){
                S_k[g][h] = 0;
            }
        }
        //Trägt Anfangskoorinaten in den vector ein
        to_do.push_back(x);
        to_do.push_back(y);

        int counter = 0;

        while(true){
            // sichert die aktuelle Größe des vectors to_do für die nächste for-Schleife
            int ntodo = to_do.size();
            if (ntodo % 2 != 0){//Fehleranzeige, falls was schief läuft
                    cout << "Ein Fehler ist augetreten! die Anzahl ntodo ist nicht durch zwei teilbar." << endl;
                }
            for(int j = 0; j < (ntodo / 2); j++){

                //Schriebe das zugehörige Potential in das statische Feld:
                S_k[to_do[0]][to_do[1]] = counter;
                //Diese Koordinaten sind nun abgearbeitet und werden dem vector processed übergeben
                processed.push_back(to_do[0]);
                processed.push_back(to_do[1]);
                //Suche Nachbarn für das Feld S_k mit den Koordinaten (to_do[0],to_do[1}) und schreibe diese in den vector to_do
                int nh[8] = {//sind alle möglichen Nachbarn
                    to_do[0],to_do[1] + 1,
                    to_do[0] + 1,to_do[1],
                    to_do[0],to_do[1] - 1,
                    to_do[0] - 1,to_do[1]
                    };
                for (int l = 0; l < 4; l++){//für jeden möglichen Nachbarn folgt:
                    if (could_a_person_go_to(nh[2*l],nh[2*l+1],obsarray) == false){// wenn eine Person hier nicht drauf darf
                        cout << "Nicht erlaubte Felder could: " << nh[2*l]<< ";" << nh[2*l+1] <<endl;
                        continue;
                    }
                    //Ist der aktuell ausgewählte Nachbar schon im Vector processed oder to_do enthalten:
                    int con = false;
                    for(int a = 0; a < processed.size()/2; a++){
                        if (processed[2*a] == nh[2*l] && processed[2*a+1] == nh[2*l+1]){
                            con = true;
                        }
                    }
                    for(int b = 1; b < to_do.size()/2; b++){
                        if(to_do[2*b] == nh[2*l] && to_do[2*b+1] == nh[2*l+1]){
                            con = true;
                        }
                    }
                    if(con == true){
                        continue;
                    }
                    to_do.push_back(nh[2*l]);
                    to_do.push_back(nh[2*l+1]);
                }

                //Lösche nun die bearbeiteten Koordinaten (to_do[0],to_do[1])
                to_do.erase(to_do.begin());
                to_do.erase(to_do.begin());
            }
            if(to_do.size() == 0){
                break;
            }
        counter++;
        }

    }
    int get_S_k(int x, int y){
        return S_k[x][y];
    }
    void print_S_k(){
        cout << "----------------------------------------------------------------" << endl;
        for(int j = 0; j < grid_height; j++){
            for(int i = 0; i < grid_width; i++){

        if (S_k[i][j] > 9 && S_k[i][j] <= 99){cout << " " << S_k[i][j] << ";" ;}
        else if (S_k[i][j] > 99){cout << S_k[i][j] << ";" ;}
        else {cout << "  " << S_k[i][j] << ";" ;}

            }
        cout << endl;
        }
    }



private:

};

class person
{
public:
// constructors
    person(){};
    person(int a, int b,destination destarray[quantity_destinations]){
        x = a;
        y = b;
        setrgb(0,0,200);
        set_w_S();
        set_S(destarray);
    };
    person(int a, int b, int f1, int f2, int f3,destination destarray[quantity_destinations]){
        x = a;
        y = b;
        setrgb(f1,f2,f3);
        set_w_S();
        set_S(destarray);
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
    double w_S[quantity_destinations]; // wie sehr kennt die Person die verschiedenen Eingänge; Eintrag ist zwischen 0,1
    double S[grid_width][grid_height];

    void set_w_S(){
        for(int i = 0; i < quantity_destinations ; i++){
            w_S[i] = (double)(rand() % 10) / 10;
            cout <<"w_S ist:" <<w_S[i] << endl;
        }
    }
    void set_S(destination destarray[quantity_destinations]){//Addiere alle S_k Arrays der einzelnen destinations zum S Array hinzu; dies verläuft nach Gewichtung
        for(int xi = 0; xi < grid_width; xi++){//setze alle Einträge von S auf 0
            for(int yi = 0; yi < grid_height; yi++){
                S[xi][yi] = 0;
            }
        }


        for(int l = 0; l < quantity_destinations; l++){//Füllt die Einträge von S
            for(int xi = 0; xi < grid_width; xi++){
                for(int yi = 0; yi < grid_height; yi++){
                    S[xi][yi] = S[xi][yi] + destarray[l].get_S_k(xi,yi)*w_S[l];
                }
            }
        }
    }
    void print_S(){
        cout << "----------------------------------------------------------------" << endl;
        for(int j = 0; j < grid_height; j++){
            for(int i = 0; i < grid_width; i++){
                if (S[i][j] > 9 && S[i][j] <= 99){cout << " " << S[i][j] << ";" ;}
                else if (S[i][j] > 99){cout << S[i][j] << ";" ;}
                else {cout << "  " << S[i][j] << ";" ;}

            }
        cout << endl;
        }
    }



private:

};



//person::person(){}
//hindernis::hindernis(){}
//ziel::ziel(){}
