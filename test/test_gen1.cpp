#include <iostream>
#include <variant>
#include <vector>
#include <utility>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <tuple>


using namespace std;

enum class result{
	SUCCESS,
	PASS,
	REJECT,
	LOGIC_ERROR,
	FAILURE
};

//concept, slow
template <class T_payload, class T_connection>
struct gen_node {
	using T_self = gen_node<T_payload, T_connection>;
	
	T_payload node_payload;
	
	std::vector<std::tuple<T_self*, T_connection*>> connections;
	std::vector<std::tuple<std::string,std::string>> tags;
};

class base_loader{
	public:
		base_loader() = default;
		~base_loader() = default;
		result set_path(std::string a){
			path = a;
			return result::SUCCESS;
		}
	//private:
		virtual result load(){return result::SUCCESS;};
		
		std::string path;
};

class PN_loader : public base_loader{
	public:
		PN_loader() : base_loader() {}
			
	private:
		virtual result load(){return result::SUCCESS;};
};

class graphex{
	public:
		graphex(){}
		~graphex(){}
		
		result set_loader(){
			loader = new PN_loader();
			return result::SUCCESS;
		}
		
		result set_path(std::string a){
			loader->set_path(a);
			cout<<loader->path<<endl;
			return result::SUCCESS;	
		}
		
	private:
		base_loader* loader;
	
};

int main(){

	graphex g;
	g.set_loader();
	g.set_path("here");

	return 1;
}
