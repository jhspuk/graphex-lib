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
	
	g.set(1, 1);
	
	#define LOOP_SIZE 100
	
	for (int i = 0; i < (LOOP_SIZE - 2); i++){
		//assemble a single pipeline
		g.find_pg(ti, tp);
		ti = g.add("./pn/loop_round_part.lpn", ps{{"in","out",tp}});
	}
	
	g.find_pg(ti, tp);
	ti = g.add("./pn/loop_round_part.lpn", ps{{"out","outx",{}},{"in","out",tp}});
	
	//at this point we have a long pipeline with a start and an end
	//in order to make a loop, we will need to connect the start and the
	//end, forming a circle. We can use ::join() to make the connection
	
	//get the place groups of the first pipeline module
	g.find_pg(ti2, tp2);
	//make a connection from ([last, this], to [last -> first in pattern, that])
	//g.join(ti, ps{{"outx","in",tp2}});
	
	//now execute and see if counter makes it around the pipeline...
	for(int i = 0; i< 1000; i++){
		g.execute(Exe_mode::Sequence,Exe_mode::Random);
	}
	
	g.print_pl();
	
};
