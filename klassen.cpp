const int grid_height = 100;
const int grid_width = 150;

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

// methods

//coordinates
    int x;
    int y;
//colour
    int r;
    int g;
    int b;

// Dynamic floor field D
    int D[grid_height][grid_width];


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

private:

};

//person::person(){}
//hindernis::hindernis(){}
//ziel::ziel(){}
