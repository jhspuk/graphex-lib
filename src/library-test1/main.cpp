#include <iostream>
#include <tuple>
#include <vector>
#include <bitset>

#define DEBUG_BUILD

#include "graphex.h"

using namespace graphex::petrinet;

int recur_demo(Graph<PL_frame<int,int>,TR_index_frame<int,int>>* pn, int depth, int previous){
	
	int a; std::vector<int> b;
	pn->find_pg(previous,b);
	
	if(depth == 0){
		pn->attach(std::vector<pattern>{{"x_in1_b","",b},{"x_in1_b","",b},{"x_in1_b","",b},{"x_in1_b","",b},{"x_in1_d","",b},{"x_in2_d","",b},{"x_in3_d","",b},{"x_in4_d","",b}},std::vector<pattern>{{"x_in1_a","",b},{"x_in2_a","",b},{"x_in3_a","",b},{"x_in4_a","",b},{"x_in1_c","",b},{"x_in2_c","",b},{"x_in3_c","",b},{"x_in4_c","",b}},GS_vars::byte_1);
		return 1;
	} else {
		a = pn->add("../../PN-examples/4-1_mux_empties.lpn",std::vector<pattern>{{"out1","in1",b}});
		recur_demo(pn, depth - 1, a);
		a = pn->add("../../PN-examples/4-1_mux_empties.lpn",std::vector<pattern>{{"out1","in2",b}});
		recur_demo(pn, depth - 1, a);
		a = pn->add("../../PN-examples/4-1_mux_empties.lpn",std::vector<pattern>{{"out1","in3",b}});
		recur_demo(pn, depth - 1, a);
		a = pn->add("../../PN-examples/4-1_mux_empties.lpn",std::vector<pattern>{{"out1","in4",b}});
		recur_demo(pn, depth - 1, a);
		return 1;
	}
}




int main(){
	
	using namespace std;
	
	//Mux Example
	
	Graph<PL_frame<int,int>,TR_index_frame<int,int>> mux;
	int a; vector<int> b;
	
	a = mux.add("../../PN-examples/4-1_mux_empties.lpn",std::vector<pattern>{});
	recur_demo(&mux, 4, a);
	
	for(int i = 0; i < mux.gs_reg.size(); i++){
		mux.set(i, 0b01010101);
	}
	b.clear();
	mux.find_pg(0, b);
	for(auto&i : b){
		cout<<"Hits: "<<i<<endl;
	}
	
	int ovs = 0;
	ovs = mux.attach(std::vector<pattern>{{"x_in1_b","",b},{"x_in1_b","",b},{"x_in1_b","",b},{"x_in1_b","",b},{"x_in1_d","",b},{"x_in2_d","",b},{"x_in3_d","",b},{"x_in4_d","",b}},std::vector<pattern>{{"x_in1_a","",b},{"x_in2_a","",b},{"x_in3_a","",b},{"x_in4_a","",b},{"x_in1_c","",b},{"x_in2_c","",b},{"x_in3_c","",b},{"x_in4_c","",b}},GS_vars::byte_1);
	
	uint8_t out = 0;
	
	while(true){
		mux.get(ovs, out);
		cout<<bitset<8>(out)<<endl;
		//mux.print_pl();
		for(auto k = 0; k < 200; k++){
			for(auto i = 0; i < mux.tr_reg.size(); i++){
				mux.execute__base(i,Exe_mode::Random);
			}
		}

	}
	
	
	//TM Example
	/*
	Graph<PL_frame<int,int>,TR_index_frame<int,int>> tm;
	int a; vector<int> b;
	a = tm.add("/home/h/Documents/workspace/work/Microsystems/tsetlin-workcraft/alex/tmCompletePetriNetDemo.lpn", std::vector<pattern>{});
	tm.find_pg(a,b);
	tm.attach({"p2","",b});
	
	tm.attach({},std::vector<pattern>{{"cl1.c0.x1TA.exc1","",b},{"cl1.c0.x1TA.exc2","",b},{"cl1.c0.x1TA.exc3","",b},{"cl1.c0.x1TA.inc4","",b},{"cl1.c0.x1TA.inc5","",b},{"cl1.c0.x1TA.inc6","",b}},GS_vars::byte_1);
	//tm.attach({},std::vector<pattern>{{"cl1.c1.cx2TA.exc1","",b},{"cl1.c1.cx2TA.exc2","",b},{"cl1.c1.cx2TA.exc3","",b},{"cl1.c1.cx2TA.inc4","",b},{"cl1.c1.cx2TA.inc5","",b},{"cl1.c1.cx2TA.inc6","",b}},GS_vars::byte_1);
	
	uint8_t epochs = 0;
	uint8_t ta1 = 0;
	uint8_t ta2 = 0;
	
	tm.get(0, epochs);
	
	uint8_t count = 0;
	while(epochs>0){
		for(int i=0; i<1000; i++){
			tm.execute__base(0,Exe_mode::Random);
		}
		tm.get(0, epochs);
		if(count != epochs){
			tm.get(1, ta1);
			cout<<"cl1.c1.cx2TA: "<<std::bitset<6>(ta1)<<endl;
		}
	}
	

	//tm.get(1, ta2);
	
	
	cout<<(int)epochs<<endl;
	
	tm.print_pl();
	
	*/
	

	//cout<<(int)ta2<<endl;
	
/*	
	
	Graph<PL_frame<int,int>,TR_index_frame<int,int>> test;
	int a; vector<int> b;
	//test.add("../../PN-examples/multinet_format.lpn", std::vector<pattern>{{"in1","out1",{1,2,3,999,555,333,222}},{"in2","out2",{6735234,2,3,999,555,333,222}}});
	//test.add("../../PN-examples/multinet_format.lpn", std::vector<pattern>{{"in1","out1",{0,1,2,3,999,555,333,222}},{"in2","out2",{0,1,2,3,999,555,333,222}}});
	a=test.add("../../PN-examples/input_reduce_4.lpn", std::vector<pattern>{});
	test.find_pg(a,b);
	vector<pattern> c; vector<pattern> d;
	
	for(int i = 0; i<4;i++){
		string x = "x_in_";
		x += to_string(i+1);
		x += "a";
		cout<<x<<endl;
		c.push_back(pattern{x,"",b});
	}
	
	for(int i = 0; i<4;i++){
		string x = "x_in_";
		x += to_string(i+1);
		x += "b";
		cout<<x<<endl;
		d.push_back(pattern{x,"",b});
	}
	

	
	test.attach(c,d,GS_vars::byte_1);
	test.attach({"x_in_1a","",b});
	
	uint8_t f = 15;
	//test.set(0, f);
	test.get(0, f);

	uint8_t g = 0;
	test.get(1, g);
	
	
	cout<< "Number answer is... "<<(int)f<<endl;
	
	test.print_pl();
	
	cout<< "Number answer is... "<<(int)g<<endl;
	* 
	*/
	
	
	/*
	test.print_pl();
	
	for(int i = 0;i< 10000;i++){
		
		test.execute__base(0,Exe_mode::Random);
		test.execute__base(1,Exe_mode::Random);
		test.execute__base(2,Exe_mode::Random);
				test.execute__base(3,Exe_mode::Random);
						test.execute__base(4,Exe_mode::Random);

	}

		test.print_pl();
	*/
	return 1;
	
}
