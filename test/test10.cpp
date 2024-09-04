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

namespace graphex{
	namespace gen_node{
		enum class e_sl : uint8_t {
			//exe
			input,
			output,
			input_interface,
			output_interface,
			//k
			k_in_func,
			k_out_func,
			k_in_data,
			k_out_data
		};

		struct gn_base{
			std::vector<std::pair<gn_base*,e_sl>> con;
		};

		template <class T_payload>
			struct gn_data : public gn_base{
				T_payload data;
			};

		struct gn_func : public gn_base{
			//function pointer here...	
			void (*func)(gn_base* ctx);
		};

		void o_join(gn_base* a, gn_base* b, uint8_t mode){
			switch(mode){
				//forwards join, given a, b is output
				case 0:
					b->con.push_back({a,e_sl::input});
					a->con.push_back({b,e_sl::output});
					break;
				//backwards join, given a, b is input
				case 1:
					b->con.push_back({a,e_sl::output});
					a->con.push_back({b,e_sl::input});
					break;
				//symmetric join, input
				case 2:
					b->con.push_back({a,e_sl::input});
					a->con.push_back({b,e_sl::input});
					break;
				//symmetric join, output
				case 3:
					b->con.push_back({a,e_sl::output});
					a->con.push_back({b,e_sl::output});
					break;
				default:
					break;
			}

			return;
		}

		struct area_gn{
			std::vector<gn_base*> gn_data_reg;
			std::vector<gn_base*> gn_func_reg;
		};

		struct area_gn_if{
			std::vector<gn_base*> gn_data_reg;

		};
	}
}
int main(){

	using namespace std;

	area_gn m_reg;

	int counter = 5;
	gn_data<int>* z_a;

	for(int i = 0; i < 10; i++){	
		z_a = new gn_data<int>;
		z_a->data = counter++;
		m_reg.gn_data_reg.push_back(z_a);
	}

	gn_func* z_b;
	z_b = new gn_func;
	z_b->func = f_add;
	m_reg.gn_func_reg.push_back(z_b);

	for(auto& i : m_reg.gn_data_reg){
		f_join(z_b, i, 1);
	}	

	z_a = new gn_data<int>;
	z_a->data = 0;
	m_reg.gn_data_reg.push_back(z_a);

	f_join(z_b, z_a, 0);
	cout<<"Data nodes..."<<endl;

	z_b->func(z_b);

	for(auto& i : m_reg.gn_data_reg){
		cout<<"Node: "<<i<<" Data: "<<((gn_data<int>*)i)->data<<endl;
		for(auto& k : i->con){
			cout<<k.first<<" + "<<(int)(k.second)<<endl;
		}
	}
	
	cout<<"Function nodes..."<<endl;

	for(auto& i : m_reg.gn_func_reg){
		cout<<"Node: "<<i<<endl;
		for(auto& k : i->con){
			cout<<k.first<<" + "<<(int)(k.second)<<endl;
		}
	}
	


	return 1;
}