#include <iostream>
#include <tuple>
#include <vector>

#include "graphex.h"

int main(){
	
	using namespace std;
	
	using namespace graphex::petrinet;
	Graph<PL_frame<int,int>,TR_index_frame<int,int>> test;
	test.add("../../PN-examples/multinet_format.lpn", std::vector<pattern>{{"in1","out1",{1,2,3,999,555,333,222}},{"in2","out2",{6735234,2,3,999,555,333,222}}});

	return 1;
	
}
