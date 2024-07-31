//Description: Generates 4^n 8 bit AND gates, and creates global in, global out
//lines for each using a recursive mux pattern. Additional behaviour can be added
//by modifying g_scalable_and_gate.lpn
//Tested up to 4^10 or 1,048,576 AND gates. This is equivalent to 1 megabyte of
//data being processed. This consumed 22GB of ram, and 2 * 1398100 mux modules
//were required for the recursive pattern. Overall this used 16,430,014 individual
//place regions, and over 200,000,000 places/transitions!

//TODO: Make AND gate recursive ... Currently output segmented but visible
//with print_pl

//Library discoveries: Need .join() functionality and the ability to synthesise
//large mux elements.


#include <iostream>
#include <vector>
#include <bitset>
#include <cmath>

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
	int temp_tr_index; vector<int> temp_places_match; vector<int> temp_places_match_2; vector<int> temp_places_match_3;
	
	int pow_4 = 4; //number of modules to create (4^n)
	
	//set up global 'ex' or 'calculate' line (fanning out)
	//topology requires fanning out to many AND gates
	vector<int> leafnodes_out; int GS_ex_in;
	r_fan_out(&large_and_gate, leafnodes_out, pow_4 - 1);
	large_and_gate.find_pg(leafnodes_out[0], temp_places_match);
	GS_ex_in = large_and_gate.attach({"x_in1_1","",temp_places_match});
	leafnodes_out.erase(leafnodes_out.begin());
	
	//set up global 'inference' or 'calculate' line (fanning in)
	//topology requires fanning in from many AND gates as halting signal
	vector<int> leafnodes_in; int GS_ex_out;
	r_fan_in(&large_and_gate, leafnodes_in, pow_4 - 1);
	large_and_gate.find_pg(leafnodes_in[0], temp_places_match);
	GS_ex_out = large_and_gate.attach({"x_out1_1","",temp_places_match});
	leafnodes_in.erase(leafnodes_in.begin());
	
	//create scalable and gate modules to the power of four
	vector<int> GS_and;
	for(int i = 0; i < pow((double)4, (pow_4 - 1)); i++){
		temp_places_match.clear();
		large_and_gate.find_pg(leafnodes_out[i], temp_places_match);
		large_and_gate.find_pg(leafnodes_in[i], temp_places_match_2);


		for(int k = 0; k < 4; k++){
			temp_tr_index = large_and_gate.add("pn/g_scalable_and_gate.lpn",std::vector<pattern>{{"ex1i","out"+to_string(k+1),temp_places_match},{"ex1o","in"+to_string(k+1),temp_places_match_2}});
			large_and_gate.find_pg(temp_tr_index, temp_places_match_3);
			GS_and.push_back(large_and_gate.attach(std::vector<pattern>{{"x_inc1_2","",temp_places_match_3},
				{"x_inc2_2","",temp_places_match_3},
				{"x_inc3_2","",temp_places_match_3},
				{"x_inc4_2","",temp_places_match_3},
				{"x_inc5_2","",temp_places_match_3},
				{"x_inc6_2","",temp_places_match_3},
				{"x_inc7_2","",temp_places_match_3},
				{"x_inc8_2","",temp_places_match_3}
				},std::vector<pattern>{{"x_inc1_1","",temp_places_match_3},
				{"x_inc2_1","",temp_places_match_3},
				{"x_inc3_1","",temp_places_match_3},
				{"x_inc4_1","",temp_places_match_3},
				{"x_inc5_1","",temp_places_match_3},
				{"x_inc6_1","",temp_places_match_3},
				{"x_inc7_1","",temp_places_match_3},
				{"x_inc8_1","",temp_places_match_3},
				},GS_vars::byte_1));
				cout<<"Gate "<< i << ", "<< k<< endl;
		}
	}
	
	for(auto&i : GS_and){
		//input vector repeated
		large_and_gate.set(i, 255);
	}
	
	//large_and_gate.print_pl();
	
	large_and_gate.set(GS_ex_in, 1);
	cout<<"Start!"<<endl;
	uint8_t end_f = 0;
	
	for(int i = 0;i<10000000;i++){
		for(int k = 0; k<10; k++){
			large_and_gate.execute(Exe_mode::Sequence, Exe_mode::Random);
		}
		large_and_gate.get(GS_ex_out, end_f);
		if(end_f != 0){
			cout<<"Finished!"<<endl;
			break;
		}
		cout<<"Loop "<<i<<endl;
	}
	
	cout<<"Finished!"<<endl;
	large_and_gate.print_pl();
		
}
