#include <iostream>
#include <vector>
#include <bitset>
#include <cmath>

#include "../../src/graphex.h"

using namespace graphex::petrinet;
using namespace std;

//choose the node size of the graph network
typedef Graph<PL_frame<int,int>,TR_index_frame<int,int>> T_graph;
//alias common type definitions...
typedef vector<int> wl;
typedef vector<pattern> ps;

int main(){
	
	T_graph g;		//graph network
	int ti; 		//temp index
	int ti2;
	wl tp;			//temp place groups
	wl tp2;
	
	//add the very first loop part
	ti = g.add("./pn/loop_round_part.lpn", ps{});
	g.find_pg(ti, tp);
	
	//store first index for later...
	ti2 = ti;
	
	//attach single place readers
	g.attach({"x_in_1","",tp});
	g.attach({"x_in_2","",tp});
	
	bool testy = 0;
	
	g.get(0, testy);
	
	cout<<"Here's testy: "<<testy<<endl;
	
	g.set(0, 1);
	
	g.get(0, testy);
	
	cout<<"Here's testy: "<<testy<<endl;
	
	#define LOOP_SIZE 10
	
	for (int i = 0; i < (LOOP_SIZE - 1); i++){
		//assemble a single pipeline
		g.find_pg(ti, tp);
		ti = g.add("./pn/loop_round_part.lpn", ps{{"in","out",tp}});
	}
	
	//at this point we have a long pipeline with a start and an end
	//in order to make a loop, we will need to connect the start and the
	//end, forming a circle. We can use ::join() to make the connection
	
	//get the place groups of the last pipeline module
	g.find_pg(ti, tp);
	//get the place groups of the first pipeline module
	g.find_pg(ti2, tp2);
	//make a connection from ([last], to [last, first in pattern])
	g.join(tp, ps{{"out","in",tp2}});
	
	g.print_pl();
	
};
