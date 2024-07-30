#include <iostream>
#include <vector>
#include <bitset>

#include "../../src/graphex.h"

using namespace graphex::petrinet;
using namespace std;

//choose the node size of the graph network
typedef Graph<PL_frame<int,int>,TR_index_frame<int,int>> T_graph;

//recursive builder for fan in, returns indexes of leafnodes
int r_fan_in(T_graph* pn, vector<int>& leafnodes, int depth, int previous){
	
	int temp_tr_index; vector<int> temp_places_match;
	pn->find_pg(previous, temp_places_match);
	
	if(depth == 0){
		leafnodes.push_back(previous);
		return 1;
	} else {
		for(int i=0; i<4; i++){
			temp_tr_index = pn->add("pn/g_fan_in_4.lpn",std::vector<pattern>{{"out1","in"+to_string(i+1),temp_places_match}});
			r_fan_in(pn, leafnodes, depth - 1, temp_tr_index);
		}
		return 1;
	}
	return 1;
}

int r_fan_in(T_graph* pn, vector<int>& leafnodes, int depth){
	
	leafnodes.push_back(pn->add("pn/g_fan_in_4.lpn",vector<pattern>{}));
	r_fan_in(pn, leafnodes, depth, leafnodes[0]);
	return 1;
}

//recursive builder for fan out, returns indexes of leafnodes
int r_fan_out(T_graph* pn, vector<int>& leafnodes, int depth, int previous){
	
	int temp_tr_index; vector<int> temp_places_match;
	pn->find_pg(previous, temp_places_match);
	
	if(depth == 0){
		leafnodes.push_back(previous);
		return 1;
	} else {
		for(int i=0; i<4; i++){
			temp_tr_index = pn->add("pn/g_fan_out_4.lpn",std::vector<pattern>{{"in1","out"+to_string(i+1),temp_places_match}});
			r_fan_out(pn, leafnodes, depth - 1, temp_tr_index);
		}
		return 1;
	}
	return 1;
}

int r_fan_out(T_graph* pn, vector<int>& leafnodes, int depth){
	
	leafnodes.push_back(pn->add("pn/g_fan_out_4.lpn",vector<pattern>{}));
	r_fan_out(pn, leafnodes, depth, leafnodes[0]);
	return 1;
}

int main(){
	
	Graph<PL_frame<int,int>,TR_index_frame<int,int>> large_and_gate;
	int temp_tr_index; vector<int> temp_places_match;
	
	vector<int> leafnodes_in;
	vector<int> leafnodes_out;
	//leafnodes.push_back(large_and_gate.add("pn/g_fan_in.lpn",vector<pattern>{}));
	//r_fan_in(&large_and_gate, leafnodes_in, 2);
	
	r_fan_out(&large_and_gate, leafnodes_out, 2);
	large_and_gate.find_pg(leafnodes_out[0], temp_places_match);
	
	uint8_t input = 0;
	
	large_and_gate.attach({"x_in1_1","",temp_places_match});
	
	large_and_gate.get(0, input);
	cout<<"here is the input: "<<(int)input<<endl;
	
	large_and_gate.set(0, 1);
	
	large_and_gate.get(0, input);
	cout<<"here is the input: "<<(int)input<<endl;
	
	for(int i = 0; i<100 ; i++){
		large_and_gate.execute(Exe_mode::Sequence, Exe_mode::Sequence);
	}
	large_and_gate.print_pl();
	
	
	//r_fan_out(&large_and_gate, leafnodes_out, 4);
	
	/*
	for(auto& i:leafnodes){
		cout<<"index: "<<i<<endl;
	}
	
	cout<<"size: "<<leafnodes.size()<<endl;
	*/
	
}
