#include <math.h>
#include <vector>
#include <iostream>
using namespace std;


//expects two vectors in format {px,py,vx,vy,m} and returns firsts objects acceleration (ax,ay) 
vector <float> accel_routine(vector <float> obj_i, vector <float> obj_j){
  float G_CONSTANT = 6.6743e-11;
  //calulating distance
  float distance = sqrt(pow((obj_i[0]-obj_j[0]), 2)+pow((obj_i[1]-obj_j[1]), 2));
  //calculating acceleration
  float obj_i_accel = G_CONSTANT*obj_j[4]/pow(distance,2);
  float obj_i_accel_x = ((obj_j[0]-obj_i[0])*obj_i_accel)/distance;
  float obj_i_accel_y = ((obj_j[1]-obj_i[1])*obj_i_accel)/distance;
  vector <float>  accelerations = {obj_i_accel_x,obj_i_accel_y};
  //We can encounter division by zero also we do not count actual sizes of planets and their mass distribution also satelites but this should not make a big deal
  


  //also object with 0 mass are not affected by force F = ma
  if(distance == 0 || obj_i[4] == 0){
  	vector <float>  accelerations = {0.0,0.0};
    return accelerations; 	
  }
  return accelerations;
}



//moves system in one time step 
//initializing dt 1/3 of a day as a default
void nextTimestep(vector <vector <float> >& system, float dT = 30.0e3){
    
		for(int i = 0; i < system.size(); i++){
			//cycle that actually changes system values using
			vector <float> sum_accel = {0.0,0.0};
      		for(int j = 0; j < system.size(); j++){
        		vector <float> accel_from_object = accel_routine(system[i],system[j]);
        		sum_accel[0] = sum_accel[0] + accel_from_object[0];
        		sum_accel[1] = sum_accel[1] + accel_from_object[1];
      		}
      		system[i][2]= system[i][2] + sum_accel[0]*dT;
      		system[i][3]= system[i][3] + sum_accel[1]*dT;      		
      		system[i][0]= system[i][0] + system[i][2]*dT;
      		system[i][1]= system[i][1] + system[i][3]*dT;

   		}

}

//handles collisions (combines two objects into one) assuming conservation of impulse
void handleCollisions(vector <vector <float> >& system, float collision_distance = 50.0 ){
	for(int i = 0; i < system.size(); i++){
		for(int j = 0; j < system.size(); j++){
			if (i == j || system[i][4] == 0 || system[j][4] == 0){
				continue;
			}
			float distance = sqrt(pow((system[i][0]-system[j][0]), 2)+pow((system[i][1]-system[j][1]), 2));
			if (distance < collision_distance){
				float result_x_velocity = (system[i][2]*system[i][4] + (system[j][2]*system[j][4]))/(system[i][4]+system[j][4]);
				float result_y_velocity = (system[i][3]*system[i][4] + (system[j][3]*system[j][4]))/(system[i][4]+system[j][4]);
				system[i][2] = result_x_velocity;
				system[i][3] = result_y_velocity;
				system[i][4] = system[i][4] + system[j][4];
				system[j][4] = 0;
				cout << "collision" << "\n";
			} 
			
   		}	
   		}
}	

//shows system values 
void printSystem(vector <vector <float> > system){
	for(int i = 0; i < system.size(); i++){
      for(int j = 0; j < system[i].size(); j++){
        cout << system[i][j] << " ";
      }
      cout << "\n";
    }
    cout << "\n";
}

//test
// as a result collisions worked fine one interesting fiture found about this specific test 
//(firstly 3 object that are on the one corner of the "square" collided because their distances are closer then to diagonal)
void testCollision_1(){
	vector <vector <float> > test_system = {
	{0.5,0.5,-0.6,-0.6,1.0},
	{0.5,-0.5,-0.6,0.6,1.0},
	{-0.5,0.5,0.6,-0.6,1.0},
	{-0.5,-0.5,0.6,0.6,1.0}
	};
    printSystem(test_system);


    for(int x = 0; x < 1000; x++){
        handleCollisions(test_system, 0.05);	
    	nextTimestep(test_system, 0.01);     	             
    }
    printSystem(test_system); 

}
void test_2(){
    vector <float> sun = {0.0,0.0,0.0,0.0,1.989e30}; 
    vector <float> earth = {0.0,1.496e11,-2.987e4,0.0,5.974e24}; 
    vector <float> mars = {0.0,2.279e11,-2.41e4,0.0,6.419e23}; 
    vector <float> mercury = {0.0,5.790e10,-4.79e4,0.0,3.302e23}; 
    vector <float> venus = {0.0,1.082e11,-3.50e4,0.0,4.869e24};
    //adding all objects to one vector that contains other vectors of planets and sun
    vector <vector <float> > solar_system = {sun,earth,mars,mercury,venus};
    printSystem(solar_system);

    for(int x = 0; x < 1000000; x++){
        handleCollisions(solar_system, 0.05);	
    	nextTimestep(solar_system, 30.0e3);     	             
    }
    printSystem(solar_system);

}





int main(){
	//testing collisions  6.3
	cout<< "Collision testing"<<"\n\n";
	testCollision_1();
	//testing previous system
	cout << "Solar system testing" << "\n\n";
	test_2();
    system("Pause");
    return 0;
}
