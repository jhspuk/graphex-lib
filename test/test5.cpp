//This is a test for the first rejig of graphEx - creating an architecture
//that is mainly independent of content, for executing and analysing
//graph structures. One big problem of the prior version was being able
//to manipulate structures already brought into memory, such as post
//join() functionality. This will address this by transitioning between
//concept datastructures, and implementation data strctures. And by keeping
//agnostic the backbone of the program.

//Another large issue with the previous issue is a specific issue with
//how cpp processes templates - the types must be specified at all levels
//which makes writing code cumbersome. This version will take advantage 
//of std::variant (or similar) to provide type erasure, and therefore a neutral class can be called

//This test is simply to create a number of nodes connected to one another

#include <iostream>
#include <variant>
#include <vector>
#include <utility>
#include <unordered_map>
#include <random>
#include <algorithm>


using namespace std;

//concept, slow
template <class T_payload, class T_connection>
struct concept_node {
	T_payload node_payload;
	
	std::vector<concept_node<T_payload, T_connection>*> connections;
	std::vector<T_connection> arc_payload;
	
	std::vector<std::pair<std::string,std::string>> tags;
};

//implementation, fast

concept_node<int, int>* new_node(vector<concept_node<int, int>*>& reg, vector<concept_node<int, int>*> list){

	concept_node<int, int>* h = new concept_node<int, int>;
	
	for(auto&i : list){
		if(find(h->connections.begin(), h->connections.end(), i) == h->connections.end())
		h->connections.push_back(i);
		if(find(i->connections.begin(), i->connections.end(), h) == i->connections.end())
		i->connections.push_back(h);
	}
	
	reg.push_back(h);
	
	return h;
}

int main(){

	vector<concept_node<int, int>*> reg;
	
	unordered_map<concept_node<int, int>*, int> eas;
	
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	 // define the range
	
	eas.insert({new_node(reg, {}), 0});
	
	static int c_n = 30;
	
	//create new nodes with (maximum) c_n connections each
	int counter = 0;
	for(int i = 0; i < 100; i++){
	
		if(reg.empty()) continue;
		vector<concept_node<int, int>*> wl;
		std::uniform_int_distribution<> distr(0, reg.size() - 1);
		for(int k = 0; k < c_n; k++){
			wl.push_back(reg[distr(gen)]);
		}
		eas.insert({new_node(reg, wl), i + 1});
	}
	
	//directory hash map
	for(auto& i : eas){
		cout<<"First: "<<i.first<<" Second: "<<i.second<<endl; 
	}
	
	//show all connections
	for(auto&i : reg){
		cout<<"node: "<<i<<endl;
		for(auto&k : i->connections){
			cout<<"connection: "<<eas.at(k)<<endl;
		}
	}
	

	return 1;
}
