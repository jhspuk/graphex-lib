#include "graphex.h"
#include <iostream>

namespace graphex{

	namespace gen_node{
	
		e_r o_link(gn_base* a, gn_base* b, e_sl x, e_sl y){
	
			#define DEBUG_GN_OLINK
			#ifdef DEBUG_GN_OLINK
			#define D_GN_OL(x) x
			#else
			#define D_GN_OL(x)
			#endif
		
			using namespace std;

			e_r res_l = e_r::SUCCESS;

			D_GN_OL(cout<<__func__<<" Start: "<<a<<" + "<<b<<endl;)

			bool redundant = 0;

			//forward link
			if(x != e_sl::nc){

				auto& a_a = a->con;
				pair a_a_2 = {b,x};

				for(auto& i : a_a){
					if(i == a_a_2){
						res_l = e_r::REDUNDANT;
						redundant = 1;
						D_GN_OL(cout<<__func__<<" Redundant! "<<i.first<<endl;)
					}
				};

				if(!redundant)
				a_a.push_back(a_a_2);
			}

			redundant = 0;

			//backward link
			if(y != e_sl::nc){
				auto& a_b = b->con;
				pair a_b_2 = {a,y};

				for(auto& i : a_b){
					if(i == a_b_2){
						res_l = e_r::REDUNDANT;
						redundant = 1;
						D_GN_OL(cout<<__func__<<" Redundant! "<<i.first<<endl;)
					}
				};

				if(!redundant)
				a_b.push_back(a_b_2);
			}

			return res_l;
			
		}

		e_r o_unlink(gn_base* a, gn_base* b){

			#define DEBUG_GN_OUNLINK
			#ifdef DEBUG_GN_OUNLINK
			#define D_GN_OUL(x) x
			#else
			#define D_GN_OUL(x)
			#endif

			using namespace std;
			
			e_r res_l = e_r::SUCCESS;
			
			auto& a_a = a->con;
			auto& a_b = b->con;
			
			auto a_size = a_a.size();
			auto b_size = a_b.size();

			//if there exists b pointer in a's connections, erase
			//Note: regardless of system link type
			a_a.erase(
				std::remove_if(a_a.begin(), a_a.end(), 
						[&](const std::pair<gn_base*, e_sl>& i) {
						return i.first == b;  // Condition to match and remove
						}),
					a_a.end()
			);
			
			//if there exists a pointer in b's connections, erase
			//Note: regardless of system link type
			a_b.erase(
				std::remove_if(a_b.begin(), a_b.end(), 
						[&](const std::pair<gn_base*, e_sl>& i) {
						return i.first == a;  // Condition to match and remove
						}),
					a_b.end()
			);

			if(a_size == a_a.size() && b_size == a_b.size()){
				res_l = e_r::REDUNDANT;
			}
		
			return res_l;
		}

		e_r o_merge(gn_base* a, gn_base* b){
			#define DEBUG_GN_OMERGE
			#ifdef DEBUG_GN_OMERGE
			#define D_GN_OMG(x) x
			#else
			#define D_GN_OMG(x)
			#endif
		
			using namespace std;

			e_r res_l = e_r::SUCCESS;
			
			auto& a_a = a->con;
			auto& a_b = b->con;

			for(auto& i : a_b){
				if(i.second == e_sl::input
					|| i.second == e_sl::output
					|| i.second == e_sl::input_interface
					|| i.second == e_sl::output_interface)
				{
					a_a.push_back(i);
				}
			}

			return res_l;

		}

		gn_area::gn_area(){

		}

		gn_area::~gn_area(){
			
		}

		e_r gn_area::execute(){
			e_r res_l = e_r::SUCCESS;

			return res_l;
		}


		namespace conv_petrinet{

			void c_add_pn(gn_base* ctx){

				using T_data = gn_data<int>;
				std::cout<<"hello from add"<<std::endl;
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




			pn_area::pn_area() : gn_area() {

			}
			pn_area::~pn_area(){

			}
			e_r pn_area::execute(){
				e_r res_l = e_r::SUCCESS;

				return res_l;

			}

		}
	}
}



