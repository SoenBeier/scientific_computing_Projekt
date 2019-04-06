#include "initial_situation.cpp"
#include <stddef.h>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>

using namespace std;



class obstacle
{
public:
// constructors
    obstacle(){};
    obstacle(int nx, int ny, int q_obst, int q_dest, int q_pers){
        x = nx;
        y = ny;

        setrgb(200,200,200);

        quantity_obstacles = q_obst;
        quantity_destinations = q_dest;
        quantity_persons = q_pers;
    };
    obstacle(int nx, int ny, int f1, int f2, int f3, int q_obst, int q_dest, int q_pers){
        x = nx;
        y = ny;

        setrgb(f1,f2,f3);

        quantity_obstacles = q_obst;
        quantity_destinations = q_dest;
        quantity_persons = q_pers;
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

//quantities
int quantity_obstacles;
int quantity_destinations;
int quantity_persons;

//coordinates
    int x;
    int y;
//colour
    int r;
    int g;
    int b;

private:

};

//##################################################################################################################################
//##################################################################################################################################

class destination
{
public:
// constructors
    destination(){};
    destination(int a, int b,vector<obstacle> &obstvec, int q_obst, int q_dest, int q_pers){
        x = a;
        y = b;
        setrgb(0,200,0);
        quantity_obstacles = q_obst;
        quantity_destinations = q_dest;
        quantity_persons = q_pers;

        if(corridor_conditions == false){
            set_static_field_k(obstvec); //wird für den Fall des Korridors nicht benötigt
        }
    };
    destination(int a, int b, int f1, int f2, int f3,vector<obstacle> &obstvec, int q_obst, int q_dest, int q_pers){
        x = a;
        y = b;
        setrgb(f1,f2,f3);
        quantity_obstacles = q_obst;
        quantity_destinations = q_dest;
        quantity_persons = q_pers;

        if(corridor_conditions == false){
            set_static_field_k(obstvec); //wird für den Fall des Korridors nicht benötigt
        }
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
    bool is_it_here(int qx, int qy){//  has the object the coordinates (qx,qy) ?
        if (x == qx && y == qy){
            return true;
        }
        else {
            return false;
        }
    }
// methods

//quantities
int quantity_obstacles;
int quantity_destinations;
int quantity_persons;
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


    bool could_a_person_go_to (int qx, int qy, vector<obstacle> &obstvec){//delivers true, if the person could stay at (x,y)/ could move to this cell
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
            //cout << "x: " << obstvec[i].x << ", y: " << obstvec[i].y << endl;
            if((obstvec[i].x == qx) && (obstvec[i].y == qy)){
                return_value = false;
            }
        }
        return return_value;
    }
    vector<int> to_do;
    vector<int> processed;
    void set_static_field_k(vector<obstacle> &obstvec){
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
//cout << S_k[8][8] << endl;
//if(counter == 3){print_S_k();}
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
                    if (could_a_person_go_to(nh[2*l],nh[2*l+1],obstvec) == false){// wenn eine Person hier nicht drauf darf
                        //cout << "Nicht erlaubte Felder could: " << nh[2*l]<< ";" << nh[2*l+1] <<endl;
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
    //Potentialfeld besitzt am Ausgang das größte Potential und nimmt von da an ab; also Werte müssen noch "umgekehrt" werden:
        //Finden des größten Eintrags in S_k:
        int max_pot = 0;
        for(int i = 0; i < grid_width; i++){
            for(int j = 0; j < grid_height; j++){
                if(S_k[i][j] > max_pot){
                    max_pot = S_k[i][j];
                }
            }
        }
        //"umkehren" der Einträge: also Potential verläuft vom Eingang aus gesehen von groß nach klein
        for(int i = 0; i < grid_width; i++){
            for(int j = 0; j < grid_height; j++){
                    if(could_a_person_go_to(i,j,obstvec)){
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

//fuer das vereinigen der Ziele:
    vector <int> dest_neighbours;

private:

};

//##################################################################################################################################
//##################################################################################################################################

class person
{
public:
// constructors
    person(){};
    person(int nx, int ny,vector<destination> &destvec, int q_obst, int q_dest, int q_pers){
        x = nx;
        y = ny;
        ax = x;
        ay = y;
        aax = ax;
        aay = ay;
        int colour_variation = (rand() % 150) - 75; //leichte Farbvariation, damit die einzelnen Personen voneinander unterschieden werden können
        setrgb(0,0,150 + colour_variation);
        quantity_obstacles = q_obst;
        quantity_destinations = q_dest;
        quantity_persons = q_pers;
        //###Zu set_w_S
        int p_d[1]; //bevorzugtes Ziel

        p_d[0] = rand() % quantity_destinations; // bevorzugtes Ziel wird zufällig ausgewählt
        set_w_S(true,1,p_d, rand() % (quantity_destinations) + 1); //die Person kennt also mindestens eines der Ziele sehr gut .. der Rest wird zufällig entschieden


        if(corridor_conditions == false){
            set_S_normal(destvec);
        }

        set_D_on_zero();
        //Zufälliges setzen des "Friction" Parameters:
        friction = (rand() % 300) / 1000;
        evacuated = false;
        number_of_conflicts = 0;

    };
    person(int nx, int ny, int f1, int f2, int f3,vector<destination> &destvec, int q_obst, int q_dest, int q_pers){
        x = nx;
        y = ny;
        ax = x;
        ay = y;
        aax = ax;
        aay = ay;

        setrgb(f1,f2,f3);

        quantity_obstacles = q_obst;
        quantity_destinations = q_dest;
        quantity_persons = q_pers;

        //###Zu set_w_S
        int p_d[1]; //bevorzugtes Ziel
        p_d[0] = rand() % quantity_destinations; // bevorzugtes Ziel wird zufällig ausgewählt
        set_w_S(true,1,p_d, rand() % (quantity_destinations) + 1); //die Person kennt also mindestens eines der Ziele sehr gut .. der Rest wird zufällig entschieden

        if(corridor_conditions == false){
            set_S_normal(destvec);
        }


        set_D_on_zero();

        //Zufälliges setzen des "Friction" Parameters:
        friction = (rand() % 300) / 1000;

        evacuated = false;
        number_of_conflicts = 0;
    };
// constructors

// methods
    void setrgb(int f1, int f2, int f3){
        r = f1;
        g = f2;
        b = f3;
    };

    double friction = 0.0; // Wahrscheinlichkeit, dass sich die Person nicht bewegt, obwohl sie sich bewegen sollte

    void moveto(int xn, int yn, bool after_conflict=false){
        double r = (rand() % 1000) / 1000.0; // Zufallszahl
        if((evacuated == false && r >= friction) || (evacuated == false && after_conflict == false)){//Wenn die vor der Bewegung kein Konflikt stattgefunden hat wird die Bewegung auf jeden Fall ausgeführt; mit Konflikt nur zu einer bestimmten Wahrscheinlichkeit, die von der "friction" abhängt

            //set_D(persvec, xn, yn, propability_arr_diff, propability_arr_dec, obstvec);   -> jetzt in update_objekt_parameter
            aax = ax;
            aay = ay;
            ax = x;
            ay = y;
            x = xn;
            y = yn;
        }
    }

    bool is_it_here(int qx, int qy){
        if (x == qx && y == qy){
            return true;
        }
        else {
            return false;
        }
    }
    bool could_I_go_to (int qx, int qy, vector<obstacle> &obstvec, vector<person> &persvec){//delivers true, if the person could stay at (x,y)/ could move to this cell
    //###cells out of borders arent available for a person:
        if ((qy >= grid_height) || (qx >= grid_width)){
            return false;
        }
        if ((qy < 0) || (qx < 0)){
            return false;
        }
    //###cells filled by a person arent available for a person:
        if(is_there_a_person_on(qx,qy,persvec)){
            return false;
        }
    //###cells filled by an obstacle arent available for a person:
        bool return_value = true;
        for(int i = 0; i < quantity_obstacles; i++){
            //cout << "x: " << obstvec[i].x << ", y: " << obstvec[i].y << endl;
            if((obstvec[i].x == qx) && (obstvec[i].y == qy)){
                return_value = false;
            }
        }

    // in all other cases:
        return return_value;
    }
    bool is_there_a_person_on(int qx, int qy,vector<person> &persvec){
        for(int i = 0; i < quantity_persons; i++){
            if(qx == persvec[i].x && qy == persvec[i].y){
                return true;
            }
        }
        return false;
    }
    void print_coords(){
        cout <<endl << "Ich bin bei x: " << x << "y: " << y << endl;
    }
// methods

//quantities
int quantity_destinations;
int quantity_obstacles;
int quantity_persons;
//coordinates
    int x;
    int y;
//alte Koordinaten
    int ax;
    int ay;
//alte Koordinate der alten Koordinaten
    int aax;
    int aay;
//letzte Bewegungsrichtung
    char last_movement_direction;
//vorletzte Bewegungsrichtung
    char a_last_movement_direction;

    char set_last_movement_direction(int ax, int ay, int jx, int jy){ //ax steht für "altes x", jx fuer "jetziges x", setzt die Variable last_movement_direction
        if(jx > ax){
            //Fehlerueberpruefung: ist die Bewegung genau 1 lang?:
            if (jx != ax + 1 && evacuated == false && corridor_conditions ==false){
                cout << "Fehler: Die Person hat sich nicht exakt um 1 bewegt,1" << endl;
            }
            if(jy != jy && evacuated == false&& corridor_conditions ==false){
                cout << "Fehler: Bewegung der Person wurde falsch ausgeführt; x und y Koordinaten wurden gleichzeitig verändert" << endl;
            }

            return 'r';
        }
        else if(jx < ax){
            //Fehlerueberpruefung: ist die Bewegung genau 1 lang?:
            if (jx != ax - 1 && evacuated == false&& corridor_conditions ==false){
                cout << "Fehler: Die Person hat sich nicht exakt um 1 bewegt,2" << endl;
            }
            if(jy != jy && evacuated == false&& corridor_conditions ==false){
                cout << "Fehler: Bewegung der Person wurde falsch ausgeführt; x und y Koordinaten wurden gleichzeitig verändert" << endl;
            }

            return 'l';
        }
        else if(jy > ay){
            //Fehlerueberpruefung: ist die Bewegung genau 1 lang?:
            if (jy != ay + 1 && evacuated == false&& corridor_conditions ==false){
                cout << "Fehler: Die Person hat sich nicht exakt um 1 bewegt,3" << endl;
            }
            if(jx != jx && evacuated == false&& corridor_conditions ==false){
                cout << "Fehler: Bewegung der Person wurde falsch ausgeführt; x und y Koordinaten wurden gleichzeitig verändert" << endl;
            }

            return 'u';
        }
        else if(jy < ay){
            //Fehlerueberpruefung: ist die Bewegung genau 1 lang?:
            if (jy != ay - 1 && evacuated == false&& corridor_conditions ==false){
                cout << "Fehler: Die Person hat sich nicht exakt um 1 bewegt,4" << endl;
            }
            if(jx != jx && evacuated == false&& corridor_conditions ==false){
                cout << "Fehler: Bewegung der Person wurde falsch ausgeführt; x und y Koordinaten wurden gleichzeitig verändert" << endl;
            }

            return 'o';
        }
        else{//Person ist stehen geblieben
            return 's';
        }
    }

//colour
    int r;
    int g;
    int b;



// ###### Dynamic floor field DZur Uni Potsdam(Brandenburg):
    double k_D = 1;
    int panic_par;
    int iterat_val;
    int D[grid_width][grid_height];

    void set_D_on_zero()
    {
        //setzt D feld auf null
        for (int i = 0; i < grid_width; i++)
        {
            for(int j = 0; j < grid_height; j++){
                D[i][j] = 0;
            }
        }
    }

    void set_D_3 (vector <person> &persvec, int j)
    {
        if (k_D!=0)
        {


        for (int i=0; i< persvec.size(); i++) //gehe alle personen durch
        {
            if (i!=j) ///nicht von der eigenen spur beeinflussen
            {
                if((reject_other_D_fields == true && followed_the_pers_my_S(persvec[i],2) == true) || reject_other_D_fields == false) ///bei mehreren zielen kann eine spur nur entstehen wenn die person in die gleich richtung laeuft
                {
                    if ((persvec[i].ax!=persvec[i].x || persvec[i].ay!=persvec[i].y) && persvec[i].evacuated == false) /// überprüfe ob sich die person i bewegt hat
                    {
                        persvec[j].D[persvec[i].ax][persvec[i].ay]++;
                    }
                }
                if (followed_the_pers_my_S(persvec[i], 2)==false && reject_other_D_fields ==true) ///bei mehreren zielen kann, wenn eine pers von mir weg läuft, diese person mein d feld in die richtung der person kauputtmachen (--)
                {
                    if ((persvec[i].ax!=persvec[i].x || persvec[i].ay!=persvec[i].y) && persvec[i].evacuated == false) /// überprüfe ob sich die person i bewegt hat
                    {
                        persvec[j].D[persvec[i].ax][persvec[i].ay]--;
                    }
                }
                /*
                else
                {
                    cout << "Fehler in set_D" << endl;
                }*/
            }
        }
        }
    }

    void decay_dyn_f(vector <int> &propability_arr_dec, vector <person> &persvec, int i)
    {
        if(decay_param == 0){return;}
        bool grid_full=true;
        //geht die gesamte fläche durch
        for (int x=0; x< grid_width; x++)
        {
            for (int y=0; y<grid_height; y++)
            {
                int d_abs_beginn = abs(persvec[i].D[x][y]);
                for (int j=0; j<d_abs_beginn; j++) ///je staerker das d feld am puntk x y ist, desto stärker kann es zerfallen
                {
                    //zerfall des d Feldes:
                    //-----------------------------
                    //schreibe wahrscheinlichkeitsarray
                    for (int k=0; k < propability_arr_dec.size(); k++)
                    {
                        propability_arr_dec[k]=0;
                    }
                    if (decay_param!=0)
                    {
                        for (int k=0; k<decay_param ; k++)
                        {
                            propability_arr_dec[k]=1;
                        }
                    }
                    /*if (i==1)
                    {
                        cout << "Person " << i << " hat die Koordinaten: (x;y) = " << "(" << persvec[i].x << ";" << persvec[i].y << ")" << endl;
                        cout << "D-Feld von Person "<< i << " ungleich null am Punkt: (x;y) = " <<"("<<x<<";"<<y<<")" << endl;
                        cout << "############################" << endl;
                    }*/



                //generiere zufällige zahl und überprüfe ob d feld zerfallen soll
                    int r_2=rand()%100;
                    if (propability_arr_dec[r_2]==1) //verifikation: D feld soll sich auflösen
                    {
                    //if (/*falls 1 Nachbar frei ist*/ persvec[i].D[x][y+1]==0 || persvec[i].D[x][y-1]==0 || persvec[i].D[x+1][y]==0 || persvec[i].D[x-1][y]==0 || /*falls 2 Nachbarn frei sind*/ persvec[i].D[x+1][y]==persvec[i].D[x][y+1]==0 || persvec[i].D[x-1][y]==persvec[i].D[x][y+1]==0 || persvec[i].D[x-1][y]==persvec[i].D[x][y-1]==0 || persvec[i].D[x+1][y]==persvec[i].D[x][y-1]==0 || /*falls 3 Nachbarn frei sind*/ persvec[i].D[x-1][y]==persvec[i].D[x][y+1]==persvec[i].D[x+1][y]==0 || persvec[i].D[x][y-1]==persvec[i].D[x-1][y]==persvec[i].D[x][y+1]==0 || persvec[i].D[x-1][y]==persvec[i].D[x][y-1]==persvec[i].D[x+1][y]==0 || persvec[i].D[x][y-1]==persvec[i].D[x+1][y]==persvec[i].D[x][y+1]==0)
                    //{
                        /*if (i==1)
                        {
                            cout << "-------------------------------------------" << endl;
                            cout << "Zerfall findet bei Person "<< i<< " am Punkt: (x;y) = " <<"("<<x<<";"<<y<<")  statt."<< endl;
                            cout << "--------------------------------------------" << endl;
                        }*/
                        if(persvec[i].D[x][y] > 0){
                            persvec[i].D[x][y]--;
                        }
                        else{
                            persvec[i].D[x][y]++;
                        }
                            //grid_full=false;
                        //}
                        /*
                        //falls das d feld komplett voll ist, zerfällt es trz iwo am rand
                        if (grid_full==true)
                        {
                            int x_rand = rand () %grid_width;
                            int y_rand = rand () %grid_height;
                            int which_side = rand() %4;
                            switch (which_side)
                            {
                                case 0:
                                    persvec[i].D[0][y_rand]=0;
                                case 1:
                                    persvec[i].D[x_rand][0]=0;
                                case 2:
                                    persvec[i].D[grid_width-1][y_rand]=0;
                                case 3:
                                    persvec[i].D[x_rand][grid_height-1]=0;
                            }
                        }
                        */
                    }
                }

            }
        }
}

    void diffusion_dyn_f(vector <int > &propability_arr_diff, vector <person> &persvec, int xn, int yn, int i, vector<obstacle> &obstvec, vector <int> &propability_arr_dec)
    {
        if(diffusion_param == 0){return;}
        bool grid_full=true;
        //gehe ganzen grundriss durch und überprüfe ob sich das d feld verteilen soll
        for (int x=0; x< grid_width; x++)
        {
            for (int y=0; y<grid_height; y++)
            {
                    for (int k=0; k<abs(persvec[i].D[x][y]); k++) ///je stärker das d feld desto schneller kann es sich verteilen
                    {
                    //Verteilung des D-Felds
                    //------------
                    //schreibe wahrscheinlichkeitsarray
                    for (int k=0; k <propability_arr_diff.size(); k++)
                    {
                        propability_arr_diff[k]=0;
                    }
                    if (diffusion_param!=0)
                    {
                        for (int k=0; k<diffusion_param ; k++)
                        {
                            propability_arr_diff[k]=1;
                        }
                    }

                    //generiere zufallszahl und überprüfe ob sich das d feld verteilen soll
                    int r_1=rand () %100;
                    if (propability_arr_diff[r_1]==1) //verifikation: D feld soll sich verteilen
                    {
                        int which_cell=rand ()%4; //zu welcher zelle propagiert das d-feld?

                        if (which_cell==0)
                        {
                            if (x-1!=xn && y!=yn)
                            {
                                if (can_d_field_be_here(x-1, y, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x-1][y]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x-1][y]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                            else if (x+1!=xn && y!=yn)
                            {
                                if (can_d_field_be_here(x+1, y, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x+1][y]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x+1][y]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                            else if (x!=xn && y-1!=yn)
                            {
                                if (can_d_field_be_here(x, y-1, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x][y-1]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x][y-1]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                            else if (x!=xn && y+1!=yn)
                            {
                                if (can_d_field_be_here(x, y+1, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x][y+1]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x][y+1]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                        }
                        else if (which_cell==1)
                        {
                            if (x+1!=xn && y!=yn)
                            {
                                if (can_d_field_be_here(x+1, y, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x+1][y]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x+1][y]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                            else if (x!=xn && y-1!=yn)
                            {
                                if (can_d_field_be_here(x, y-1, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x][y-1]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x][y-1]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                            else if (x!=xn && y+1!=yn)
                            {
                               if (can_d_field_be_here(x, y+1, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x][y+1]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x][y+1]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                            else if (x-1!=xn && y!=yn)
                            {
                                if (can_d_field_be_here(x-1, y, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x-1][y]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x-1][y]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                        }
                        else if (which_cell==2)
                        {
                            if (x!=xn && y-1!=yn)
                            {
                                if (can_d_field_be_here(x, y-1, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x][y-1]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x][y-1]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                            else if (x!=xn && y+1!=yn)
                            {
                               if (can_d_field_be_here(x, y+1, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x][y+1]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x][y+1]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                            else if (x-1!=xn && y!=yn)
                            {
                                if (can_d_field_be_here(x-1, y, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x-1][y]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x-1][y]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                            else if (x+1!=xn && y!=yn)
                            {
                                if (can_d_field_be_here(x+1, y, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x+1][y]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x+1][y]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                        }
                        else if (which_cell==3)
                        {
                            if (x!=xn && y+1!=yn)
                            {
                               if (can_d_field_be_here(x, y+1, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x][y+1]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x][y+1]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                            else if (x-1!=xn && y!=yn)
                            {
                                if (can_d_field_be_here(x-1, y, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x-1][y]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x-1][y]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                            else if (x+1!=xn && y!=yn)
                            {
                                if (can_d_field_be_here(x+1, y, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x+1][y]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x+1][y]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                            else if (x!=xn && y-1!=yn)
                            {
                                if (can_d_field_be_here(x, y-1, obstvec)==true)
                                {
                                    if (persvec[i].D[x][y]>0)
                                    {
                                        persvec[i].D[x][y-1]++;
                                        //persvec[i].D[x][y]--; ///d feld verwischt
                                    }
                                    else
                                    {
                                        persvec[i].D[x][y-1]--;
                                        //persvec[i].D[x][y]++; ///d feld verwischt
                                    }
                                }
                                grid_full=false;
                            }
                        }
                    }
                }
            }
        }

        if (grid_full==true && diffusion_param !=0) //sollte nicht passieren, vermeidet fehler
        {
            decay_dyn_f(propability_arr_dec, persvec, i);
        }
    }

    bool can_d_field_be_here (int qx, int qy, vector<obstacle> &obstvec)
    {
        //delivers true, if the d field could stay at (x,y)/ could move to this cell

        //###cells out of borders arent available for d feld:
        if ((qy >= grid_height) || (qx >= grid_width))
        {
            return false;
        }
        if ((qy < 0) || (qx < 0))
        {
            return false;
        }

        //###cells filled by an obstacle arent available for d feld:
        for(int i = 0; i < quantity_obstacles; i++)
        {
            if((obstvec[i].x == qx) && (obstvec[i].y == qy))
            {
                return false;
            }
        }
        return true;
    }



    void set_panic_par(vector <person> &persvec, int i, int iteration)
    {
        if(panik_aktiviert == false){return;} // wenn der Panik Parameter nicht beachtet werden soll, wird diese Funktion abgebrochen
        if (iteration==0)
        {
            persvec[i].iterat_val=0;
        }

        ///rising panic parameter
        if (persvec[i].had_a_conflict==true)
        {
            persvec[i].panic_par++;
            persvec[i].iterat_val=persvec[i].iteration;
            /*r=0;
            g=255;
            b=0;*/
        }

        ///zerfall vom panikparameter
        if (persvec[i].iterat_val+1==persvec[i].iteration && persvec[i].had_a_conflict==false && persvec[i].evacuated==false)
        {
            persvec[i].panic_par--;
            /*r=0;
            g=0;
            b=255;*/
        }

        ///panikschwelle -> diffusion
        if (persvec[i].panic_par>=panik_schwelle)
        {
            for (int k=0; k<persvec.size(); k++)
            {
                if (persvec[k].x==persvec[i].x-1 && persvec[k].y==persvec[i].y)
                {
                    persvec[k].panic_par++;
                }
                if (persvec[k].x==persvec[i].x+1 && persvec[k].y==persvec[i].y)
                {
                    persvec[k].panic_par++;
                }
                if (persvec[k].x==persvec[i].x && persvec[k].y==persvec[i].y-1)
                {
                    persvec[k].panic_par++;
                }
                if (persvec[k].x==persvec[i].x-1 && persvec[k].y==persvec[i].y+1)
                {
                    persvec[k].panic_par++;
                }
                /*r=255;
                g=0;
                b=0;*/
            }
        }
    }

    void print_D()
    {
        cout << "----------------------------------------------------------------" << endl;
        for(int j = 0; j < grid_height; j++){
            for(int i = 0; i < grid_width; i++){
                if (D[i][j] > 9999){cout << "" << (int)D[i][j] << ";" ;}
                else if (D[i][j] > 999){cout << " " << (int)D[i][j] << ";" ;}
                else if (D[i][j] > 99){cout << "  " << (int)D[i][j] << ";" ;}
                else if (D[i][j] > 9){cout << "   " << (int)D[i][j] << ";" ;}
                else {cout << "    " << D[i][j] << ";" ;}

            }
        cout << endl;
        }
    }


// ####### Static field S
    double k_S = 1;
    vector <double> w_S;// wie sehr kennt die Person die verschiedenen Eingänge; Eintrag ist zwischen 0,1
    double S[grid_width][grid_height];

    void set_w_S(double w){// der Wissenstand der Personen wird für alle Ausgänge gleich groß gewählt (so groß wie w)
        w_S.resize(quantity_destinations);
        for(int i = 0; i < quantity_destinations; i++){
            w_S[i] = w;
        }
    }
    void set_w_S(int quantity_known_dest, bool previously_set = false){//legt den anfänglicher Wissensstand der Person über die Ausgänge fest, Parameter w_S wird für eine Anzahl(quantity_known_dest) von Zielen zufällg gewählt, wenn previously set == false ist (Ausnahmefall ist, wenn quantity_known_dest == 1 ist, dann ist w_S = 1; previously set = true wird nur intern benutzt
        w_S.resize(quantity_destinations);
        for(int i = 0; i < quantity_destinations; i++){
            if(previously_set == false)
            w_S[i] = 0;
        }
        //Fehlervermeidung:
        if(quantity_known_dest > quantity_destinations){
            cout << "Fehler in set_w_S(int) Anzahl der bekannten Ziele ist größer, als die Anzahl der Ziele!!" << endl;
            return;
        }
        //Füllt vector selected_dest mit Nummern, die den Zielen zugeordnet werden, die schon einen Wert w_S erhalten haben:
        vector <int> selected_dest;
        for(int i = 0; i < quantity_known_dest; i++){
            int r = (rand() % quantity_destinations);
            //jede Nummer darf nur ein mal vorkommen:
            bool used = false;
            for(int j = 0; j < selected_dest.size(); j++){
                if(selected_dest[j] == r){
                    used = true;
                }
            }
            if(used == false && (previously_set == false || (previously_set == true && w_S[r] == 0))){
                //Füllen von w_S zum ausgewählen Ziel mit einer Zufälligen Zahl von (0 bis 1], außer, wenn nur ein Ausgang bekannt ist (in diesem Fall ist w_S immer 1)
                if(quantity_known_dest == 1){
                    w_S[r] = 1.;
                }
                else{
                w_S[r] = (double) (rand() % 10) / 10 + 0.1;
                }
                //cout <<"w_S[r] ist:" << w_S[r] <<" mit r =" << r << endl;
                selected_dest.push_back(r);
            }
            else{
                i--;//könnte eine Endlosscheife verursachen -> nicht so schön ;) aber geht
            }
        }
    }
    void set_w_S(bool prefer_a_dest, int quantity_preferred_dest, int *preferred_dest, int quantity_known_dest){//legt den anfänglichen Wissensstand der Person über die Ausgänge fest; preferierte Ziele werden bevorzugt nach der Festlegung angesteuert, qpd ist die Anzahl der übergebenen Ziele
        w_S.resize(quantity_destinations);
        // set all values of w_S = 0:
        for(int i = 0; i < quantity_destinations; i++){
            w_S[i] = 0;
        }
        if (prefer_a_dest == true){
            if (quantity_known_dest < quantity_preferred_dest){//Fehlervermeidung
                cout << "Fehler in set_w_S; die Anzahl der Ziele, die nicht 0 sein sollen ist zu klein!" << endl;
                return;
            }
            for(int i = 0; i < quantity_preferred_dest; i++){
                w_S[preferred_dest[i]] = 2;
            }
            set_w_S(quantity_known_dest-quantity_preferred_dest, true);


        }
        else{
            set_w_S(quantity_known_dest);
        }




    }
    void renew_w_S_and_S(vector<destination> &destvec, bool foreign_call){// erneuert die Einträge von w_S, wenn bestimmt Umstände eintreten
        //wenn sich die Person sehr nahe an einem Ausgang befindet bekommt der Wert w_S, der für das Wissen über diesen Ausgang steht, einen sehr hohen Wert, da die Person den Ausgang sieht o.Ä.
        if(foreign_call == false){//Bedingung, da sonst die Analyse verfälscht wird
            int r_influence_sphere = 5;// legt fest, ab wann die Person den Ausgang sehen kann
            for(int i = 0; i < quantity_destinations; i++){
                if(destvec[i].x > x - r_influence_sphere && destvec[i].x < x + r_influence_sphere){
                    if(destvec[i].y > y - r_influence_sphere && destvec[i].y < y + r_influence_sphere){
                        w_S[i] = 2;
                    }
                }
            }
        }
        if(take_closest_exit == true){//Wenn es einen näheren Ausang gibt wird w_S so verändert, dass dieser angesteuert wird
            // Suchen des Ziels, zu welchem die Person am wenigsten Schritte machen muss
            int max_S_k = 0;
            int numb_new_dest;
            for (int i = 0; i < destvec.size(); i++){
                if (destvec[i].S_k[x][y] > max_S_k){
                    max_S_k = destvec[i].S_k[x][y];
                    numb_new_dest = i;
                    //cout << min_S_k << endl;
                }
            }
            //Veränderung von w_S, alle weiter entfernten Ziele erhalten ein w_S von 0
            for (int i = 0; i < w_S.size(); i++){
                if(i != numb_new_dest){
                    w_S[i] = 0;
                }
                else{
                    w_S[i] = 1;
                    g = (int)(i * 250 / destvec.size());//Ändert die Farbe der Personen, damit klar ersichtlich ist, welche PErson welches Ziel ansteuert
                }
            }
        }
        if((foreign_call == false || take_closest_exit == true) && corridor_conditions == false){
            set_S_normal(destvec);
        }

    }
    void print_w_S(){
        cout << endl << "print_w_S:" << endl;
        for(int i = 0; i < quantity_destinations; i++){
            cout << w_S[i] << ";";
        }
        cout << endl;
    }
    void set_S_normal(vector<destination> &destvec){//Addiere alle S_k Arrays der einzelnen destinations zum S Array hinzu; dies verläuft nach Gewichtung
        double max_S = 0;
        //setze alle Einträge von S auf 0:
        for(int xi = 0; xi < grid_width; xi++){
            for(int yi = 0; yi < grid_height; yi++){
                S[xi][yi] = 0;
            }
        }

        //Füllt die Einträge von S
        for(int l = 0; l < quantity_destinations; l++){
            for(int xi = 0; xi < grid_width; xi++){
                for(int yi = 0; yi < grid_height; yi++){
                    S[xi][yi] = S[xi][yi] + destvec[l].get_S_k(xi,yi) * w_S[l];
                    if(S[xi][yi] > max_S){
                        max_S = S[xi][yi];
                    }
                }
            }
        }
    }
    void set_S_corridor( vector<person> &persvec, vector<destination> &destvec, vector<obstacle> &obstvec){
        //setze alle Einträge von S auf 0:
        for(int xi = 0; xi < grid_width; xi++){
            for(int yi = 0; yi < grid_height; yi++){
                S[xi][yi] = 0;
            }
        }
        //Wenn diese Funktion ausgeführt wird ein waagerechter Korridor simuliert, der mehrere Ziele am Anfang und am Ende des Korridors besitzt
        //Geht die Ziele durch und merkt sich die größte x Koordinate
        int max_x = 0;
        for(int i = 0; i < destvec.size(); i++){
            if(destvec[i].x > max_x){
                max_x = destvec[i].x;
            }
        }

        if(destvec[numb_selected_dest].x == max_x){//Dies ist das rechte Ziel, also ist die Formel S[x][y] = x
            // Aendert Farbe der Personen für eine bessere Übersicht
            b = 0;
            g = 50;
            r = 255;
            for(int i = 0; i < max_x; i++){
                for(int j = 0; j < grid_height; j++){
                    if(could_I_go_to(i,j,obstvec,persvec) || is_there_a_person_on(i,j,persvec)){//Abfrage ob eine Person auf das Feld gehen könnte
                        S[i][j] = i;
                    }
                }
            }
        }
        else{//Es bleibt nur das linke Ziel übrig, also ist die Formel S[x][y] = x_max - x
            b = 255;
            g = 0;
            for(int i = 0; i < max_x; i++){
                for(int j = 0; j < grid_height; j++){
                    if(could_I_go_to(i,j,obstvec,persvec) || is_there_a_person_on(i,j,persvec)){//Abfrage ob eine Person auf das Feld gehen könnte
                        S[i][j] = max_x - i;
                    }
                }
            }
        }

    }
    void print_S(int width = grid_width, int height = grid_height){
        cout << "----------------------------------------------------------------" << endl;
        for(int j = 0; j < height; j++){
            for(int i = 0; i < width; i++){
                if (S[i][j] > 9999){cout << "" << (int)S[i][j] << ";" ;}
                else if (S[i][j] > 999){cout << " " << (int)S[i][j] << ";" ;}
                else if (S[i][j] > 99){cout << "  " << (int)S[i][j] << ";" ;}
                else if (S[i][j] > 9){cout << "   " << (int)S[i][j] << ";" ;}
                else {cout << "    " << S[i][j] << ";" ;}

            }
        cout << endl;
        }
    }

// ###### Transition matrix
    long double T[3][3];

    void set_T(vector<obstacle> &obstvec,vector<person> &persvec, char movement_mode = 's'){
    //in der Funktion wird zwischen dem Erstellen des T Arrays für eine parallele Update-Regel und einer sequentiellen Update-Regel unterschieden
    //Bei der paralellen Update-Regel wird nicht abgefragt ob sich eine Person auf dem angefragen Fehld befindet
    //füllt Einträge:
        //Alle Felder bekommen Wert 0; nicht benutzte Felder (nach Neumann Nachbarschaft) bleiben 0:
        for (int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                T[i][j] = 0;
            }
        }
        //cout << "hier müsste alles null sein:" << endl;
        //print_T();
        //Eintrag oben:
        //cout << "oben ?" << could_I_go_to(x,y - 1,obstvec) << endl;
        if(could_I_go_to(x,y - 1,obstvec,persvec)){ // entweder sequentieller Ablauf: dann could I go to; bei paralellen ist es egal ob auf dem Feld gerade eine andere Person steht
            T[1][0] = expl(k_S * S[x][y - 1] + (k_D+panic_par) * D[x][y - 1]);
        }
        //Eintrag rechts:
        //cout << "rechts ?" << could_I_go_to(x + 1,y,obstvec) << endl;
        if(could_I_go_to(x + 1,y,obstvec,persvec)){
            T[2][1] = expl(k_S * S[x + 1][y] + (k_D+panic_par) * D[x + 1][y]);
        }
        //Eintrag unten:
        //cout << "unten ?" << could_I_go_to(x,y+1,obstvec) << endl;
        if(could_I_go_to(x,y + 1,obstvec,persvec)){
            T[1][2] = expl(k_S * S[x][y + 1] + (k_D+panic_par) * D[x][y + 1]);
        }
        //Eintrag links:
        //cout << "unten ?" << could_I_go_to(x,y+1,obstvec) << endl;
        if(could_I_go_to(x - 1,y,obstvec,persvec)){
            T[0][1] = expl(k_S * S[x - 1][y]+ (k_D+panic_par) * D[x - 1][y]);
        }
        //mitte:
        //cout << "hier bleiben ?" << could_I_go_to(x,y,obstvec) << endl;
            T[1][1] = expl(k_S * S[x][y] + (k_D+panic_par) * D[x][y]);


    //Überprüfung ob die Einträge des Feldes zu groß sind und deswegen T fehlerhaft erstellt wird:
        for (int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if (isinf(T[i][j])){
                    cout << "Fehler beim Erstellen der Matrix T aufgetreten ! Die Groesse der Eintraege uebersteigt die maximale Groesse des long double Zahlentyps." << endl << "Versuchen sie die Simulation mit einem kleineren Feld zu wiederholen." << endl;
                }
            }
        }

        //cout << "T befor normalization:" <<endl;
        //print_T();
    //Normalisierung der T-Matrix:
        //Finden der Summe der Einträge von T:
        long double sum_T_entries = 0;
        for (int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                sum_T_entries = sum_T_entries + T[i][j];
            }
        }
        //Normalisierung durchführen:
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                T[i][j] = T[i][j] / sum_T_entries;
            }
        }
        //cout << "T after normalization:" <<endl;
        //print_T();
    }
    void print_T(){
        cout << "--------------------------------------" << endl;
        cout << "Koordinaten:(";
        print_coords();
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

// ##### desired coordinates; are used, when update rule is parallel
    int desired_x;
    int desired_y;
    char desired_direction;
    int number_of_conflicts;
    bool wins_conflict;
    bool had_a_conflict; //wird benutzt um herauszufinden ob bei einer Bewegung der "friction" Parameter angewendet werden muss

// ###### time measurement for the analysis of movement of the person
    double time_start;
    double time_end;
    double evacuation_time = 0;
    int iteration;
    int iteration_when_evacuated;
    bool evacuated;

    void start_time_measurement(){
        time_start = clock()/CLOCKS_PER_SEC;
    }
    void end_time_measurement(){
        time_end = clock()/CLOCKS_PER_SEC;
        evacuation_time = time_end - time_start;
    }

// ###### wenn "reject_other_D_fields" aktiviert ist, werden folgende Variablen benutzt:
    int numb_selected_dest; // gibt das Ziel an welches die Person kennt und ansteuern wird
    //last_movement_direction wird benutzt und steht bei den Koordinaten





private:
    bool followed_the_pers_my_S(person qpers, int iterations_done){//ist wahr, wenn die gefragte Person mit ihrem letzten(iterations_done == 1) bzw. vorletzten(iterations_done == 2) Schritt dem eigenen Potentialfeld gefolgt ist und in die Richtung des eigenen Ziels geht.
        int nx, ny, ax, ay;
        if(iterations_done == 1){
            nx = qpers.x;
            ny = qpers.y;
            ax = qpers.ax;
            ay = qpers.ay;
        }
        else if(iterations_done == 2){
            nx = qpers.ax;
            ny = qpers.ay;
            ax = qpers.aax;
            ay = qpers.aay;
        }
        else{
            cout << "Fehler in followed_the_pers_my_S()" << endl;
        }
        if((k_S * S[nx][ny]) > (k_S * S[ax][ay]) && qpers.evacuated == false && iteration > 1)
        {
            if ((iterations_done==2 && followed_the_pers_my_S(qpers, 1)==true) || iterations_done==1)
            {
                return true;
            }
        }
        return false;
    }
};


class conflict{
public:
    conflict(){

    }
    conflict(int nx, int ny, vector<int> &cp, vector<person> &persvec){
        x = nx;
        y = ny;
        conflict_partner = cp;
        rise_number_of_conflicts_of_persons(persvec);
        set_C(persvec);
        number_of_winner = who_winns_conflict();
    }

    //Ort des Konfliktes:
    int x;
    int y;

    //Nummern der Personen im Konflikt:
    vector<int> conflict_partner;

    //Gewinner des Konfliktes:
    int number_of_winner;

    //Konfliktmatrix
    double C[3][3];

    //Hilfsmatrix, in der die Nummern der Personen stehen, die am Konflikt teilnehmen (mit relativer Position zum Ort des Konflikts
    double C_pers_numb[3][3];



    void print_C(){
        cout << "----------------C---------------" << endl;
        cout << "An der Stelle: " << x << ";" << y << endl;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                cout << C[j][i] << " , ";
            }
            cout << endl;
        }
        cout << "----------------C---------------" << endl;
    }
    void print_C_pers_numb(){
        cout << "----------------Cpers---------------" << endl;
        cout << "An der Stelle: " << x << ";" << y << endl;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                cout << C_pers_numb[j][i] << " , ";
            }
            cout << endl;
        }
        cout << "----------------Cpers---------------" << endl;
    }
    double get_C(int x, int y){
        return C[x][y];
    }
    double get_C_pers_numb(int x, int y){
        return C_pers_numb[x][y];
    }

private:
    void set_C(vector<person> &persvec){

        //Setzt Einträge der Matrizen auf 0
        for(int k = 0; k < 3; k++){
            for(int l = 0; l < 3; l++){
                C[k][l] = 0;
                C_pers_numb[k][l] = 0;
            }
        }

        //Füllt Einträge von C, die Werte von der Transitionmatrix, die dazu geführt haben, dass sich die Person auf das Feld Conflict.x,Conflict.y bewegen will wird in die Konfliktmatrix C eingetragen
        for(int i = 0; i < conflict_partner.size(); i++){
            C[persvec[conflict_partner[i]].x - x + 1][persvec[conflict_partner[i]].y - y + 1] = persvec[conflict_partner[i]].get_T(x - persvec[conflict_partner[i]].x + 1, y - persvec[conflict_partner[i]].y + 1);
            C_pers_numb[persvec[conflict_partner[i]].x - x + 1][persvec[conflict_partner[i]].y - y + 1] =  conflict_partner[i];
        }
        //Normalisierung der C-Matrix:
         //Finden der Summe der Einträge von C:
        double sum_C_entries = 0;
        for (int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                sum_C_entries = sum_C_entries + C[i][j];
            }
        }
         //Normalisierung durchführen:
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                C[i][j] = C[i][j] / sum_C_entries;
            }
        }
    }
    int who_winns_conflict(){
        double r = rand() % 1000 / 1000.;
        if(r < get_C(1,0)){
            return get_C_pers_numb(1,0);
        }
        else if(r < (get_C(1,0) + get_C(2,1))){
            return get_C_pers_numb(2,1);
        }
        else if(r < (get_C(1,0) + get_C(2,1) + get_C(1,2))){
            return get_C_pers_numb(1,2);
        }
        else if(r < (get_C(1,0) + get_C(2,1) + get_C(1,2) + get_C(0,1))){
            return get_C_pers_numb(0,1);
        }
        else{
            return get_C_pers_numb(1,1);
        }
    }
    void rise_number_of_conflicts_of_persons(vector<person> &persvec){
        for(int i = 0; i < conflict_partner.size(); i++){
            persvec[conflict_partner[i]].number_of_conflicts ++;
            persvec[conflict_partner[i]].had_a_conflict = true; // für die richtige Anwendung des friction Parameters
        }
    }
};

//person::person(){}
//hindernis::hindernis(){}
//ziel::ziel(){}
