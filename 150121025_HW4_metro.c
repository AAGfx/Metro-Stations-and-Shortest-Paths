/*

This program regulates metro systems,lines and stations also determines best way to arrival

Ahmet Abdullah GULTEKIN 
150121025

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Define a constant SIZE with value 10
#define SIZE 10

double disX, disY, disOfNeigh, predistOfNeigh, shortDisOfNeigh;
double disXtoFinish, disYtoFinish, distoFinish, predistoFinish, shortDistoFinish;

//Structures of stations,lines and systems
typedef struct MetroStation {
	
	char name[20];
	double x, y;
	
} MetroStation;

typedef struct MetroLine {
	
	char color[20];
	MetroStation MetroStations[SIZE];
	
} MetroLine;

typedef struct MetroSystem {
	
	char name[20];
	MetroLine MetroLines[SIZE];
	
} MetroSystem;

//Declare a MetroSystem with the name of istanbul and an empty content.
MetroSystem istanbul = {"istanbul", '\0'};


//Function definitions
int equals(MetroStation s1, MetroStation s2);
void addStation(MetroLine* l1, MetroStation s1);
int hasStation(MetroLine l1, MetroStation s1);
MetroStation getFirstStop(MetroLine l1);
MetroStation getPreviousStop(MetroLine l1, MetroStation s1);
MetroStation getNextStop(MetroLine l1, MetroStation s1);
void addLine(MetroSystem* s1, MetroLine l1);
void printLine(MetroLine l1);
void printPath(MetroStation MetroStations[]);
double getDistanceTravelled(MetroStation MetroStations[]);
MetroStation findNearestStation(MetroSystem s1, double x, double y);
void getNeighboringStations(MetroSystem sys1, MetroStation st1, MetroStation MetroStations[]);
void findPath(MetroStation s1, MetroStation s2, MetroStation path[]);
void recursiveFindPath(MetroStation s1, MetroStation s2, MetroStation partialPath[], MetroStation bestPath[]);

//Main funtion
int main()
{
	int i;
	double myX=1, myY=2; 
	double goalX=62, goalY=45; 
	
	// define 3 metro lines, 9 metro stations, and an empty myPath
	MetroLine red={'\0'}, blue={'\0'}, green={'\0'};
	MetroStation s1, s2, s3, s4, s5, s6, s7, s8, s9;
	MetroStation myPath[SIZE]={'\0'};
	
	strcpy(red.color, "red"); 
	strcpy(blue.color, "blue");
	strcpy(green.color, "green");

	
	strcpy(s1.name, "Haydarpasa"); 		s1.x=0; 	s1.y=0;
	strcpy(s2.name, "Sogutlucesme"); 	s2.x=10; 	s2.y=5;
	strcpy(s3.name, "Goztepe"); 		s3.x=20; 	s3.y=10;
	strcpy(s4.name, "Kozyatagi"); 		s4.x=30; 	s4.y=35;
	strcpy(s5.name, "Bostanci"); 		s5.x=45; 	s5.y=20;
	strcpy(s6.name, "Kartal"); 			s6.x=55; 	s6.y=20;
	strcpy(s7.name, "Samandira"); 		s7.x=60; 	s7.y=40;
	strcpy(s8.name, "Icmeler"); 		s8.x=70; 	s8.y=15;
	
	
	//Add several metro stations to the given metro lines.
	addStation(&red, s1); addStation(&red, s2); addStation(&red, s3); addStation(&red, s4); addStation(&red, s5); addStation(&red, s8);
	
	addStation(&blue, s2); addStation(&blue, s3); addStation(&blue, s4); addStation(&blue, s6); addStation(&blue, s7);
	
	addStation(&green, s2); addStation(&green, s3); addStation(&green, s5); addStation(&green, s6); addStation(&green, s8);
	
	// Add red, blue, green metro lines to the Istanbul metro system.
	addLine(&istanbul, red);
	addLine(&istanbul, blue);
	addLine(&istanbul, green);
	
	// print the content of the red, blue, green metro lines
	printLine(red);
	printLine(blue);
	printLine(green);

		
	// find the nearest stations to the current and target locations
	MetroStation nearMe = findNearestStation(istanbul, myX, myY);
	MetroStation nearGoal = findNearestStation(istanbul, goalX, goalY);
	
	printf("\n");
	
	printf("The best path from %s to %s is:\n", nearMe.name, nearGoal.name);
	
	// if the nearest current and target stations are the same, then print a message and exit.
	if(equals(nearMe, nearGoal)){
		printf("It is better to walk!\n");
		return 0;
	}
	
	// Calculate and print the myPath with the minimum distance travelled from start to target stations.
	findPath(nearMe, nearGoal, myPath);
	
	if(strlen(myPath[0].name) == 0)
		printf("There is no path on the metro!\n");
	else{
		printPath(myPath);
	}
	
	return 0;

}

//Check whether stations are equal
int equals(MetroStation s1, MetroStation s2) {
	
	int isEqual;
	isEqual = (strcmp(s1.name, s2.name) == 0) ? 1 : 0;
	return isEqual;
}

//Add station to end of array
void addStation(MetroLine* l1, MetroStation s1) {
	
	int x;
	for(x = 0;x < SIZE;x++) {
			
		if(strlen((*l1).MetroStations[x].name) == 0) {

			(*l1).MetroStations[x] = s1;
			break;
		}
		
	}
}

//Check whether line has a station as input s1
int hasStation(MetroLine l1, MetroStation s1) {
	
	int has = 0, x = 0;
	for(x = 0;x < SIZE; x += 1) {
		
		if(strcmp(s1.name, l1.MetroStations[x].name)) {
			continue;
		}
		else {
			has = 1;
			break;
		}
	}
	
	return has;
}

//Invoke first station from given Metroline
MetroStation getFirstStop(MetroLine l1) {
	
	MetroStation nullStation;
	
	if(strlen(l1.MetroStations[0].name) == 0) {
		return nullStation;
	}
	else {
	    return l1.MetroStations[0];
	}
}

//Return previous station
MetroStation getPreviousStop(MetroLine l1, MetroStation s1) {
	
	MetroStation nullStation;
	int queue, x;
	
	for(x = 0;x < SIZE;x += 1) {
		
		if(strcmp(s1.name, l1.MetroStations[x].name)) {
			continue;
		}
		else {
			queue = x;
		    break;
		}
	}
	
	if(queue == 0) {
		return nullStation;
	}
	else {
	    return l1.MetroStations[queue - 1];
	}
}

//Return next station
MetroStation getNextStop(MetroLine l1, MetroStation s1) {
	
	MetroStation nullStation;
	int queue, x;
	
	for(x = 0;x < SIZE;x += 1) {
		
		if(strcmp(s1.name, l1.MetroStations[x].name)) {
			continue;
		}
		else {
			queue = x;
		    break;
		}
	}
	
	if(queue == SIZE - 1) {
		return nullStation;
	}
	else {
	    return l1.MetroStations[queue + 1];
	}	
}

//Add line to end of system
void addLine(MetroSystem* s1, MetroLine l1) {
	
	int x;
	for(x = 0;x < SIZE;x++) {
			
		if(strlen((*s1).MetroLines[x].color) == 0) {

			(*s1).MetroLines[x] = l1;
			break;
		}
		
	}
}

//Print given line
void printLine(MetroLine l1) {
	
	printf("Metroline %s: ", l1.color);
	
	int x;
	for(x = 0;x < SIZE;x += 1) {
		
		if(strlen(l1.MetroStations[x].name) != 0 && x != 0) {
			printf(",  %s", l1.MetroStations[x].name);
		}
		else if(x == 0) {
			printf("%s", l1.MetroStations[x].name);
		}
		else {
			printf("%s.\n", l1.MetroStations[x].name);
			break;
		}
	}
}

//Print given stations
void printPath(MetroStation MetroStations[]) {
	
	int x = 0;
	while(strlen(MetroStations[x].name) != 0) {
		
		printf("%d. %s\n", x+1, MetroStations[x].name);
		x++;
	}
}

//Return travelled distance
double getDistanceTravelled(MetroStation MetroStations[]) {
		
	double distance = 0;
	int numOfStat = 0;
	int z, difX, difY;
	
	for(z = 0;z < SIZE;z += 1) {
		
		if(strlen(MetroStations[z].name) != 0) {
			numOfStat++;
		}
	}
	
	if(numOfStat < 2) {
		return 0.0;
	}
	else {
		while(numOfStat - 1) {

			difX = MetroStations[numOfStat - 1].x - MetroStations[numOfStat - 2].x;
			difY = MetroStations[numOfStat - 1].y - MetroStations[numOfStat - 2].y;
			
			distance += sqrt((difX * difX) + (difY * difY));
			
			numOfStat--;
		}
		
		return distance;
	}
}

//Compute nearest station with x and y coordinates
MetroStation findNearestStation(MetroSystem s1, double x, double y) {
	
	int lines, stations;
	int nearestLine, nearestStation;
	double  difX, difY, prevdistance, currdistance, shortestDistance;

	for(lines = 0;lines < SIZE;lines++) {
    	for(stations = 0;stations < SIZE;stations++) {
    		
			//Find the distance for each station  	
    	    if(strlen(s1.MetroLines[lines].color) != 0) {
			    if(strlen(s1.MetroLines[lines].MetroStations[stations].name) != 0) {
			
			        //Calculate differences x,y
				    difX = s1.MetroLines[lines].MetroStations[stations].x - x;
		        	difY = s1.MetroLines[lines].MetroStations[stations].y - y;
			    
			        //if current station is not first stop, assign current to previous
			        if( !(lines == 0 && stations == 0) ) {
			         	prevdistance = currdistance;
			    				
			            currdistance = sqrt((difX * difX) + (difY * difY));

		     	        if(shortestDistance > currdistance) {
			         	    nearestLine = lines;
			             	nearestStation = stations;
			             	shortestDistance = currdistance;
			         	}
			    	}
			    	else {
			    		nearestLine = lines;
			        	nearestStation = stations;
			        	
			        	currdistance = sqrt((difX * difX) + (difY * difY));
			        	shortestDistance = currdistance;
			    	}
			    }
			}
		}
	}
	
	return s1.MetroLines[nearestLine].MetroStations[nearestStation];
}

//Collect all neighboring stations
void getNeighboringStations(MetroSystem sys1, MetroStation st1, MetroStation neigboringStations[]) {
	
	int lineindex, stationindex, x, y;
	int added = 0;
	
	for(x = 0;x < SIZE;x++) {
    	for(y = 0;y < SIZE;y++) {
    		
    		//Find the distance for each station  	
    	    if(strlen(sys1.MetroLines[x].color) != 0) {
			    if(strlen(sys1.MetroLines[x].MetroStations[y].name) != 0){
			    	if(strcmp(st1.name, sys1.MetroLines[x].MetroStations[y].name) == 0) {

			    		lineindex = x;
			    		stationindex = y;
			    		
			    		if(stationindex == 0) {
						    neigboringStations[added] = sys1.MetroLines[lineindex].MetroStations[stationindex + 1];
							added++;	
						}
						else if(stationindex == SIZE - 1) {
							neigboringStations[added] = sys1.MetroLines[lineindex].MetroStations[stationindex - 1];
							added++;
						}
						else {
							neigboringStations[added] = sys1.MetroLines[lineindex].MetroStations[stationindex - 1];
							added++;
							neigboringStations[added] = sys1.MetroLines[lineindex].MetroStations[stationindex + 1];
							added++;
						}	
					}
			    }
			}
		}
	}
	
}

//Find path
void findPath(MetroStation start, MetroStation finish, MetroStation path[]) {
	
	//Define partialPath
	MetroStation partialPath[SIZE] = {'\0'};
	
	//Recursive function
	recursiveFindPath(start, finish, path, partialPath);
	
	int x = 0;
	while(strlen(partialPath[x].name) != 0) {
		path[x] = partialPath[x];
		x++;
	}
}

//Use recursion
void recursiveFindPath(MetroStation start, MetroStation finish, MetroStation partialPath[], MetroStation bestPath[]) {

	//Define a neighbor array
	MetroStation neighbors[SIZE] = {'\0'};
	getNeighboringStations(istanbul,start,neighbors);
	MetroStation currentPath[SIZE] = {'\0'};
	
	//If inputs cointained, return
	int x;
	for(x = 0;x < SIZE;x++) {

		if(strcmp(partialPath[x].name, start.name) == 0)
			return;
    }
    
    
	//Transfer contents to duplicate array
	MetroStation duplicatePath[SIZE] = {'\0'};
	int i;
	for(i = 0;i < SIZE;i++) {
		duplicatePath[i] = partialPath[i];
	}

	//Add start
	int j = 0;
	while(strlen(duplicatePath[j].name) != 0)
		j++;

	duplicatePath[j] = start;
	
	//Check equal
	if(equals(start, finish)) {

		//If true assign to best
		for(x = 0;x < SIZE;x++) {
			
			bestPath[x] = duplicatePath[x];
		}

		return;
	}

	
	if(currentPath != NULL) {
		getDistanceTravelled(currentPath);
	}

	//Find neighbors
	int z;
	MetroStation neighbor;
	for(z = 0;z < SIZE;z++) {
		
		if(strlen(neighbors[z].name) != 0) {
			
			//Calculate x and y distances
			disX = neighbors[z].x - start.x;
		    disY = neighbors[z].y - start.y;
		    disXtoFinish = finish.x - neighbors[z].x;
		    disYtoFinish = finish.y - neighbors[z].y;

		    predistOfNeigh = disOfNeigh;
		    predistoFinish = distoFinish;
		
		    //If we get first station, do following
		    if(predistOfNeigh == 0) {
		    	predistOfNeigh = sqrt(disX * disX + disY * disY);
		    	disOfNeigh = sqrt(disX * disX + disY * disY);
		    	shortDisOfNeigh = disOfNeigh;
		    	
		    	predistoFinish = sqrt(disXtoFinish * disXtoFinish + disYtoFinish * disYtoFinish);
		    	distoFinish = sqrt(disXtoFinish * disXtoFinish + disYtoFinish * disYtoFinish);
		    	shortDistoFinish = distoFinish;
		    	
		    	neighbor = neighbors[z];
	    	}
		
		
	     	disOfNeigh = sqrt(disX * disX + disY * disY);
	     	distoFinish = sqrt(disXtoFinish * disXtoFinish + disYtoFinish * disYtoFinish);
		
		    //If we have shorter distance than previous one,substitute it
	    	if(shortDistoFinish >= distoFinish) {
	    		
	    		shortDisOfNeigh = disOfNeigh;
	    		shortDistoFinish = distoFinish;
	    		
	     		neighbor = neighbors[z];
	    	}
	 	}
	}

	//Recursive function
	recursiveFindPath(neighbor, finish, duplicatePath, bestPath);
}
