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

struct gn_base{
	std::vector<std::pair<gn_base*,uint8_t>> con;
	std::vector<std::pair<gn_base*,uint8_t>> kno;
};

template <class T_payload>
struct gn_data : public gn_base{
	T_payload data;
};

struct gn_func : public gn_base{
	//function pointer here...	
};

struct area_gn{
	std::vector<gn_base*> gn_data_reg;
	std::vector<gn_base*> gn_func_reg;
};

struct area_gn_if{
	std::vector<gn_base*> gn_data_reg;

};


int main(){

	using namespace std;

	return 1;
}
