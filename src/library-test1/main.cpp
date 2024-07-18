#include <iostream>
#include <tuple>
#include <vector>

#define DEBUG_BUILD

#include "graphex.h"

int main(){
	
	using namespace std;
	
	using namespace graphex::petrinet;
	Graph<PL_frame<int,int>,TR_index_frame<int,int>> test;
	//test.add("../../PN-examples/multinet_format.lpn", std::vector<pattern>{{"in1","out1",{1,2,3,999,555,333,222}},{"in2","out2",{6735234,2,3,999,555,333,222}}});
	//test.add("../../PN-examples/multinet_format.lpn", std::vector<pattern>{{"in1","out1",{0,1,2,3,999,555,333,222}},{"in2","out2",{0,1,2,3,999,555,333,222}}});
	test.add("../../PN-examples/pipe_supplement_2.lpn", std::vector<pattern>{});
	test.add("../../PN-examples/pipe_supplement_2.lpn", std::vector<pattern>{{"in","out",{1,2,3,4,5,6,7}}});
	test.add("../../PN-examples/pipe_supplement_2.lpn", std::vector<pattern>{{"in","out",{3,4,5,6,7}}});
	

	test.print_pl();
	
	for(int i = 0;i< 10000;i++){
		
		test.execute__base(0,Exe_mode::Random);
		test.execute__base(1,Exe_mode::Random);
		test.execute__base(2,Exe_mode::Random);

	}

		test.print_pl();

	return 1;
	
}
