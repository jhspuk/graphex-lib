#include <iostream>
#include <variant>
#include <vector>
#include <utility>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <tuple>

template <class T_payload, class T_connection>
struct gen_node {
	using T_self = gen_node<T_payload, T_connection>;
	
	T_payload node_payload;
	
	std::vector<gen_node<T_payload, T_connection>*> connections;
	std::vector<T_connection> arc_payload;
	
	std::vector<std::tuple<std::string,std::string>> tags;
};

template <class T_payload, class T_connection>
gen_node<T_payload, T_connection>* new_node(std::vector<gen_node<T_payload, T_connection>*>& reg, std::vector<gen_node<T_payload, T_connection>*> list){

	gen_node<T_payload, T_connection>* h = new gen_node<T_payload, T_connection>;
	
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
	
	using namespace std;
	vector<gen_node<int, int>*> reg;
	vector<gen_node<int, int>*> wl;
	
	new_node(reg, {});
	
	
	wl.push_back(new_node(reg, {reg[0]}));
	wl.push_back(new_node(reg, {reg[0]}));
	wl.push_back(new_node(reg, {reg[0]}));
	wl.push_back(new_node(reg, {reg[0]}));
	
	new_node(reg, wl);
	
	
	for(auto&i : reg){
		cout<<"node: "<<i<<endl;
		for(auto&k : i->connections){
			cout<<"connection: "<<k<<endl;
		}
	}
		
	return 0;
}
