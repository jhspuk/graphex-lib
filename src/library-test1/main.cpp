#include <iostream>
#include <tuple>
#include <vector>

#define DEBUG_BUILD

#include "graphex.h"

int main(){
	
	using namespace std;
	
	using namespace graphex::petrinet;
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
	
	uint8_t f = 15;
	test.set(0, f);
	test.get(0, f);

	cout<< "Number answer is... "<<(int)f<<endl;
	
	test.print_pl();
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
