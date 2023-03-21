#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>
#include <vector>
#include <iostream>

using namespace std;

//expects two vectors in format {px,py,vx,vy,m} and returns firsts objects acceleration (ax,ay) 
vector <double> accel_routine(vector <double> obj_i, vector <double> obj_j){
  double G_CONSTANT = 6.6743e-11;
  //calulating distance
  double distance = sqrt(pow((obj_i[0]-obj_j[0]), 2)+pow((obj_i[1]-obj_j[1]), 2));
  //calculating acceleration
  double obj_i_accel = G_CONSTANT*obj_j[4]/pow(distance,2);
  double obj_i_accel_x = ((obj_j[0]-obj_i[0])*obj_i_accel)/distance;
  double obj_i_accel_y = ((obj_j[1]-obj_i[1])*obj_i_accel)/distance;
  vector <double>  accelerations = {obj_i_accel_x,obj_i_accel_y};
  //We can encounter division by zero also we do not count actual sizes of planets and their mass distribution also satelites but this should not make a big deal
  


  //also object with 0 mass are not affected by force F = ma
  if(distance == 0 || obj_i[4] == 0){
  	vector <double>  accelerations = {0.0,0.0};
    return accelerations; 	
  }
  return accelerations;
}



//moves system in one time step 
//initializing dt 1/3 of a day as a default
void nextTimestep(vector <vector <double> >& system, double dT = 30.0e3){
    
		for(unsigned int i = 0; i < system.size(); i++){
			//cycle that actually changes system values using
			vector <double> sum_accel = {0.0,0.0};
          //#pragma omp parallel for
      		for(unsigned int j = 0; j < system.size(); j++){
        		vector <double> accel_from_object = accel_routine(system[i],system[j]);
        		sum_accel[0] = sum_accel[0] + accel_from_object[0];
        		sum_accel[1] = sum_accel[1] + accel_from_object[1];
      		}
      		system[i][2]= system[i][2] + sum_accel[0]*dT;
      		system[i][3]= system[i][3] + sum_accel[1]*dT;      		
      		system[i][0]= system[i][0] + system[i][2]*dT;
      		system[i][1]= system[i][1] + system[i][3]*dT;

   		}

}

//handles collisions (combines two objects unsigned into one) assuming conservation of impulse
void handleCollisions(vector <vector <double> >& system, double collision_distance = 50.0 ){
	for(unsigned int i = 0; i < system.size(); i++){
		for(unsigned int j = 0; j < system.size(); j++){
			if (i == j || system[i][4] == 0 || system[j][4] == 0){
				continue;
			}
			double distance = sqrt(pow((system[i][0]-system[j][0]), 2)+pow((system[i][1]-system[j][1]), 2));
			if (distance < collision_distance){
				double result_x_velocity = (system[i][2]*system[i][4] + (system[j][2]*system[j][4]))/(system[i][4]+system[j][4]);
				double result_y_velocity = (system[i][3]*system[i][4] + (system[j][3]*system[j][4]))/(system[i][4]+system[j][4]);
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
void printSystem(vector <vector <double> > system){
	for(unsigned int i = 0; i < system.size(); i++){
      for(unsigned int j = 0; j < system[i].size(); j++){
        cout << system[i][j] << " ";
      }
      cout << "\n";
    }
    cout << "\n";
}

//test
// as a result collisions worked fine one unsigned interesting fiture found about this specific test 
//(firstly 3 object that are on the one corner of the "square" collided because their distances are closer then to diagonal)
void testCollision_1(){
	vector <vector <double> > test_system = {
	{0.5,0.5,-0.6,-0.6,1.0},
	{0.5,-0.5,-0.6,0.6,1.0},
	{-0.5,0.5,0.6,-0.6,1.0},
	{-0.5,-0.5,0.6,0.6,1.0}
	};
    printSystem(test_system);


    for(unsigned int x = 0; x < 1000; x++){
        handleCollisions(test_system, 0.05);	
    	nextTimestep(test_system, 0.01);     	             
    }
    printSystem(test_system); 

}
void test_2(unsigned int iter){
    vector <double> sun = {0.0,0.0,0.0,0.0,1.989e30}; 
    vector <double> earth = {0.0,1.496e11,-2.987e4,0.0,5.974e24}; 
    vector <double> mars = {0.0,2.279e11,-2.41e4,0.0,6.419e23}; 
    vector <double> mercury = {0.0,5.790e10,-4.79e4,0.0,3.302e23}; 
    vector <double> venus = {0.0,1.082e11,-3.50e4,0.0,4.869e24};
    //adding all objects to one vector that contains other vectors of planets and sun
    vector <vector <double> > solar_system = {sun,earth,mars,mercury,venus};
    //printSystem(solar_system);
    timeval st,en;
    

    gettimeofday(&st, NULL);
    //parallelization is used here but it causes race conditions 
    //to avoid this we can add parallelization to nextTimestep() but it takes too long to distribute such a small cycle
    //so we get only slowed down program
    #pragma omp parallel for num_threads(4)                
    for(unsigned int x = 0; x < iter; x++){
      handleCollisions(solar_system, 0.05);	
    	nextTimestep(solar_system, 30.0e3);     	             
    }
    gettimeofday(&en, NULL);
    double seconds = en.tv_sec - st.tv_sec;
    double useconds = en.tv_usec - st.tv_usec;
    cout << seconds*1000 + useconds/1000 << endl;
    //printSystem(solar_system);     
    
       


}
/*Compare 106 steps (we meet RAW n WAR error cause defferent threds change the same list variable)
no parallel
-86119.1 240410 -0.0742539 0.133314 1.989e+030
-8.87546e+010 1.20295e+011 -24120.6 -17655.2 5.974e+024
-7.51964e+010 2.1497e+011 -22757.2 -7982.84 6.419e+023
-2.60194e+010 -5.14686e+010 42399.3 -22719.9 3.302e+023
-9.25219e+010 5.50686e+010 -18022.2 -30203.8 4.869e+024
parallel
-81545.8 242479 -0.0716147 0.135364 1.989e+030
-8.88151e+010 1.20282e+011 -24158.4 -17665.9 5.974e+024
-7.52194e+010 2.14969e+011 -22769.2 -7984.28 6.419e+023
-3.05415e+010 -5.31978e+010 38837.5 -26050.3 3.302e+023
-9.27718e+010 5.50249e+010 -18162.6 -30228.8 4.869e+024
Time comparison
no parallel 3.023 3.027 4.022 4.03 4.031 4.032 4.032 4.032 4.032 4.032 4.032 4.033 4.033 4.042 5.041  
2 threads 2.014 2.015 2.015 2.016 2.016 2.016 2.016 2.019 2.019 2.019 2.027 3.02 3.023 3.025 3.025  73 percent faster
4 threads 1.002 3.146 2.429 2.004 2.002 1.004 4.013 2.003 1.002 2.302 2.007 1.007 1.002 2.006 1.002 120 percent faster

the result are kinda weird cause there is some time to distribute steps through threds 
we get the result 100 percent and 200 percent if we get higher number of iterations in our cycle
but in this case we meet severe RAW WAR error so it is not reasonable and we should not use parallelization this way here

*/
int main(){
	//testing collisions  6.3
	//cout<< "Collision testing"<<"\n\n";
	//testCollision_1();
	//testing previous system
	cout << "Solar system testing" << "\n\n";
  for(unsigned int iter = 105; iter < 120; iter++){
	test_2(iter);
  }
  cin.get();
  return 0;
}
