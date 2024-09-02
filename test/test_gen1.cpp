

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
struct gen_node {
	T_payload node_payload;
	
	std::vector< gen_node<T_payload, T_connection>* > connections;
	std::vector<T_connection*> arc_payload;
	
	std::vector< std::pair<std::string,std::string> > tags;
};

//obsolete
template <class T_payload, class T_connection>
struct gen_node_area{
	std::vector< gen_node<T_payload,T_connection>* >
	
	std::vector< std::pair<std::string,std::string> > tags;
};

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

int main(){



	return 1;
}
