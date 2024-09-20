#include <iostream>
#include <vector>
#include <bitset>
#include <cmath>
#include <chrono>

#include "../../src/graphex.h"

using namespace graphex::petrinet;
using namespace std;

#pragma pack(1) 

//choose the node size of the graph network
typedef Graph<PL_frame<bool,uint8_t>,TR_index_frame<uint8_t,uint8_t>> T_graph;

int r_argmax(T_graph* pn, vector<int>& leafnodes, int depth, int previous){

	int temp_tr_index; vector<int> temp_places_match;
	pn->find_pg(previous, temp_places_match);

	if(depth == 0){
		leafnodes.push_back(previous);
	} else {


	}

}

int r_argmax(T_graph* pn, vector<int>& leafnodes, int depth){
	
	leafnodes.push_back(pn->add("pn/argmax_cell.lpn", vector<pattern>{}));
	r_argmax(pn, leafnodes, depth - 1, leafnodes[0]);

}

int main(){

	T_graph arg_max;

	int temp_tr_index;



	cout<<"testing"<<endl;
	return 1;
}
