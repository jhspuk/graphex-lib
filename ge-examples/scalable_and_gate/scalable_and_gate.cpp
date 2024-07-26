#include <iostream>
#include <vector>
#include <bitset>

#include "../../src/graphex.h"

using namespace graphex::petrinet;
using namespace std;

int main(){
	Graph<PL_frame<int,int>,TR_index_frame<int,int>> tm;
	int a; vector<int> b;
	a = tm.add("/home/h/Documents/workspace/work/Microsystems/tsetlin-workcraft/alex/tmCompletePetriNetDemo.lpn", std::vector<pattern>{});
	tm.find_pg(a,b);
	tm.attach({"p2","",b});
	
	tm.attach({},std::vector<pattern>{{"cl1.c0.x1TA.exc1","",b},{"cl1.c0.x1TA.exc2","",b},{"cl1.c0.x1TA.exc3","",b},{"cl1.c0.x1TA.inc4","",b},{"cl1.c0.x1TA.inc5","",b},{"cl1.c0.x1TA.inc6","",b}},GS_vars::byte_1);
	tm.attach({},std::vector<pattern>{{"cl1.c1.cx2TA.exc1","",b},{"cl1.c1.cx2TA.exc2","",b},{"cl1.c1.cx2TA.exc3","",b},{"cl1.c1.cx2TA.inc4","",b},{"cl1.c1.cx2TA.inc5","",b},{"cl1.c1.cx2TA.inc6","",b}},GS_vars::byte_1);
	
	uint8_t epochs = 10;
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
			tm.get<uint8_t>(1, ta1);
			tm.get<uint8_t>(2, ta2);
			cout<<"cl1.c0.cx1TA: "<<std::bitset<6>(ta1)<<endl;
			cout<<"cl1.c1.cx2TA: "<<std::bitset<6>(ta2)<<endl<<endl;
		}
	}
	

	//tm.get(1, ta2);
	
	
	cout<<(int)epochs<<endl;
	
	//tm.print_pl();
	
}
