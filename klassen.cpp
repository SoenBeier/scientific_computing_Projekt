#include "initial_situation.cpp"
#include <stddef.h>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <math.h>

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
        //Setzt alle Eintr‰ge von S_k auf 0
        for(int g = 0; g < grid_width; g++){
            for(int h = 0; h< grid_height; h++){
                S_k[g][h] = 0;
            }
        }
        //Tr‰gt Anfangskoorinaten in den vector ein
        to_do.push_back(x);
        to_do.push_back(y);

        int counter = 0;

        while(true){
            // sichert die aktuelle Grˆﬂe des vectors to_do f¸r die n‰chste for-Schleife
            int ntodo = to_do.size();
            if (ntodo % 2 != 0){//Fehleranzeige, falls was schief l‰uft
                    cout << "Ein Fehler ist augetreten! die Anzahl ntodo ist nicht durch zwei teilbar." << endl;
                }
            for(int j = 0; j < (ntodo / 2); j++){

                //Schriebe das zugehˆrige Potential in das statische Feld:
                S_k[to_do[0]][to_do[1]] = counter;
                //Diese Koordinaten sind nun abgearbeitet und werden dem vector processed ¸bergeben
                processed.push_back(to_do[0]);
                processed.push_back(to_do[1]);
                //Suche Nachbarn f¸r das Feld S_k mit den Koordinaten (to_do[0],to_do[1}) und schreibe diese in den vector to_do
                int nh[8] = {//sind alle mˆglichen Nachbarn
                    to_do[0],to_do[1] + 1,
                    to_do[0] + 1,to_do[1],
                    to_do[0],to_do[1] - 1,
                    to_do[0] - 1,to_do[1]
                    };
                for (int l = 0; l < 4; l++){//f¸r jeden mˆglichen Nachbarn folgt:
                    if (could_a_person_go_to(nh[2*l],nh[2*l+1],obsarray) == false){// wenn eine Person hier nicht drauf darf
                        cout << "Nicht erlaubte Felder could: " << nh[2*l]<< ";" << nh[2*l+1] <<endl;
                        continue;
                    }
                    //Ist der aktuell ausgew‰hlte Nachbar schon im Vector processed oder to_do enthalten:
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

                //Lˆsche nun die bearbeiteten Koordinaten (to_do[0],to_do[1])
                to_do.erase(to_do.begin());
                to_do.erase(to_do.begin());
            }
            if(to_do.size() == 0){
                break;
            }
        counter++;
        }
    //Potentialfeld besitzt am Ausgang das grˆﬂte Potential und nimmt von da an ab; also Werte m¸ssen noch "umgekehrt" werden:
        //Finden des grˆﬂten Eintrags in S_k:
        int max_pot = 0;
        for(int i = 0; i < grid_width; i++){
            for(int j = 0; j < grid_height; j++){
                if(S_k[i][j] > max_pot){
                    max_pot = S_k[i][j];
                }
            }
        }
        //"umkehren" der Eintr‰ge: also Potential verl‰uft vom Eingang aus gesehen von groﬂ nach klein
        for(int i = 0; i < grid_width; i++){
            for(int j = 0; j < grid_height; j++){
                    if(could_a_person_go_to(i,j,obsarray)){
                        S_k[i][j] = max_pot - S_k[i][j];
                    }

            }
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
        renew_w_S(destarray);
        set_S(destarray);
        set_D();
    };
    person(int a, int b, int f1, int f2, int f3,destination destarray[quantity_destinations]){
        x = a;
        y = b;
        setrgb(f1,f2,f3);
        set_w_S();
        renew_w_S(destarray);
        set_S(destarray);
        set_D();
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
    void print_coords(){
        cout << "Ich bin bei x: " << x << "y: " << y << endl;
    }

// methods

//coordinates
    int x;
    int y;
//colour
    int r;
    int g;
    int b;

// ###### Dynamic floor field DZur Uni Potsdam(Brandenburg):
    double k_D = 1;
    int D[grid_width][grid_height];

    void set_D(){
        for (int i = 0; i < grid_width; i++){
            for(int j = 0; j < grid_height; j++){
                D[i][j] = 0;
            }
        }
    }
    void print_D(){
        for (int i = 0; i < grid_height; i++){
            for(int j = 0; j < grid_width; j++){
                cout << D[i][j] << ":";
            }
            cout << endl;
        }
    }


// ####### Static field S
    double k_S = 1;
    double w_S[quantity_destinations]; // wie sehr kennt die Person die verschiedenen Eing‰nge; Eintrag ist zwischen 0,1
    double S[grid_width][grid_height];

    void set_w_S(){//legt den anf‰nglicher Wissensstand der Person ¸ber die Ausg‰nge
        for(int i = 0; i < quantity_destinations ; i++){
            //Gibt dem Wert w_S f¸r das jeweilige Ziele einen zuf‰lligen Wert zwischen 0 und 1; w_S stellt hier das Wissen der Person ¸ber den ort des Ausgangs da
            w_S[i] = (double)(rand() % 10) / 10;
            cout <<"w_S ist:" <<w_S[i] << endl;
        }
    }
    void renew_w_S(destination destarray[quantity_destinations]){// erneuert die Eintr‰ge von w_S, wenn bestimmt Umst‰nde eintreten
        //wenn sich die Person sehr nahe an einem Ausgang befindet bekommt der Wert w_S, der f¸r das Wissen ¸ber diesen Ausgang steht, einen sehr hohen Wert, da die Person den Ausgang sieht o.ƒ.
        int r_influence_sphere = 5;// legt fest, ab wann die Person den Ausgang sehen kann
        for(int i = 0; i < quantity_destinations; i++){
            if(destarray[i].x > x - r_influence_sphere && destarray[i].x < x + r_influence_sphere){
                if(destarray[i].y > y - r_influence_sphere && destarray[i].y < y + r_influence_sphere){
                    w_S[i] = 2;
                }
            }
        }
    }
    void set_S(destination destarray[quantity_destinations]){//Addiere alle S_k Arrays der einzelnen destinations zum S Array hinzu; dies verl‰uft nach Gewichtung
        //setze alle Eintr‰ge von S auf 0:
        for(int xi = 0; xi < grid_width; xi++){
            for(int yi = 0; yi < grid_height; yi++){
                S[xi][yi] = 0;
            }
        }

        //F¸llt die Eintr‰ge von S
        for(int l = 0; l < quantity_destinations; l++){
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

// ###### Transmission matrix
    double T[3][3];

    void set_T(obstacle* obsarray){
    //f¸llt Eintr‰ge:
        //Alle Felder bekommen Wert 0; nicht benutzte Felder (nach Neumann Nachbarschaft) bleiben 0:
        for (int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                T[i][j] = 0;
            }
        }
        //cout << "hier m¸sste alles null sein:" << endl;
        //print_T();
        //Eintrag oben:
        //cout << "oben ?" << could_I_go_to(x,y - 1,obsarray) << endl;
        if(could_I_go_to(x,y - 1,obsarray)){
            T[1][0] = exp(k_S * S[x][y - 1]) + exp(k_D * D[x][y - 1]);
        }
        //Eintrag rechts:
        //cout << "rechts ?" << could_I_go_to(x + 1,y,obsarray) << endl;
        if(could_I_go_to(x + 2,y + 1,obsarray)){
            T[2][1] = exp(k_S * S[x + 1][y]) + exp(k_D * D[x + 1][y]);
        }
        //Eintrag unten:
        //cout << "unten ?" << could_I_go_to(x,y+1,obsarray) << endl;
        if(could_I_go_to(x + 1,y + 2,obsarray)){
            T[1][2] = exp(k_S * S[x][y + 1]) + exp(k_D * D[x][y + 1]);
        }
        //Eintrag links:
        //cout << "unten ?" << could_I_go_to(x,y+1,obsarray) << endl;
        if(could_I_go_to(x - 1,y,obsarray)){
            T[0][1] = exp(k_S * S[x - 1][y]) + exp(k_D * D[x - 1][y]);
        }
        //mitte:
        //cout << "hier bleiben ?" << could_I_go_to(x,y,obsarray) << endl;
        if(could_I_go_to(x,y,obsarray)){
            T[1][1] = exp(k_S * S[x][y]) + exp(k_D * D[x][y]);
        }
        //cout << "T befor normalization:" <<endl;
        //print_T();

    //Normalisierung der T-Matrix:
        //Finden der Summe der Eintr‰ge von T:
        double sum_T_entries = 0;
        for (int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                sum_T_entries = sum_T_entries + T[i][j];
            }
        }
        //Normalisierung durchf¸hren:
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                T[i][j] = T[i][j] / sum_T_entries;
            }
        }
        //cout << "T after normalization:" <<endl;
        print_T();
    }
    void print_T(){
        cout << "--------------------------------------" << endl;
        cout << "transition matrix T:" << endl;
        for (int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                cout << T[i][j] << " ; ";
            }
            cout << endl;
        }
        cout << "--------------------------------------" << endl;
    }
    double get_T(int qx, int qy){// gibt Eintrag der 3x3 Matrix aus
        if(qx > 2 || qy > 2 || qx < 0 || qy < 0){
                cout << "Fehler aufgetreten bei get_T!!" << endl;
        }
        return T[qx][qy];
    }

private:

};



//person::person(){}
//hindernis::hindernis(){}
//ziel::ziel(){}
