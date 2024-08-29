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

void lpn_loader(std::string path){
	
	using namespace std;
	
	ifstream file(path);
	
	if(!file.is_open()){
		cerr<<"Cannot open file: "<<path<<endl;
		return -1;
	}
	
}

int main(){
	
	using namespace std;
	using node = gen_node<int, int>;
	
	lpn_loader("./test_pn/g_scalable_and_gate.lpn");
	
	return 0;
}
