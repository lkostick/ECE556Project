// ECE556 - Copyright 2014 University of Wisconsin-Madison.  All Rights Reserved.

#include "ece556.h"

int readBenchmark(const char *fileName, routingInst *rst){
	/*********** TO BE FILLED BY YOU **********/  
	char trash[10], g1[10], g2[10], capacity[10], n0[10], n1[10], nodeName[10], numNets[10], numNodes[10];
	int i, j, intNumNets, intNumNodes;
	FILE *input;
  
  if((input = fopen(fileName, "r")) == NULL){
	  fprintf(stderr, "Could not read file\n");
	  return 1;
  }
  
  	fscanf(input, "%s %s %s", trash, g1, g2);
  	(*rst).gx = atoi(g1);
  	(*rst).gy = atoi(g2);
  	fprintf(stderr,"The input was: %s\n", trash);
  	
	fscanf(input, "%s %s\n", trash, capacity);
	(*rst).cap = atoi(capacity);
	fprintf(stderr,"%s %s\n", trash, capacity);
	
	fscanf(input, "%s %s %s\n", trash, trash, numNets);
	fprintf(stderr,"%s %s %s\n", trash, trash, numNets);
	intNumNets = atoi(numNets);
	(*rst).numNets = intNumNets;
	
	net  *newNetArray;
	newNetArray = (net*)malloc(intNumNets*sizeof(net));
	((*rst).nets)= newNetArray;
	fprintf(stderr,"it works?\n");
	
	for(i = 0; i < intNumNets; i++){
		
		fscanf(input, "%s %s\n", nodeName, numNodes);
		intNumNodes = atoi(numNodes);
		fprintf(stderr, numNodes+"\n");
		
		net *newNet = (net*)malloc(sizeof(net));
		(*newNet).numPins = intNumNodes;
		
		for(j = 0; j < intNumNodes; j++){
			
			fscanf(input, "%s %s\n", n0, n1);
			fprintf(stderr, "%s %s\n", n0, n1);
			
			
			point *newPoint = (point*)malloc(sizeof(point));
			(*newPoint).x = atoi(n0);
			(*newPoint).y = atoi(n1);
			fprintf(stderr, "here\n");
			(*newNet).pins[j] = *newPoint;
		}
		(*rst).nets[i] = *newNet;
	}
	
  fclose(input);// closes the file we opened

  return 1;

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
  

