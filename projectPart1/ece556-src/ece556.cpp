// ECE556 - Copyright 2014 University of Wisconsin-Madison.  All Rights Reserved.

#include "ece556.h"


// This function reads an input file and initializes all of the information
// This function reads an input file and initializes all of the information
// read from the file into a Routing instance
// Input: fileName is the name of the file we will read from
// Input: rst is a pointer to the routing instance we will save all of
// 			the inputs to
// Output: We will output 1 on successful completion of function
int readBenchmark(const char *fileName, routingInst *rst){
	
	// initializes c-strings in order to store the information from the
	// input file
	char trash[10], g1[10], g2[10], capacity[10], n0[10], n1[10], netName[10], numNets[10], numbPins[10],numBlockages[100];
	char x1[100], x2[100], y1[100], y2[100], newCap[100];
	// initializes ints used in the rest of the function
	int i, j, intNumNets, intNumPins, h1, h2, intNumEdges, intNumBlockages;
	int intx1, inty1, intx2, inty2, intnewCap, edge;
	// Initializes the FILE pointer we will use to read the input file
	FILE *input;
  if((input = fopen(fileName, "r")) == NULL){
	  fprintf(stderr, "Could not read file\n");
	  fprintf(stderr, "pointer: %p\n", input);
	  return 1;
  }

  
	// This scan and store statement scans the first line of the input
	// file and stores the grid size gx and gy
  	fscanf(input, "%s %s %s", trash, g1, g2);
  	(*rst).gx = atoi(g1);
  	(*rst).gy = atoi(g2);
  	
  	// This scan and store statement scans the second line of the input
  	// file and stores the capacity of the grid
	fscanf(input, "%s %s\n", trash, capacity);
	(*rst).cap = atoi(capacity);
	 	
	// This scan and store statment scans the third line of the input 
	// file and stores the number of nets there are
	fscanf(input, "%s %s %s\n", trash, trash, numNets);
	intNumNets = atoi(numNets);
	(*rst).numNets = intNumNets;
	
	// These statements allocates a new array of nets in order to store
	// all of the nets that will be input from the input file
	net  *newNetArray;
	newNetArray = (net*)malloc(intNumNets*sizeof(net));
	

	// This for loop repeats for each net we will read from the input 
	// file
	for(i = 0; i < intNumNets; i++){
		
		// This scan reads the net name and the number of points that
		// are in the net
		fscanf(input, "%s %s\n", netName, numbPins);
		intNumPins = atoi(numbPins);
		newNetArray[i].id = i;
		newNetArray[i].numPins = intNumPins;
		
		// Initilizes an array of points in order to store the next
		// inputs from read from the input file in order to store all
		// of the points for the net
		point *newPins = (point*) malloc(intNumPins*sizeof(point));
		
		// This for loop repeats for each number of points that are in
		// the net and saves the points to the array created above
		for(j = 0; j < intNumPins; j++){
			
			// This scan and store statement scans the input file and 
			// allocates a new point in order to store the information
			// scanned. Then saves it to the net.
			fscanf(input, "%s %s\n", n0, n1);
			point newPoint;
			(newPoint).x = atoi(n0);
			(newPoint).y = atoi(n1);
			newPins[j]= newPoint;
			
		}
		newNetArray[i].pins = newPins;	
		
	}
		
	
	// Saves the net array we just finished storing points to to the 
	// Routing Instance we are using.
	(*rst).nets = newNetArray;
	
	
	
	// This creates ints that store the size of the grid we are using
	h1 = atoi(g1);
	h2 = atoi(g2);

	// This creates an int that stores the total number of edges we have
	// and then saves it to the Routing Instance
	intNumEdges = (h1*(h2-1))+(h2*(h1-1));
	(*rst).numEdges = intNumEdges; 

	
	// This creates an array of ints that we will use to store the
	// capacities of each edge
	(*rst).edgeCaps = (int*)malloc(intNumEdges*sizeof(int));


	// This for loop sets each edge capacity to the default
	for(i = 0; i < (intNumEdges); i++){
		(*rst).edgeCaps[i] = atoi(capacity);
	}

	fscanf(input, "%s\n", numBlockages);
	intNumBlockages = atoi(numBlockages);
	for(i = 0; i < intNumBlockages; i++){
		
		fscanf(input, "%s %s %s %s %s\n", x1, y1, x2, y2, newCap);
		intx1 = atof(x1);
		inty1 = atof(y1);
		intx2 = atof(x2);
		inty2 = atof(y2); 
		intnewCap = atoi(newCap);
		edge = 0;
		//fprintf(stderr, "x1:%s y1:%s x2:%s y2:%s Cap:%s\n", x1, y1, x2, y2, newCap);
		
		edge = pointsToEdge(h1, h2, intx1, inty1, intx2, inty2);
		
		(*rst).edgeCaps[edge] = intnewCap;
	}
	(*rst).edgeUtils = (int*)malloc(intNumEdges*sizeof(int));
	for(i = 0; i < (intNumEdges); i++){
		(*rst).edgeUtils[i] = 0;
	}
	fclose(input);
 
  return 1;
}

