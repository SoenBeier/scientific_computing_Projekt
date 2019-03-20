#include "initial_situation.cpp"
#include <stddef.h>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <time.h>

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
        set_static_field_k(obstvec);
    };
    destination(int a, int b, int f1, int f2, int f3,vector<obstacle> &obstvec, int q_obst, int q_dest, int q_pers){
        x = a;
        y = b;
        setrgb(f1,f2,f3);
        quantity_obstacles = q_obst;
        quantity_destinations = q_dest;
        quantity_persons = q_pers;
        set_static_field_k(obstvec);
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

    // in all other cases:
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
        setrgb(0,0,200);

        quantity_obstacles = q_obst;
        quantity_destinations = q_dest;
        quantity_persons = q_pers;

        //###Zu set_w_S
        int p_d[1]; //bevorzugtes Ziel
        p_d[0] = rand() % quantity_destinations; // bevorzugtes Ziel wird zufällig ausgewählt
        set_w_S(true,1,p_d, rand() % (quantity_destinations) + 1); //die Person kennt also mindestens eines der Ziele sehr gut .. der Rest wird zufällig entschieden
        renew_w_S(destvec);

        set_S(destvec);

        set_D_on_zero();

        evacuated = false;
        number_of_conflicts = 0;

    };
    person(int nx, int ny, int f1, int f2, int f3,vector<destination> &destvec, int q_obst, int q_dest, int q_pers){
        x = nx;
        y = ny;
        setrgb(f1,f2,f3);

        quantity_obstacles = q_obst;
        quantity_destinations = q_dest;
        quantity_persons = q_pers;

        //###Zu set_w_S
        int p_d[1]; //bevorzugtes Ziel
        p_d[0] = rand() % quantity_destinations; // bevorzugtes Ziel wird zufällig ausgewählt
        set_w_S(true,1,p_d, rand() % (quantity_destinations) + 1); //die Person kennt also mindestens eines der Ziele sehr gut .. der Rest wird zufällig entschieden
        renew_w_S(destvec);

        set_S(destvec);

        set_D_on_zero();

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

    void moveto(int xn, int yn, vector<person> &persvec, vector <int > &propability_arr_diff, vector<int> &propability_arr_dec)
    {
        if(evacuated == false)
        {
            set_D(persvec, xn, yn, propability_arr_diff, propability_arr_dec);
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
//colour
    int r;
    int g;
    int b;



// ###### Dynamic floor field DZur Uni Potsdam(Brandenburg):
    double k_D = 1;
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

    void set_D(vector<person> &persvec, int xn, int yn, vector<int> &propability_arr_diff, vector<int> &propability_arr_dec)
    {
        for (int k=0; k< quantity_persons; k++)
        {
            if (persvec[k].x != x && persvec[k].y != y)
            {
                persvec[k].D[x][y]++;
            }
        }

        //nach jeder bewegung wird an jedem ort geprüft ob sich das dfeld verteilt oder zerfällt
        for (int i=0; i< persvec.size(); i++)
        {
            for (int x=0; x< grid_width; x++)
            {
                for (int y=0; y< grid_height; y++)
                {
                    if (persvec[i].D[x][y]!=0)
                    {
                        //diffusion_dyn_f(propability_arr_diff, persvec, xn, yn, i);
                        //decay_dyn_f(propability_arr_dec, persvec, i);
                    }
                }
            }
        }
    }



    void decay_dyn_f(vector <int> &propability_arr_dec, vector <person> &persvec, int i)
    {
        //zerfall des d Feldes:
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
                        int r_2=rand()%100;
                        if (propability_arr_dec[r_2]==1) //verifikation: D feld soll sich auflösen
                        {
                            if (/*falls 1 Nachbar frei ist*/ persvec[i].D[x][y+1]==0 || persvec[i].D[x][y-1]==0 || persvec[i].D[x+1][y]==0 || persvec[i].D[x-1][y]==0 || /*falls 2 Nachbarn frei sind*/ persvec[i].D[x+1][y]==persvec[i].D[x][y+1]==0 || persvec[i].D[x-1][y]==persvec[i].D[x][y+1]==0 || persvec[i].D[x-1][y]==persvec[i].D[x][y-1]==0 || persvec[i].D[x+1][y]==persvec[i].D[x][y-1]==0 || /*falls 3 Nachbarn frei sind*/ persvec[i].D[x-1][y]==persvec[i].D[x][y+1]==persvec[i].D[x+1][y]==0 || persvec[i].D[x][y-1]==persvec[i].D[x-1][y]==persvec[i].D[x][y+1]==0 || persvec[i].D[x-1][y]==persvec[i].D[x][y-1]==persvec[i].D[x+1][y]==0 || persvec[i].D[x][y-1]==persvec[i].D[x+1][y]==persvec[i].D[x][y+1]==0)
                            {
                                persvec[i].D[x][y]=0;
                            }
                        }
    }

    void diffusion_dyn_f(vector <int > &propability_arr_diff, vector <person> &persvec,int ax, int ay, int xn, int yn, int i)
    {
        //Verteilung des D-Felds
                        //vector <int > propability_arr_diff(10); steht in der main damit in shell ausgegeben werden kann
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

                        int r_1=rand () %100;
                        if (propability_arr_diff[r_1]==1) //verifikation: D feld soll sich verteilen
                        {
                            int which_cell=rand ()%4; //zu welcher zelle propagiert das d-feld?

                            if (which_cell==0)
                            {
                                if (ax-1!=xn && ay!=yn)
                                {
                                    persvec[i].D[ax-1][ay]++;
                                }
                                else if (ax+1!=xn && ay!=yn)
                                {
                                    persvec[i].D[ax+1][ay]++;
                                }
                                else if (ax!=xn && ay-1!=yn)
                                {
                                    persvec[i].D[ax][ay-1]++;
                                }
                                else if (ax!=xn && ay+1!=yn)
                                {
                                    persvec[i].D[ax][ay+1]++;
                                }
                            }
                            else if (which_cell==1)
                            {
                                if (ax+1!=xn && ay!=yn)
                                {
                                    persvec[i].D[ax+1][ay]++;
                                }
                                else if (ax!=xn && ay-1!=yn)
                                {
                                    persvec[i].D[ax][ay-1]++;
                                }
                                else if (ax!=xn && ay+1!=yn)
                                {
                                    persvec[i].D[ax][ay+1]++;
                                }
                                else if (ax-1!=xn && ay!=yn)
                                {
                                    persvec[i].D[ax-1][ay]++;
                                }
                            }
                            else if (which_cell==2)
                            {
                                if (ax!=xn && ay-1!=yn)
                                {
                                    persvec[i].D[ax][ay-1]++;
                                }
                                else if (ax!=xn && ay+1!=yn)
                                {
                                    persvec[i].D[ax][ay+1]++;
                                }
                                else if (ax-1!=xn && ay!=yn)
                                {
                                    persvec[i].D[ax-1][ay]++;
                                }
                                else if (ax+1!=xn && ay!=yn)
                                {
                                    persvec[i].D[ax+1][ay]++;
                                }
                            }
                            else if (which_cell==3)
                            {
                                if (ax!=xn && ay+1!=yn)
                                {
                                    persvec[i].D[ax][ay+1]++;
                                }
                                else if (ax-1!=xn && ay!=yn)
                                {
                                    persvec[i].D[ax-1][ay]++;
                                }
                                else if (ax+1!=xn && ay!=yn)
                                {
                                    persvec[i].D[ax+1][ay]++;
                                }
                                else if (ax!=xn && ay-1!=yn)
                                {
                                    persvec[i].D[ax][ay-1]++;
                                }
                            }
                        }
    }


    void print_D()
    {
        for (int i = 0; i < grid_width; i++){
            for(int j = 0; j < grid_height; j++){
                cout << D[i][j] << ":";
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
    void set_w_S(int quantity_known_dest, bool previously_set = false){//legt den anfänglicher Wissensstand der Person über die Ausgänge fest
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
                //Füllen von w_S zum ausgewählen Ziel mit einer Zufälligen Zahl von (0 bis 1]
                w_S[r] = (double) (rand() % 10) / 10 + 0.1;
                cout <<"w_S[r] ist:" << w_S[r] <<" mit r =" << r << endl;
                selected_dest.push_back(r);
            }
            else{
                i--;//könnte eine Endlosscheife verursachen -> nicht so schön ;) aber geht
            }
        }

        /*
        for(int i = 0; i < quantity_known_dest; i++){
            //Gibt dem Wert w_S für das jeweilige Ziele einen zufälligen Wert zwischen 0 und 1; w_S stellt hier das Wissen der Person über den ort des Ausgangs da
            w_S[i] = (double)(rand() % 10) / 10;
            cout <<"w_S ist:" <<w_S[i] << endl;
        }*/
    }
    void set_w_S(bool prefer_a_dest, int quantity_preferred_dest, int *preferred_dest, int quantity_known_dest){//legt den anfänglichen Wissensstand der Person über die Ausgänge fest; preferierte Ziele werden bevorzugt nach der Festlegungangesteuert, qpd ist die Anzahl der übergebenen Ziele
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
                w_S[preferred_dest[i]] = 1;
            }
            set_w_S(quantity_known_dest-quantity_preferred_dest, true);


        }
        else{
            set_w_S(quantity_known_dest);
        }




    }
    void renew_w_S(vector<destination> &destvec){// erneuert die Einträge von w_S, wenn bestimmt Umstände eintreten
        //wenn sich die Person sehr nahe an einem Ausgang befindet bekommt der Wert w_S, der für das Wissen über diesen Ausgang steht, einen sehr hohen Wert, da die Person den Ausgang sieht o.Ä.
        int r_influence_sphere = 5;// legt fest, ab wann die Person den Ausgang sehen kann
        for(int i = 0; i < quantity_destinations; i++){
            if(destvec[i].x > x - r_influence_sphere && destvec[i].x < x + r_influence_sphere){
                if(destvec[i].y > y - r_influence_sphere && destvec[i].y < y + r_influence_sphere){
                    w_S[i] = 5;
                }
            }
        }
    }
    void print_w_S(){
        cout << endl << "print_w_S:" << endl;
        for(int i = 0; i < quantity_destinations; i++){
            cout << w_S[i] << ";";
        }
        cout << endl;
    }
    void set_S(vector<destination> &destvec){//Addiere alle S_k Arrays der einzelnen destinations zum S Array hinzu; dies verläuft nach Gewichtung
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
        /*
        //cout <<"max_S : " <<max_S << endl;
        // Beschränkt die Größe der Einträge, damit die größe der Zahlen nicht die Speichergrenze beim exponentieren überschreiten
        while (max_S > 200){
            for(int l = 0; l < quantity_destinations; l++){
                for(int xi = 0; xi < grid_width; xi++){
                    for(int yi = 0; yi < grid_height; yi++){
                        S[xi][yi] = S[xi][yi]/2;


                    }
                }

            }
            max_S = max_S/2;
            //cout <<"max_S2 : " <<max_S << endl;
        }*/


    }
    void print_S(int width = grid_width, int height = grid_height){
        cout << "----------------------------------------------------------------" << endl;
        for(int j = 0; j < height; j++){
            for(int i = 0; i < width; i++){
                if (S[i][j] > 9 && S[i][j] <= 99){cout << " " << S[i][j] << ";" ;}
                else if (S[i][j] > 99){cout << S[i][j] << ";" ;}
                else {cout << "  " << S[i][j] << ";" ;}

            }
        cout << endl;
        }
    }

// ###### Transmission matrix
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
        if((could_I_go_to(x,y - 1,obstvec,persvec) && movement_mode == 's' ) || ((could_I_go_to(x,y - 1,obstvec,persvec) || is_there_a_person_on(x,y - 1, persvec)) && movement_mode == 'p' )){ // entweder sequentieller Ablauf: dann could I go to; bei paralellen ist es egal ob auf dem Feld gerade eine andere Person steht
            T[1][0] = expl(k_S * S[x][y - 1]) + exp((k_D /*+ number_of_conflicts*/) * D[x][y - 1]);
        }
        //Eintrag rechts:
        //cout << "rechts ?" << could_I_go_to(x + 1,y,obstvec) << endl;
        if((could_I_go_to(x + 1,y,obstvec,persvec) && movement_mode == 's' ) || ((could_I_go_to(x + 1,y,obstvec,persvec) || is_there_a_person_on(x + 1,y, persvec)) && movement_mode == 'p' )){
            T[2][1] = expl(k_S * S[x + 1][y]) + exp((k_D /*+ number_of_conflicts*/) * D[x + 1][y]);
        }
        //Eintrag unten:
        //cout << "unten ?" << could_I_go_to(x,y+1,obstvec) << endl;
        if((could_I_go_to(x,y + 1,obstvec,persvec) && movement_mode == 's' ) || ((could_I_go_to(x,y + 1,obstvec,persvec) || is_there_a_person_on(x,y + 1, persvec)) && movement_mode == 'p' )){
            T[1][2] = expl(k_S * S[x][y + 1]) + exp((k_D/* + number_of_conflicts*/) * D[x][y + 1]);
        }
        //Eintrag links:
        //cout << "unten ?" << could_I_go_to(x,y+1,obstvec) << endl;
        if((could_I_go_to(x - 1,y,obstvec,persvec)&& movement_mode == 's' ) || ((could_I_go_to(x - 1,y,obstvec,persvec) || is_there_a_person_on(x - 1,y, persvec)) && movement_mode == 'p' )){
            T[0][1] = expl(k_S * S[x - 1][y]) + exp((k_D/* + number_of_conflicts*/) * D[x - 1][y]);
        }
        //mitte:
        //cout << "hier bleiben ?" << could_I_go_to(x,y,obstvec) << endl;
            T[1][1] = expl(k_S * S[x][y]) + exp((k_D/* + number_of_conflicts*/) * D[x][y]);


    //Überprüfung ob die Einträge des Feldes zu groß sind und deswegen T fehlerhaft erstellt wird:
        for (int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if (isinf(T[i][j])){
                    cout << "Fehler beim Erstellen der Matrix T aufgetreten ! Die Größe der Einträge übersteigt die maximale Größe des long double Zahlentyps." << endl << "Versuchen sie die Simulation mit einem kleineren Feld zu wiederholen." << endl;
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
    bool already_moved;
    vector<int> conflict_partner;
    int number_of_conflicts;

// ###### time measurement for the analysis of movement of the person
    double time_start;
    double time_end;
    double evacuation_time = 0;
    bool evacuated;

    void start_time_measurement(){
        time_start = clock()/CLOCKS_PER_SEC;
    }
    void end_time_measurement(){
        time_end = clock()/CLOCKS_PER_SEC;
        evacuation_time = time_end - time_start;
    }

private:

};



//person::person(){}
//hindernis::hindernis(){}
//ziel::ziel(){}
