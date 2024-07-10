#include <iostream>
#include <tuple>
#include <vector>

#include "graphex.h"

int main(){
	
	using namespace std;
	vector<tuple<int,int>> tup;
	
	using namespace graphex::petrinet;
	Graph<PL_frame<int,int>,TR_index_frame<int,int>> test;
	test.add("../../PN-examples/multinet_format.lpn", tup);

	return 1;
	
}