// This is a simple routing solver. We will create a solution that will
// not account for any blockages.
// Input: rst is a pointer to a Routing Instance that we want to solve for
// Output: We will output 1 on successful completion of the function
int solveRouting(routingInst *rst){

int i, j, k;
int numbNets, numbPins;
numbNets = (*rst).numNets;

// This loop goes through each net, initializing a route and making 
// the segments needed to populate the route
for(i = 0; i < numbNets; i ++){
	
	
	//route newRoute = (route)malloc(sizeof(route));
	
	numbPins = (*rst).nets[i].numPins;
	segment *segs = (segment*)malloc(sizeof(segment)*(numbPins-1));
	int edgeCount;
	
	
	// This loop creates all of the segmets that we will need to solve for
	for(j = 0; j < (numbPins-1); j ++){
		
		// 
		point pin1 = (*rst). nets[i].pins[j];
		point pin2 = (*rst). nets[i].pins[j+1];
		
		segment curSeg;
		
		// these statements initialize and store the difference in 
		// vertical and horizontal change between points
		int changex = abs((pin1.x) - (pin2.x));
		int changey = abs((pin1.y) - (pin2.y));
		curSeg.numEdges = changex+changey;
		curSeg.p1 = pin1;
		curSeg.p2 = pin2;
		
		int *curEdges = (int*)malloc(sizeof(int)*curSeg.numEdges);
		edgeCount = 0;
		int gridx, gridy;
		gridx = (*rst).gx;
		gridy = (*rst).gy;
		
		// This loop creates all of the horizontal edges
		for(k = 0; k < changex; k ++){
			if(pin1.x < pin2.x){
				curEdges[edgeCount] = pointsToEdge(gridx, gridy, pin1.x + k, pin1.y, (pin1.x) + 1 + k, pin1.y);
				edgeCount++;
			}else{
				curEdges[edgeCount] = pointsToEdge(gridx, gridy, pin2.x + k, pin2.y, (pin2.x) + 1 + k, pin2.y);
				edgeCount++;
			}
		}
		// This loop creates all of the vertical edges
		for(k = 0; k < changey; k ++){
			if(pin1.y < pin2.y){
				curEdges[edgeCount] = pointsToEdge(gridx, gridy, pin1.x, pin1.y + k, pin1.x, (pin1.y) + 1 + k);
				edgeCount++;
			}else{
				curEdges[edgeCount] = pointsToEdge(gridx, gridy, pin2.x, pin2.y + k, pin2.x, (pin2.y) + 1 + k);
				edgeCount++;
			}
		}
		curSeg.edges = curEdges;
		segs[j] = curSeg;
		
	} 
	(*rst).nets[i].nroute.segments = segs;
	(*rst).nets[i].nroute.numSegs = numbPins -1;
}

	// Returns 1 on successful completion
  return 1;
}


