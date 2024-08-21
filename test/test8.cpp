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

int main(){
	
	using namespace std;
	using node = gen_node<int, int>;
	
		
	
	return 0;
}
