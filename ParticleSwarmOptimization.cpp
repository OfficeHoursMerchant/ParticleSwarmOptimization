#include <iostream>
#include <cstdlib>
using namespace std;
#include <cmath>
#include <time.h>

double RNG(int magnitude);
double RNG(int low, int high);
double calculator(double x, double y);

int mag = 2; // Maximum magnitude of direction vectors 

int lowx = -15; // Lower bound of the x domain
int highx =15; // Upper bound of the x domain

int lowy = -3; // Lower bound of the y range
int highy = 3; // Upper bound of the y range

static double teamBestX;
static double teamBestY;
static double teamBestZ;

static double r = 0.8;
static double c = 1;


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
    // Default constructor. 

    Bird::Bird()
    : x(RNG(lowx, highx)), y(RNG(lowy, highy)), z(calculator(x,y)),  // 1. Defines x and y "randomly" based on parameters, then finds the Z value at that point in the function.
     directionVectorX(RNG(mag)), directionVectorY(RNG(mag)), // Defines the velocity vector randomly between [-mag, mag]
     personalBestX(x), personalBestY(y), personalBestZ(z)
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
        directionVectorX = (0.95)*(directionVectorX) + (c*r)*(personalBestX - x) + (c*r)*(teamBestX - x); 
        directionVectorY = (0.95)*(directionVectorY) + (c*r)*(personalBestY - y) + (c*r)*(teamBestY - y);
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
    static int swarmSize = 20; // Clerc (textbook) recommends 20-40. At least for 3D?
    Bird* swarm[swarmSize];

    // Occupies array with default constructor (psuedorandom) Bird object pointers. Aka: an array filled with random "birds" (particles).
    for (int i = 0; i<swarmSize; i++){ 
        swarm[i] = new Bird();
    }





// Sets the global best to the 1st (0th) bird. This is so it has a Z value to compare to for the following function.
teamBestX = swarm[0]->getX();
teamBestY = swarm[0]->getY();
teamBestZ = swarm[0]->getZ();

int turns = 1000;

globalBest_function(swarmSize, swarm); // Iterates through the entire swarm and finds the global best. Does this so it can have an actual global Best

swarm[0]->print();
int counter = 0;
//for (int i = 0; i<turns; i++){ // This is the stopping condition
for(int i = 0; i<300; i++){
    for (int j = 0; j<swarmSize; j++){ // Per turn, iterate through entire swarm
     swarm[j]->updateVelocity(); 
     swarm[j]->move(); 
     swarm[j]->personalBestUpdater();
    }
    counter += 1;
    globalBest_function(swarmSize, swarm); // updates global best (maybe i should rename) // At the end of every swarm turn, update the global. This is how it's supposed to be done,
    // But it also allows for parallelization--had the global best been updated as soon as it's found, then you could not do every swarm object in parallel per turn.

}

swarm[0]->print();
cout << endl << "Counter:" << counter;



return 0;} // Main end






// Functions (ooohh)


// Random (double) for the magnitude.
double RNG(int magnitude){ // Multiplying by 100 then dividing by 100 allows for values to be floats... I.e [-0.35, 0.28] rather than [-1, 0]
    double random = rand() % ((100*2*magnitude)+1); // will be between [0, 200*mag]
    random -= 100*magnitude; // [-100mag, 100mag]
    random /= 100;  // [-mag, mag], with two decimal points (-0.38, 2.37, etc.)
    return random; 
}


// Random point within the range.
double RNG(int low, int high){
    int length = abs(low) + abs(high); 
    double random = rand() % (length+1); // [0,length]
    random -= low; // [low, high]
    return random; 
}

// Gets Z for f(x,y) given x, y
double calculator(double x, double y){
   double z = pow((1.5 - x + x*y),2) + pow(2.25 - x + x* (pow(y,2)),2) + pow((2.625 - x + x * pow(y,3)),2); // Beale (3, 0.5, 0)
   //double z = pow((pow(x, 2) + y - 11),2) + pow((x+ pow(y,2) - 7),2); // Himmelblau (four, but 3,2,0)
   //double z= 100 * pow(abs(y - 0.01 * (pow(x,2))),0.5) + 0.01 * abs(x+10); // bulkin function n.6 -10, 1 0
  //)
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


// Ok, custom range and directional mag done. Looks like the local minima are trapping in Bukin No.6. But Beale and Himmelblau work fine.