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

enum class e_sl : uint8_t {
	input = 0,
	output = 1,
	input_interface = 2,
	output_interface = 3
};

struct gn_base{
	std::vector<std::pair<gn_base*,e_sl>> con;
	std::vector<std::pair<gn_base*,e_sl>> kno;
};

template <class T_payload>
struct gn_data : public gn_base{
	T_payload data;
};

struct gn_func : public gn_base{
	//function pointer here...	
	void (*func)(gn_base* ctx);
};

void add(gn_base* ctx){
	
	using T_data = gn_data<int>;

	T_data* output;

	int acc = 0;
	
	for(auto& i: ctx->con){
		if(i.second == e_sl::output){
			output = (T_data*)(i.first);
		}else if(i.second == e_sl::input){
			acc += ((T_data*)(i.first))->data;
		}
	}
	
	output->data = acc;
	
	return;
}

struct area_gn{
	std::vector<gn_base*> gn_data_reg;
	std::vector<gn_base*> gn_func_reg;
};

struct area_gn_if{
	std::vector<gn_base*> gn_data_reg;

};


int main(){

	using namespace std;

	area_gn m_reg;

	m_reg.gn_data_reg.push_back(new gn_data<int>);
	m_reg.gn_data_reg.push_back(new gn_data<int>);
	m_reg.gn_data_reg.push_back(new gn_data<int>);
	m_reg.gn_data_reg.push_back(new gn_data<int>);

	m_reg.gn_func_reg.push_back(new gn_func);

	

	return 1;
}
