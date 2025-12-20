#include <iostream>
#include <cstdlib>
using namespace std;
#include <cmath>
#include <time.h>


double RNG();
double calculator(double x, double y);


static double teamBestX;
static double teamBestY;
static double teamBestZ;

static double r = 0.8;
static double c = 2;


// Makes class for the swarm/bird/particle objects
  class Bird {
        double x;
        double y;
        double z;

        double personalBestX;
        double personalBestY;
        double personalBestZ;
        

        double directionVectorX;
        double directionVectorY;

    public:
    // Accessors!
        const double getX() {return x;}
        const double getY() {return y;}
        const double getZ() {return z;}

        const double getPersonalBestX() {return personalBestX;}
        const double getPersonalBestY() {return personalBestY;}
        const double getPersonalBestZ() {return personalBestZ;}

        const double getDirectionVectorX() {return directionVectorX;}
        const double getDirectionVectorY() {return directionVectorY;}
    

    // Mutators!
        void changeX(double newX) {x = newX;}
        void changeY(double newY) {y = newY;}
        void changeZ(double newZ) {z = newZ;}

        void changePBX(double newPBX) {personalBestX = newPBX;}
        void changePBY(double newPBY) {personalBestY = newPBY;}
        void changePBZ(double newPBZ) {personalBestZ = newPBZ;}

        void changeDVX(double newDVX) {directionVectorX = newDVX;}
        void changeDVY(double newDVY) {directionVectorY = newDVY;}
     


    // Constructors
        Bird();
        Bird(int x, int y);
      

    // Member functions
        void print();
        void personalBestUpdater();
        void move();
        void updateVelocity();
    };
   
// Defining the class
    // Constructors. Just so it lets me do custom constructor. 33.23 So if I get that value I know something weird is going on.
    Bird::Bird()
    : x(33.23), y(33.23), directionVectorX(33.23), directionVectorY(33.22)
    {}
    // Custom. This is the one I use
    Bird::Bird(int x, int y)
    : x(x), y(y), z(calculator(x, y)), // Initialzes x, y, and z coordinate based on inputs
    personalBestX(x), personalBestY(y), personalBestZ(calculator(x,y)), // Initializes the personal best to its current coordinate.
    directionVectorX(RNG()), directionVectorY(RNG())  // Initializes the direction vector to a random value
    {}


//Member functions 

    //Print function
    void Bird::print(){
        cout << "Coordinates: " << x << ", " << y << ", " << z << endl;
        cout << "Personal Best: " << personalBestX << ", " << personalBestY << ", " << personalBestZ << endl;
        cout << "Team Best: " << teamBestX << ", " << teamBestY << ", " << teamBestZ << endl;
        cout << "Directional vector: " << directionVectorX << ", " << directionVectorY << endl;
        }

    // Updates the velocity. The weighted equation comes from the PSO definition.
    void Bird::updateVelocity(){
        directionVectorX = (0.9)*(directionVectorX) + (c*r)*(personalBestX - x) + (c*r)*(teamBestX - x); 
        directionVectorY = (0.9)*(directionVectorY) + (c*r)*(personalBestY - y) + (c*r)*(teamBestY - y);
    }

    void Bird::personalBestUpdater(){ // Checks if the current Bird's value is lower than its personal best. If so, update ALL the personal best vals.
        if (z < personalBestZ){
            personalBestX = x;
            personalBestY = y;
            personalBestZ = z;
        }
    }

    void Bird::move(){ // Add the current pos + the vector
        x = x+directionVectorX;
        y = y+directionVectorY;
        z = calculator(x, y); // Update the Z based on this new position
    }


void globalBest_function(int s_size,  Bird *array[]); // Prototype.


int main(){
    srand(time(NULL));

    // Initialize the swarm parameters
    int swarmLength = 10;
    int swarmHeight = 10;
    static int swarmSize = (swarmLength*swarmHeight);
    Bird* swarm[swarmSize];


    int arrayCounter = -1; // so it can start at 0

    // Fills 1D array with Bird objects (each one initialize at x, y), so it makes a grid. (Should be RNG)
    for (int y = 0; y < swarmHeight; y++){
        for (int x= 0; x < swarmLength; x++){
            arrayCounter+=1;
        swarm[arrayCounter] = new Bird(x, y); 
     }
    }

// Sets the global best to the 1st bird. This is so it has a Z value to compare to for the following function.
teamBestX = swarm[0]->getX();
teamBestY = swarm[0]->getY();
teamBestZ = swarm[0]->getZ();

int turns = 1000;

globalBest_function(swarmSize, swarm); // Iterates through the entire swarm and finds the global best. Does this so it can have an actual global Best



for (int i = 0; i<turns; i++){ // This is the stopping condition
    for (int j = 0; j<swarmSize; j++){ // Per turn, iterate through entire swarm
     swarm[j]->updateVelocity(); 
     swarm[j]->move(); 
     swarm[j]->personalBestUpdater();
    }
    globalBest_function(swarmSize, swarm); // updates global best (maybe i should rename) // At the end of every swarm turn, update the global. This is how it's supposed to be done,
    // But it also allows for parallelization (Had the global best been updated as soon as it's found, then you could not do every swarm object in parallel per turn.)

}



return 0;} // Main end






// functions ooo


// Returns a random val between -1 and 1
double RNG(){
    double random = rand() % 201; // [0,200]
    random -= 100; // [-100, 100]
    random /= 100; // [-1, 1] 
    return random; 
}

// Gets Z for f(x,y) given x, y
double calculator(double x, double y){
   // double z = pow((1.5 - x + x*y),2) + pow(2.25 - x + x* (pow(y,2)),2) + pow((2.625 - x + x * pow(y,3)),2); // Beale (3, 0.5, 0)
  // double z = pow((pow(x, 2) + y - 11),2) + pow((x+ pow(y,2) - 7),2); // Himmelblau (four, but 3,2,0)
   //double z= 100 * pow(abs(y - 0.01 * (pow(x,2))),0.5) + 0.01 * abs(x+10); // bulkin function n.6 -10, 1 0
  double z = pow((x + 2*y - 7),2) + pow((2*x + y -5),2);
   return z;}


// Updates teamBest if the z is lower
void globalBest_function(int s_size, Bird *array[]){
   for (int i = 0; i<s_size; i++){
        if (array[i]->getZ() < teamBestZ){
            teamBestX = array[i]->getX();
            teamBestY = array[i]->getY();
            teamBestZ = array[i]->getZ();
        }
   }
}