// This function will output the solution of the Routing Instance
// to a txt file.
// Input: outRouteFile is the name of the file we will output the
// 			solution to
// Input: rst is the Routing instnace that we will output the solution for
// Output: We will output 1 on successful completion of the function
int writeOutput(const char *outRouteFile, routingInst *rst){

int i, j ,numberSegs;
FILE *output;

 
if((output = fopen(outRouteFile, "w")) == NULL){
	  fprintf(stderr, "Could not read file\n");
	  return 0;
}

int xDistance, newX;

for(i = 0; i < (*rst).numNets; i ++){
	
	fprintf(output, "n%d\n", (*rst).nets[i].id);

	numberSegs = (*rst).nets[i].nroute.numSegs;
	for(j = 0; j < numberSegs; j ++){

		// initializes new points and sets them equal to the start and
		// end point of the segment. Then prints out the start and 
		// end point to the file in the appropriate format.
		point point1, point2;
		point1 = (*rst).nets[i].nroute.segments[j].p1;
		point2 = (*rst).nets[i].nroute.segments[j].p2;
		xDistance = abs(point1.x-point2.x);
		if(point1.x < point2.x){
			newX = point1.x + xDistance;
			fprintf(output,"(%d,%d)-(%d,%d)\n", point1.x, point1.y, point2.x, point1.y);
		}
		else if(point2.x<point1.x){
			newX = point2.x + xDistance;
			fprintf(output,"(%d,%d)-(%d,%d)\n", point2.x, point2.y, point1.x, point2.y);
		}
		if(point1.y < point2.y){
			if(point1.x > point2.x){
				newX = point1.x;
			}else{
				newX = point2.x;
			}
			fprintf(output,"(%d,%d)-(%d,%d)\n", newX, point1.y, newX, point2.y);
		}
		else if(point2.y<point1.y){
			if(point1.x > point2.x){
				newX = point1.x;
			}else{
				newX = point2.x;
			}    
			fprintf(output,"(%d,%d)-(%d,%d)\n", newX, point2.y, newX, point1.y);
		}
		
		//fprintf(output, "(%d,%d)-(%d,%d)\n", point1.x, point1.y, point2.x, point2.y);
	} 
fprintf(output, "!\n");
}
	
	
	
// Returns one on successful completion
return 1;
}


// This function releases the memory that we allocated throughout the 
// use of this program
// Input: rst contains all of the information that we allocated 
//			throughout the use of this program
// Output: We will output 1 on successful completion of deallocation of
//			the memory.
int release(routingInst *rst){
  /*********** TO BE FILLED BY YOU **********/
  
	int i,j;
	int sizeNets,sizeSegments;
	if(rst == NULL) return 0;
	sizeNets = rst->numNets;
	for(i = 0; i < sizeNets; i++){ // Iterate through all the nets in the routing instance
		free(rst->nets[i].pins);
		sizeSegments = rst->nets[i].nroute.numSegs;
		for(j = 0; j < sizeSegments; j++){ // Iterate through the segements in the route 
			free(rst->nets[i].nroute.segments[j].edges);
		}
		free(rst->nets[i].nroute.segments);
	}
	free(rst->nets);
	free(rst->edgeCaps);
	free(rst->edgeUtils);
	free(rst);
	
  return 1;
  
}


// This funciton is used to convert a start point(x1,y1) and end point
// (x2,y2) into an edge number given the size of the grid x,y
// Input: x,y are the grid size
// Input: (x1,y1),(x2,y2) are the start and end points of the edge
//			respectively
// Return: Returns the integer that is the edge number
int pointsToEdge(int x, int y, int x1, int y1, int x2, int y2){
	
int edge;
if(y1 == y2){
	
		edge = (x-1)*y1;
		if(y1 == 0){
			edge = edge -1;
		}
		
		if(x1 < x2){
			edge = edge + x1;
		}else{
			edge = edge + x2;
		}
}else{
	edge = ((x-1)*y);
	edge = edge + x1;
	if(y1 < y2){
		edge = edge + (x*y1);
		if(y1 == 0){
			edge = edge -1;
		}
	}else{
		edge = edge + (x*y2);
		if(y2 == 0){
			edge = edge -1;
		}
	}
}
	return edge;
}


// This function is used to convert an edge number to the start point and
// end point of the edge. 
// Input: x,y are the grid size
// Input: edgeNum is the edge number
// Input: retInts: is a pointer to an array of ints that we will use to
//			return the start and end points to the user. Must be an
// 			array of 4 ints.
// Return: Returns 1 on successful completion
int edgeToPoints(int x, int y, int edgeNum, int* retInts){
	 
int numHoriz, width;

numHoriz = (x-1)*(y);
width = x - 1;

			
if(edgeNum <(numHoriz-1)){
	//Determines if the edge is horizontal
	retInts[0] = edgeNum % (width);
	retInts[2] = retInts[0] + 1;
	retInts[1] = edgeNum / (width);
	retInts[3] = retInts[1];
}else{
	//Otherwise we know that the edge is vertical
	edgeNum = edgeNum - numHoriz ;
	retInts[0] = (edgeNum % (width+1));
	retInts[2] = retInts[0];
	retInts[1] = edgeNum / (width+1);
	retInts[3] = retInts[1] + 1;
}

	return 1;
}
