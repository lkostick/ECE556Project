// ECE556 - Copyright 2014 University of Wisconsin-Madison.  All Rights Reserved.

#include "ece556.h"

int readBenchmark(const char *fileName, routingInst *rst){
  /*********** TO BE FILLED BY YOU **********/  
	using namespace std;
	int gx, gy;
	char* str;

	ifstream inf(fileName);
	if(!inf){
		return 0;
	}
	std:: string token;
	std:: string strInput;	
	while(inf){
		getline(inf, strInput);
		while(getline(strInput, token, " ")){
				cout << token;
				cout << strInput << endl;
		}
	}
	

  return 1;
}

int solveRouting(routingInst *rst){
  /*********** TO BE FILLED BY YOU **********/

  return 1;
}

int writeOutput(const char *outRouteFile, routingInst *rst){
  /*********** TO BE FILLED BY YOU **********/

  return 1;
}


int release(routingInst *rst){
  /*********** TO BE FILLED BY YOU **********/
	int i = j = k = 0;
	int sizeNets = sizePins = sizeSegments = 0;
	if(rst == NULL) return 0;
	sizeNets = rst->numNets;
	for(i = 0; i < sizeNets; i++){ // Iterate through all the nets in the routing instance
		sizePins = rst->nets[i].numPins;
		for(j = 0; j < sizePins; j++){ // Iterate through the pins for each net struct
			free(rst->nets[i].pins[j]);
		}
		sizeSegments = rst->nets[i].nroute.numSegs;
		for(k = 0; k < sizeSegments; k++){ // Iterate through the segements in the route 
			free(rst->nets[i].nroute.segments[k].edges);
			free(rst->nets[i].nroute.segments[k]);
		}
		free(rst->nets[i].nroute);
		free(rst->nets[i]);
	}
	free(rst->edgeCaps);
	free(rst->edgeUtils);
	free(rst);
  return 1;
}
  

