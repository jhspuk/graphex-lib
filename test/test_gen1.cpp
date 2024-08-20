#include <iostream>
#include <variant>
#include <vector>
#include <utility>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <tuple>


using namespace std;

//concept, slow
template <class T_payload, class T_connection>
struct gen_node {
	T_payload node_payload;
	
	std::vector<std::tuple<gen_node<T_payload, T_connection>*, T_connection*>> connections;
	std::vector<std::tuple<std::string,std::string>> tags;
	
	//atomic search
	size_t find_payload(T_payload search_term){
		
	}
	
	size_t find_connection(T_connection search_term){
		
	}
	//atomic modify
	size_t add_connection();
};

/*
class gen_controller{
public:
		gen_controller();
		
		search();
		paint();
		modify();
		execute();
		
		attach();
		get();
		set();
		
private:
		gen_node<T_payload, T_connection>* root;
		std::vector< gen_node<T_payload, T_connection>* > search_results;
};

class imp_PN_cpu{
	public:
		imp_PN_cpu();
	private:
		execute();
		
};
*/

int main(){



	return 1;
}
