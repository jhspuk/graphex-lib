#include <iostream>
#include <variant>
#include <vector>
#include <utility>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <tuple>
#include <string>
#include <mutex>
#include <fstream>
#include <sstream>

template <class T_payload, class T_connection>
struct gen_node {
	using T_self = gen_node<T_payload, T_connection>;
	
	T_payload node_payload;
	
	std::vector<gen_node<T_payload, T_connection>*> connections;
	std::vector<T_connection> arc_payload;
	
	std::vector<std::tuple<std::string,std::string>> tags;
	
};

class gen_node {
	public:
		gen_node();
	private:
		
};

class pn_node : public gen_node{
	
};

int lpn_loader(std::string path, gen_node<int, int>*){
	
	using namespace std;
	
	ifstream file(path);
	
	unordered_map<string, gen_node<int,int>*> f_tr_map;
	unordered_map<string, gen_node<int,int>*> f_pl_map;
	
	string prefix_shared = "x_";
	
	if(!file.is_open()){
		cerr<<"Cannot open file: "<<path<<endl;
		return -1;
	}
	

	return 1;
}

int main(){
	
	using namespace std;
	using node = gen_node<int, int>;
	
	node * a = new node;
	
	lpn_loader("./test_pn/g_scalable_and_gate.lpn", a);
	
	return 0;
}
